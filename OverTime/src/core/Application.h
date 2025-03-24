#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "platform/window.h"
#include "platform/input.h"
#include "layers/layerStack.h"
#include "imGui/imguiLayer.h"
#include "codes/keyCodes.h"
#include "events/applicationEvent.h"
#include "timeStep.h"

#include <memory>

namespace overtime {
	class application {
	public:
		application();
		virtual ~application();

		void run();
		void onEvent(event& event);

		void pushLayer(layer* layer);
		void pushOverlay(layer* layer);
		void pushImGuiLayer(layer* layer);
		inline window& getWindow() { return *m_Window; }
		static inline application& getInst() { return *s_Instance; }
	private:
		imGuiLayer* m_ImGuiLayer;
		bool onWindowClose(windowCloseEvent &event);

		static application* s_Instance;
		layerStack m_LayerStack;
		std::unique_ptr<window> m_Window;
		bool m_Running = true;

	};

	// To be defined in CLIENT
	application* createApplication();


}
#endif