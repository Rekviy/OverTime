#pragma once
#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H
#include "event.h"

namespace overtime {
	class OVERTIME_API windowResizeEvent : public event {
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
		EVENT_CLASS_TYPE(windowResize)
		EVENT_CLASS_CATEGORY(eventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class OVERTIME_API windowCloseEvent : public event {
	public:
		windowCloseEvent() {}
		EVENT_CLASS_TYPE(windowClose)
		EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};
	class OVERTIME_API appTickEvent : public event {
	public:
		appTickEvent() {}
		EVENT_CLASS_TYPE(appTick)
		EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};
	class OVERTIME_API appUpdateEvent : public event {
	public:
		appUpdateEvent() {}
		EVENT_CLASS_TYPE(appUpdate)
		EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};
	class OVERTIME_API appRenderEvent : public event {
	public:
		appRenderEvent() {}
		EVENT_CLASS_TYPE(appRender)
		EVENT_CLASS_CATEGORY(eventCategoryApplication)
	};
}


#endif