#include "winput.h"
#include "core/application.h"
#include <GLFW/glfw3.h>
namespace overtime {
	input* input::s_Instance = new windowsInput();
	bool windowsInput::isKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*> (application::getInst().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool windowsInput::isMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*> (application::getInst().getWindow().getNativeWindow());
		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	}

	std::pair<float, float> windowsInput::getMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*> (application::getInst().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float windowsInput::getMouseXImpl()
	{
		auto [x, y] = getMousePosImpl();
		return x;
	}

	float windowsInput::getMouseYImpl()
	{
		auto [x, y] = getMousePosImpl();
		return y;
	}
}