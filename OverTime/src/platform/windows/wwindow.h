#pragma once
#ifndef WWINDOW_H
#define WWINDOW_H

#include "core/window.h"
#include <GLFW/glfw3.h>
namespace overtime {
	class windowsWindow : public window {
	public:
		windowsWindow(const windowProps& props);
		virtual ~windowsWindow();
		void onUpdate() override;
		inline unsigned int getWidth() const override { return m_Data.width; }
		inline unsigned int getHeight() const override { return m_Data.height; }
		// Window attributes
		inline void setEventCallback(const eventCallbackFn& callback) override { m_Data.eventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;
	private:
		virtual void init(const windowProps& props);
		virtual void shutdown();
	private:
		GLFWwindow* m_Window;
		struct windowData {
			std::string title;
			unsigned int width, height;
			bool VSync;
			eventCallbackFn eventCallback;
		};
		windowData m_Data;
	};
}

#endif