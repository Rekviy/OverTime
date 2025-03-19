#pragma once
#ifndef WWINDOW_H
#define WWINDOW_H

#include "renderer/renderContext.h"
#include "core/window.h"

#include <string>

struct GLFWwindow;

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
		inline virtual void* getNativeWindow() const { return m_Window; }
	private:
		virtual void init(const windowProps& props);
		virtual void shutdown();

		renderContext* m_Context;
		GLFWwindow* m_Window;
		struct windowData {
			std::string title;
			unsigned int width = 640, height = 480;
			bool VSync = 0;
			eventCallbackFn eventCallback;
		};
		windowData m_Data;
	};
}

#endif