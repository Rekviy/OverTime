#include "button.h"
#include "gameLayer.h"
#include "cameraWrapper.h"

using namespace overtime;


button::button(const std::string& name, const glm::vec3& position, const glm::vec2& size, const std::vector<std::string>& keys, bool isActive,
	const std::function<bool(button*)>& funcOnRelease, const std::function<bool(button*)>& funcOnPress)
	:interactElement(name), _pos(position), _size(size), _keys(keys), _funcOnRelease(funcOnRelease), _funcOnPress(funcOnPress)
{
	_currentState = state::idle;
	_style = themeManager::getStyle(_keys.at(_currentState));
	updateBounds();
	if (isActive)
		activate();
}

button::~button()
{}

void button::updateBounds()
{
	_bounds = {
		cameraWrapper::worldToScreen({ _pos.x - _size.x / 2.0f, _pos.y + _size.y / 2.0f }),//UpLeft
		cameraWrapper::worldToScreen({ _pos.x + _size.x / 2.0f, _pos.y - _size.y / 2.0f }) //BottomRight
	};
}

void button::onRender()
{
	if (_isVisible)
		renderer2D::drawSquad(_pos, _size, _style->_color, _style->_texture, _style->_textureSize);
}

void button::onEvent(overtime::event& event)
{
	eventDispatcher dispatcher(event);
	dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(button::onWindowResize));
	if (_isActive) {
		dispatcher.dispatch<mouseMovedEvent>(OT_BIND_EVENT_FN(button::onMouseMoved));
		dispatcher.dispatch<mouseButtonPressedEvent>(OT_BIND_EVENT_FN(button::onMouseButtonPressed));
		dispatcher.dispatch<mouseButtonReleasedEvent>(OT_BIND_EVENT_FN(button::onMouseButtonReleased));
		_style = themeManager::getStyle(_keys.at(_currentState));
	}
}

bool button::onWindowResize(overtime::windowResizeEvent& event)
{
	updateBounds();
	return false;
}

bool button::onMouseMoved(mouseMovedEvent& event)
{
	glm::vec2 mousePos(event.getX(), event.getY());
	//OT_INFO("Mouse: {0}, {1}", mousePos.x, mousePos.y);
	//OT_INFO("Button: {0}, {1}, {2}, {3}", _bounds.x, _bounds.y, _bounds.z, _bounds.w);
	//OT_WARN("State={0}", _currentState);
	if (mousePos.x >= _bounds.x && mousePos.y >= _bounds.y &&
		mousePos.x <= _bounds.z && mousePos.y <= _bounds.w) {
		if (_currentState == state::clicked) return true;
		_currentState = state::hover;
		return true;
	}
	else if (_currentState != state::idle)
		_currentState = state::idle;
	return false;
}

bool button::onMouseButtonPressed(overtime::mouseButtonPressedEvent& event)
{
	switch (event.getMouseButton()) {
		case mouseButtonCodes::OT_MOUSE_BUTTON_LEFT:
			if (_currentState) {
				_currentState = state::clicked;
				return _funcOnPress(this);
			}
	}
	return false;
}
bool button::onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event)
{
	if (_currentState != state::idle) {
		_currentState = state::hover;
		return _funcOnRelease(this);
	}
	return false;
}
