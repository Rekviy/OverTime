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

	void application::onEvent(event& event)
	{
		eventDispatcher dispatcher(event);
		dispatcher.dispatch<windowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		OT_CORE_TRACE("{0}", event.toString());
	}

	void application::run()
	{
		while (m_Running) {
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->onUpdate();
		}
	}
	bool application::onWindowClose(windowCloseEvent &event)
	{
		m_Running = false;
		return true;
	}

}

