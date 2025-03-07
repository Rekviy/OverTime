#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include <string>

#include "core.h"
#include "events/event.h"

namespace overtime {
	struct windowProps {
		windowProps(const std::string& title = "OverTime Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: title(title), width(width), height(height) 
		{}

		std::string title;
		unsigned int width;
		unsigned int height;
	};

	// Interface representing a desktop system based Window
	class window {
	public:
		using eventCallbackFn = std::function<void(event&)>;
		virtual ~window() {}
		virtual void onUpdate() = 0;
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		// Window attributes
		virtual void setEventCallback(const eventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static window* create(const windowProps& props = windowProps());
	};
}
#endif