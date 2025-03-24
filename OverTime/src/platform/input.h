#pragma once
#ifndef INPUT_H
#define INPUT_H
#include <utility>

namespace overtime {
	class input {
	public:
		static inline bool isKeyPressed(int keycode) { return s_Instance->isKeyPressedImpl(keycode); }
		static inline bool isMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedImpl(button); }
		static inline std::pair<float, float> getMousePos() { return s_Instance->getMousePosImpl(); }
		static inline float getMouseX() { return s_Instance->getMouseXImpl(); }
		static inline float getMouseY() { return s_Instance->getMouseYImpl(); }

	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float,float> getMousePosImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;

	private:
		static input* s_Instance;
	};
}

#endif