#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "core/core.h"

#include <iostream>
#include <sstream>
#include <functional>

namespace overtime {

	enum class eventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum eventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static eventType getStaticType() { return eventType::type; }\
								virtual eventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class event {
		friend class eventDispatcher;

	public:
		virtual eventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }
		inline bool isHandled() { return m_Handled; }
		inline bool isInCategory(eventCategory category) { return getCategoryFlags() & category; }

	protected:
		bool m_Handled = false;
	};

	class eventDispatcher {
	public:
		eventDispatcher(event& event) : m_Event(event) {}

		template<typename T, typename F>
		bool dispatch(const F& func)
		{
			if (m_Event.getEventType() == T::getStaticType()) {
				//ascend parent to derived class and give it to given func
				m_Event.m_Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		event& m_Event;
	};
	inline std::ostream& operator<<(std::ostream& os, const event& e) { return os << e.toString(); }
}
#endif