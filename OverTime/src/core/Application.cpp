#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application.h"
#include "core.h"
#include "core/log.h"

namespace overtime {

	application* application::s_Instance = nullptr;
	application::application()
	{
		OT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<window>(window::create());
		m_Window->setEventCallback(OT_BIND_EVENT_FN(application::onEvent));

		m_ImGuiLayer = new overtime::imGuiLayer();
		pushOverlay(m_ImGuiLayer);
		
		
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

	
		float vertices[3*3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};
		m_VertexBuffer.reset(vertexBuffer::create(vertices, sizeof(vertices)));
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);


		unsigned indices[3] = { 0,1,2 };
		m_IndexBuffer.reset(indexBuffer::create(indices, 3));


		std::string vertexSrc = R"(

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			uniform float time;
			vec3 fragCoord = gl_FragCoord.xyz;

			void main()
			{
				// Normalized pixel coordinates (from 0 to 1)
				vec3 uv = fragCoord/v_Position.xyz;

				// Time varying pixel color
				vec3 col = 0.5 + 0.5*cos(time+uv.xyx+vec3(0,2,4));

				// Output to screen
				color = vec4(col,1.0);
			}
		)";


		m_Shader.reset(new shader(vertexSrc,fragmentSrc));
	}

	application::~application()
	{

	}

	void application::pushLayer(layer* layer)
	{
		m_LayerStack.pushLayer(layer);
	}
	void application::pushOverlay(layer* overlay)
	{
		m_LayerStack.pushOverlay(overlay);
	}

	void application::onEvent(event& event)
	{
		eventDispatcher dispatcher(event);
		dispatcher.dispatch<windowCloseEvent>(OT_BIND_EVENT_FN(application::onWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(event);
			if (event.isHandled())
				break;
		}
		//OT_CORE_TRACE("{0}", event.toString());
	}

	void application::run()
	{
		while (m_Running) {
			glClearColor(0, 0.6f, 0.6f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Shader->bind();
			glUniform1f(glGetUniformLocation(m_Shader->getRendererId(), "time"), glfwGetTime());
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (layer* layer : m_LayerStack)
				layer->onUpdate();
			
			m_ImGuiLayer->begin();
			for (layer* layer : m_LayerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->end();

			m_Window->onUpdate();
		}
	}
	bool application::onWindowClose(windowCloseEvent &event)
	{
		m_Running = false;
		return true;
	}

}

