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

int main(int argc, char** argv);

namespace overtime {
	class application {
	public:
		application();
		virtual ~application();

		void onEvent(event& event);

		void pushLayer(layer* layer);
		void pushOverlay(layer* overlay);
		inline window& getWindow() { return *m_Window; }
		static inline application& getInst() { return *s_Instance; }
	private:
		void run();
		friend int ::main(int argc, char** argv);
		bool onWindowClose(windowCloseEvent &event);
		bool onWindowResize(windowResizeEvent& event);

		static application* s_Instance;
		imGuiLayer* m_ImGuiLayer;
		scope<window> m_Window;
		layerStack m_LayerStack;
		bool m_Running = true;
		bool m_Minimized = false;
	};

	// To be defined in CLIENT
	application* createApplication();


}
#endif