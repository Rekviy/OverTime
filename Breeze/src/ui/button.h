#pragma once
#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include "interactElement.h"
#include "themeManager.h"
#include "array"
enum class buttonState {
	idle = 0, hover, clicked, stateCount
};
class button : public interactElement {
public:

	button(const std::string& name, const glm::vec3& position, const glm::vec2& size, std::array<std::string, (size_t)buttonState::stateCount>&& keys,
		std::function<bool(button*)>&& funcOnRelease, std::function<bool(button*)>&& funcOnPress = [](button* btn) {return true; }, bool isActive = false);
	virtual ~button() = default;

	virtual void reset() override;

	virtual void onRender() noexcept override;
	virtual void onImGui()  noexcept override;
	virtual void onEvent(overtime::event& event) override;

	virtual inline const glm::vec3& getPos() const override { return _pos; }
	virtual inline const glm::vec2& getSize() const override { return _size; }

	virtual inline void setPos(const glm::vec3& newPos) override { _pos = newPos; updateBounds(); }
	virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; updateBounds(); }

	virtual void activate() override { interactElement::activate(); updateBounds(); }

	ELEMENT_CLASS_TYPE(buttonElement)
private:

	bool onWindowResize(overtime::windowResizeEvent& event);
	bool onMouseMoved(overtime::mouseMovedEvent& event);
	bool onMouseButtonPressed(overtime::mouseButtonPressedEvent& event);
	bool onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event);
	void updateBounds();
	glm::vec3 _pos;
	glm::vec2 _size;
	glm::vec4 _bounds;
	std::array<std::string, (size_t)buttonState::stateCount> _keys;
	overtime::ref<style> _style;
	buttonState _currentState = buttonState::idle;
	std::function<bool(button*)> _funcOnPress;
	std::function<bool(button*)> _funcOnRelease;
};

#endif
