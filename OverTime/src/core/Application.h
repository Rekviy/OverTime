#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "core.h"
#include "events/event.h"
#include "events/applicationEvent.h"
#include "window.h"

namespace overtime {
	class OVERTIME_API application {
	public:
		application();
		virtual ~application();

		void run();
		void onEvent(event& event);

	private:
		bool onWindowClose(windowCloseEvent &event);
		std::unique_ptr<window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLIENT
	application* createApplication();


}
#endif