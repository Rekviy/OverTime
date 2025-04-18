#pragma once
#ifndef WINPUT_H
#define WINPUT_H

#include "platform/input.h"

#include <utility>

namespace overtime {
	class windowsInput : public input{
		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> getMousePosImpl() override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
	};
}
#endif