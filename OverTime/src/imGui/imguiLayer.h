#pragma once
#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "core/layer.h"
#include "core/application.h"
#include "events/event.h"
#include "events/applicationEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <backends/imgui_impl_opengl3.h>
//#include "platform/openGL/imguiOpenGLRenderer.h"
//temp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace overtime {
	class OVERTIME_API imGuiLayer : public layer {
	public:
		imGuiLayer();
		~imGuiLayer();
		void onAttach();
		void onDetach();
		void onUpdate();
		void onEvent(event& event);
		bool onMouseButtonPressedEvent(mouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(mouseButtonReleasedEvent& e);
		bool onMouseMovedEvent(mouseMovedEvent& e);
		bool onMouseScrolledEvent(mouseScrolledEvent& e);
		bool onKeyPressedEvent(keyPressedEvent& e);
		bool onKeyReleasedEvent(keyReleasedEvent& e);
		bool onKeyTypedEvent(keyTypedEvent& e);
		bool onWindowResizeEvent(windowResizeEvent& e);
		ImGuiKey glfw_KeyToImGuiKey(int keycode);
	private:
		float m_time = 0.0f;
	};

	

}
#endif