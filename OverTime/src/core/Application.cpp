#include "application.h"
#include "core.h"
#include "core/log.h"
#include "renderer/renderer.h"

#include <GLFW/glfw3.h>

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
		
		m_VertexArray.reset(vertexArray::create());
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			  0.0f, 0.5f, 0.0f,
		};
		std::shared_ptr<vertexBuffer> vBuffer;
		vBuffer.reset(vertexBuffer::create(vertices, sizeof(vertices)));

		bufferLayout layout = {
			{shaderDataType::Float3, "a_Pos"}
		};

		vBuffer->setLayout(layout);
		m_VertexArray->addVertexBuffer(vBuffer);

		unsigned indices[3] = { 0,1,2 };
		std::shared_ptr<indexBuffer> iBuffer;
		iBuffer.reset(indexBuffer::create(indices, 3));
		m_VertexArray->setIndexBuffer(iBuffer);

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

			void main()
			{
				vec3 col = 0.5 + 0.5*cos(v_Position.xyx+vec3(0,2,4));

				// Output to screen
				color = vec4(col,1.0);
			}
		)";


		m_Shader.reset(new shader(vertexSrc, fragmentSrc));
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
			rendererAPI::setClearColor({ 0, 0.6f, 0.6f, 1 });
			rendererAPI::clear();
			m_Shader->bind();

			renderer::submit(m_VertexArray);
			
			for (layer* layer : m_LayerStack)
				layer->onUpdate();

			m_ImGuiLayer->begin();
			for (layer* layer : m_LayerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->end();

			m_Window->onUpdate();
		}
	}
	bool application::onWindowClose(windowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

}

