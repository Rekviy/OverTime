#pragma once
#ifndef KEY_EVENT_H
#define KEY_EVENT_H
#include "event.h"

namespace overtime {
	class keyEvent : public event {
	public:
		inline int getKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(eventCategoryKeyboard | eventCategoryInput)
	protected:
		keyEvent(unsigned int keycode)
			: m_KeyCode(keycode)
		{}
		unsigned int m_KeyCode;
	};
	
	class keyPressedEvent : public keyEvent {
	public:
		keyPressedEvent(unsigned int keycode, int repeatCount)
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

	class keyReleasedEvent : public keyEvent {
	public:
		keyReleasedEvent(unsigned int keycode)
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
	class keyTypedEvent : public keyEvent {
	public:
		keyTypedEvent(unsigned int keycode)
			: keyEvent(keycode)
		{}
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "keyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(keyTyped)
	};
}
#endif