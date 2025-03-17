#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include <memory>

#include "window.h"
//#include "input.h"
#include "layerStack.h"
#include "imGui/imguiLayer.h"
//#include "keyCodes.h"
#include "events/applicationEvent.h"
#include "renderer/shader.h"

namespace overtime {
	class application {
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
		unsigned m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<shader> m_Shader;
	};

	// To be defined in CLIENT
	application* createApplication();


}
#endif