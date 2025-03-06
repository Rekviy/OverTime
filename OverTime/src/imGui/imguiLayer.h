#pragma once
#ifndef IMGUILAYER_H
#define IMGUILAYER_H

//temp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "core/layer.h"

#include "events/event.h"
#include "events/applicationEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

namespace overtime {
	class OVERTIME_API imGuiLayer : public layer {
	public:
		imGuiLayer();
		~imGuiLayer() override;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;
		void begin();
		void end();
	};

	

}
#endif