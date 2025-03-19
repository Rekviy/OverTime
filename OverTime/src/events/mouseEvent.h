#pragma once
#include "Event.h"

namespace overtime {
	class mouseMovedEvent : public event {
	public:
		mouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y)
		{}
		inline float getX() const { return m_MouseX; }
		inline float getY() const { return m_MouseY; }
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "mouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class mouseScrolledEvent : public event {
	public:
		mouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset)
		{}
		inline float getXOffset() const { return m_XOffset; }
		inline float getYOffset() const { return m_YOffset; }
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "mouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class mouseButtonEvent : public event {
	public:
		inline int getMouseButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		mouseButtonEvent(int button)
			: m_Button(button)
		{}
		int m_Button;
	};

	class mouseButtonPressedEvent : public mouseButtonEvent {
	public:
		mouseButtonPressedEvent(int button)
			: mouseButtonEvent(button)
		{}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "mouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class mouseButtonReleasedEvent : public mouseButtonEvent {
	public:
		mouseButtonReleasedEvent(int button)
			: mouseButtonEvent(button)
		{}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "mouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}