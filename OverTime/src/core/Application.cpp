#include "application.h"
#include "core/log.h"
#include <GLFW/glfw3.h>

namespace overtime {
#define BIND_EVENT_FN(x) std::bind(&application::x, this, std::placeholders::_1)
	application::application()
	{
		m_Window = std::unique_ptr<window>(window::create());
		m_Window->setEventCallback(BIND_EVENT_FN(onEvent));
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
		dispatcher.dispatch<windowCloseEvent>(BIND_EVENT_FN(onWindowClose));

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
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (layer* layer : m_LayerStack)
				layer->onUpdate();
			m_Window->onUpdate();
		}
	}
	bool application::onWindowClose(windowCloseEvent &event)
	{
		m_Running = false;
		return true;
	}

}

