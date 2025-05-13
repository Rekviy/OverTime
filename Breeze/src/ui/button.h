#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include "interactElement.h"
#include "themeManager.h"

#include <initializer_list>

class button : public interactElement {
public:
	button(const std::string& name, const glm::vec3& position, const glm::vec2& size, const std::vector<std::string>& keys, bool isActive,
		const std::function<bool(button*)>& funcOnRelease, const std::function<bool(button*)>& funcOnPress = [](button* btn) {return true; });
	
	virtual ~button() override;
	virtual void onRender() override;
	virtual inline const glm::vec3& getPos() const override { return _pos; }
	virtual inline const glm::vec2& getSize() const override { return _size; }
	virtual inline void setPos(const glm::vec3& newPos) override { _pos = newPos; updateBounds(); }
	virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; updateBounds(); }
	virtual inline bool isActive() const override { return _isActive; }
	virtual void activate()  override { _isVisible = true; _isActive = true; }
	virtual void deactivate() override { _isVisible = false; _isActive =false; }
	virtual inline bool isVisible() const override { return _isVisible; }
	virtual inline void setVisibility(bool newVisibility) override { _isVisible = newVisibility; }
	virtual void onEvent(overtime::event& event) override;
	virtual inline elementType getType() override { return _type; }
	enum state {
		idle = 0, hover, clicked
	};
private:
	
	bool onWindowResize(overtime::windowResizeEvent& event);
	bool onMouseMoved(overtime::mouseMovedEvent& event);
	bool onMouseButtonPressed(overtime::mouseButtonPressedEvent& event);
	bool onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event);
	void updateBounds();
	elementType _type = elementType::button;
	bool _isActive = true;
	bool _isVisible = true;
	glm::vec3 _pos;
	glm::vec2 _size;
	glm::vec4 _bounds;
	std::vector<std::string> _keys;
	overtime::ref<themeManager::style> _style;
	state _currentState = state::idle;
	std::function<bool(button*)> _funcOnPress;
	std::function<bool(button*)> _funcOnRelease;
};

#endif
