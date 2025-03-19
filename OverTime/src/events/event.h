#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "core/core.h"

#include <iostream>
#include <sstream>
#include <functional>

namespace overtime {

	enum class eventType {
		none = 0,
		windowClose, windowResize, windowFocus, windowLostFocus, windowMoved,
		appTick, appUpdate, appRender,
		keyPressed, keyReleased, keyTyped,
		mouseButtonPressed, mouseButtonReleased, mouseMoved, mouseScrolled
	};

	enum eventCategory {
		none = 0,
		eventCategoryApplication = BIT(0),
		eventCategoryInput = BIT(1),
		eventCategoryKeyboard = BIT(2),
		eventCategoryMouse = BIT(3),
		eventCategoryMouseButton = BIT(4)
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
		template<typename T>
		using eventFn = std::function<bool(T&)>;
	public:
		eventDispatcher(event& event) : m_Event(event) {}

		template<typename T>
		bool dispatch(eventFn<T> func)
		{
			if (m_Event.getEventType() == T::getStaticType()) {
				//ascend parrent to derived class and give it to given func
				m_Event.m_Handled = func(*(T*)&m_Event);
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