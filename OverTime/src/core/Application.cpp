#include "application.h"
#include "core.h"
#include "core/log.h"

#include <GLFW/glfw3.h>

namespace overtime {

	application* application::s_Instance = nullptr;
	application::application()

	{
		OT_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<window>(window::create());
		m_Window->setEventCallback(OT_BIND_EVENT_FN(application::onEvent));

		m_ImGuiLayer = new imGuiLayer();
		pushOverlay(m_ImGuiLayer);
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
		float lastFrameTime = 0.0f;
		while (m_Running) {
			float time = glfwGetTime();
			timeStep ts = time - lastFrameTime;
			lastFrameTime = time;

			for (layer* layer : m_LayerStack)
				layer->onUpdate(ts);

			m_Window->onUpdate();
		}
	}
	bool application::onWindowClose(windowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

}

