#include "wwindow.h"
#include "core/core.h"
#include "core/log.h"
#include "events/applicationEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

namespace overtime {

	static bool s_GLFWInitialized = false;
	static void glfwErrorCallback(int error, const char* description)
	{
		OT_CORE_ERROR("GLFW ERROR: [{0}] - {1}", error, description);
	}
	window* window::create(const windowProps& props)
	{
		return new windowsWindow(props);
	}
	windowsWindow::windowsWindow(const windowProps& props)
	{
		init(props);
	}
	windowsWindow::~windowsWindow()
	{
		shutdown();
	}
	void windowsWindow::init(const windowProps& props)
	{
		m_Data.title = props.title;
		m_Data.width = props.width;
		m_Data.height = props.height;
		OT_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);
		if (!s_GLFWInitialized) {
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			OT_CORE_ASSERT(success, "Failed to intialize GLFW!");
			glfwSetErrorCallback(glfwErrorCallback);
			s_GLFWInitialized = true;
		}
		m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OT_CORE_ASSERT(status, "Failde to intialize Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		setVSync(true);
		//set glfw callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			data.width = width; data.height = height;

			windowResizeEvent event(width, height);
			data.eventCallback(event);
		});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			windowCloseEvent event;
			data.eventCallback(event);
		});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS: {
					keyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					keyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					keyPressedEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			keyTypedEvent event(keycode);
			data.eventCallback(event);
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS: {
					mouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					mouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
			}
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			mouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
			mouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});
	}
	void windowsWindow::shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
	void windowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
	void windowsWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = enabled;
	}
	bool windowsWindow::isVSync() const
	{
		return m_Data.VSync;
	}
}