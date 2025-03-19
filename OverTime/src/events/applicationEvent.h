#pragma once
#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H
#include "event.h"

namespace overtime {
	class windowResizeEvent : public event {
	public:
		windowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height)
		{}
		inline unsigned int getWidth() const { return m_Width; }
		inline unsigned int getHeight() const { return m_Height; }
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "windowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class windowCloseEvent : public event {
	public:
		windowCloseEvent() {}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class appTickEvent : public event {
	public:
		appTickEvent() {}
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class appUpdateEvent : public event {
	public:
		appUpdateEvent() {}
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class appRenderEvent : public event {
	public:
		appRenderEvent() {}
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}


#endif