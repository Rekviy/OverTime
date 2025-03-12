#pragma once
#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "renderer/renderContext.h"

struct GLFWwindow;

namespace overtime {
	class openGLContext : public renderContext {
	public:
		openGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swap() override;


	private:
		GLFWwindow* m_WindowHandle;
	};
}
#endif