#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
//#include "input.h"
#include "layerStack.h"
#include "imGui/imguiLayer.h"
//#include "keyCodes.h"
#include "events/applicationEvent.h"
#include "renderer/shader.h"
#include "renderer/buffer.h"
#include "renderer/vertexArray.h"
#include "renderer/camera.h"
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

		std::unique_ptr<window> m_Window;
		bool m_Running = true;
		layerStack m_LayerStack;
		std::shared_ptr<vertexArray> m_VertexArray;
		std::shared_ptr<shader> m_Shader;
		orthographCamera m_Camera;
	};

	// To be defined in CLIENT
	application* createApplication();


}
#endif