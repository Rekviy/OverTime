#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include "interactElement.h"
#include "themeManager.h"

#include <initializer_list>

class button : public interactElement {
public:
	button(const glm::vec3& position, const glm::vec2& size, const std::initializer_list<std::string>& keys,
		const std::function<bool(void)>& funcOnRelease, const std::function<bool(void)>& funcOnPress = []() {return true; });
	virtual ~button() override;
	virtual void onRender() override;
	virtual inline const glm::vec3& getPos() const override { return _pos; }
	virtual inline const glm::vec2& getSize() const override { return _size; }
	virtual inline void setPos(const glm::vec3& newPos) override { _pos = newPos; updateBounds();}
	virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; updateBounds();}
	virtual void onEvent(overtime::event& event) override;
	bool onWindowResize(overtime::windowResizeEvent& event);
	bool onMouseMoved(overtime::mouseMovedEvent& event);
	bool onMouseButtonPressed(overtime::mouseButtonPressedEvent& event);
	bool onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event);
	enum state {
		idle = 0, hover = 1, clicked = 2
	};
private:
	void updateBounds();

	glm::vec3 _pos;
	glm::vec2 _size;
	glm::vec4 _bounds;
	std::vector<std::string> _keys;
	overtime::ref<themeManager::style> _style;
	state _currentState = state::idle;
	std::function<bool(void)> _funcOnPress;
	std::function<bool(void)> _funcOnRelease;
};

#endif
