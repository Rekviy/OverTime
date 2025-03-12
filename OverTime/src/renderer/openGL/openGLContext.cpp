#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "openGLContext.h"
namespace overtime {
	openGLContext::openGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle) {}
	void openGLContext::init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OT_CORE_ASSERT(status, "Failed to intialize Glad!");
	}
	void openGLContext::swap()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}