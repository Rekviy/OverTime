#include "application.h"
#include "core.h"
#include "core/log.h"
#include "renderer/renderer.h"

#include <GLFW/glfw3.h>

namespace overtime {

	application* application::s_Instance = nullptr;
	application::application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f)
	{
		OT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<window>(window::create());
		m_Window->setEventCallback(OT_BIND_EVENT_FN(application::onEvent));

		m_ImGuiLayer = new imGuiLayer();
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
			layout(location = 0) in vec3 position;
			
			uniform mat4 u_ViewProj;
			
			out vec3 v_Pos;

			void main()
			{
				v_Pos = position;
				gl_Position = u_ViewProj * vec4(position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			layout(location = 0) out vec4 color;
			in vec3 v_Pos;

			void main()
			{
				vec3 col = 0.5 + 0.5*cos(v_Pos.xyx+vec3(0,2,4));

				// Output to screen
				color = vec4(col,1.0);
			}
		)";


		m_Shader.reset(new shader(vertexSrc, fragmentSrc));
	}

	application::~application() {}

	void application::pushLayer(layer* layer)
	{
		m_LayerStack.pushLayer(layer);
	}
	void application::pushOverlay(layer* layer)
	{
		m_LayerStack.pushOverlay(layer);
	}
	void application::pushImGuiLayer(layer* layer)
	{
		m_ImGuiLayer->pushLayer(layer);
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

			m_Camera.setPosition({ -0.5f,-0.5f, 0.0f });
			m_Camera.setRotation(45.0f);

			renderer::beginScene(m_Camera);
			
			renderer::submit(m_VertexArray, m_Shader);

			renderer::endScene();

			for (layer* layer : m_LayerStack)
				layer->onUpdate();

			m_Window->onUpdate();
		}
	}
	bool application::onWindowClose(windowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

}

