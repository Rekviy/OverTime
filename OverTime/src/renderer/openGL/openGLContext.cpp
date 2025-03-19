#include "openGLContext.h"
#include "core/log.h"
#include "core/core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace overtime {
	openGLContext::openGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle) {}
	void openGLContext::init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OT_CORE_ASSERT(status, "Failed to intialize Glad!");
		OT_CORE_INFO("OpenGL Info:\n Vendor: {0}\n Renderer: {1}\n Version: {2}", 
			(const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER), (const char*)glGetString(GL_VERSION));

	}
	void openGLContext::swap()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}