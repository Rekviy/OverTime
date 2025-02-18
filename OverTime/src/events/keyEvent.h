#pragma once
#ifndef KEY_EVENT_H
#define KEY_EVENT_H
#include "event.h"

namespace overtime {
	class OVERTIME_API keyEvent : public event {
	public:
		inline int getKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(eventCategoryKeyboard | eventCategoryInput)
	protected:
		keyEvent(int keycode)
			: m_KeyCode(keycode)
		{}
		int m_KeyCode;
	};

	class OVERTIME_API keyPressedEvent : public keyEvent {
	public:
		keyPressedEvent(int keycode, int repeatCount)
			: keyEvent(keycode), m_RepeatCount(repeatCount)
		{}
		inline int getRepeatCount() const { return m_RepeatCount; }
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "keyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(keyPressed)
	private:
		int m_RepeatCount;
	};

	class OVERTIME_API keyReleasedEvent : public keyEvent {
	public:
		keyReleasedEvent(int keycode)
			: keyEvent(keycode)
		{}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "keyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(keyReleased)
	};
}
#endif