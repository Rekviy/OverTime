#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "core.h"
#include "window.h"
#include "input.h"
#include "layerStack.h"
#include "imGui/imguiLayer.h"
#include "keyCodes.h"
#include "events/event.h"
#include "events/applicationEvent.h"

namespace overtime {
	class OVERTIME_API application {
	public:
		application();
		virtual ~application();

		void run();
		void onEvent(event& event);

		void pushLayer(layer* layer);
		void pushOverlay(layer* overlay);
		inline window& getWindow() { return *m_Window; }
		static inline application& getInst() { return *s_Instance; }
	private:
		imGuiLayer* m_ImGuiLayer;
		bool onWindowClose(windowCloseEvent &event);

		static application* s_Instance;

		
		std::unique_ptr<window> m_Window;
		bool m_Running = true;
		layerStack m_LayerStack;
	};

	// To be defined in CLIENT
	application* createApplication();


}
#endif