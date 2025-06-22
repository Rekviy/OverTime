#include "button.h"
#include "gameLayer.h"
#include "cameraWrapper.h"

#include <glm/gtc/type_ptr.hpp>

using namespace overtime;


button::button(const std::string& name, const glm::vec3& position, const glm::vec2& size, std::array<std::string, (size_t)buttonState::stateCount>&& keys,
	std::function<bool(button*)>&& funcOnRelease, std::function<bool(button*)>&& funcOnPress, bool isActive)
	:interactElement(name), _pos(position), _size(size), _keys(keys), _funcOnRelease(funcOnRelease), _funcOnPress(funcOnPress)
{
	if (_keys.empty()) throw std::invalid_argument("Button keys cannot be empty! Name: " + _name);

	if(!themeManager::isKeysValid(std::vector<std::string>(_keys.cbegin(), _keys.cend())))
		throw std::invalid_argument("Style keys not valid! Name: " + _name);

	_currentState = buttonState::idle;
	_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
	updateBounds();
	if (isActive)
		activate();
}

void button::reset()
{
	_currentState = buttonState::idle;
	_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
}

void button::updateBounds()
{
	_bounds = {
		cameraWrapper::worldToScreen({ _pos.x - _size.x / 2.0f, _pos.y + _size.y / 2.0f }),//UpLeft
		cameraWrapper::worldToScreen({ _pos.x + _size.x / 2.0f, _pos.y - _size.y / 2.0f }) //BottomRight
	};
}

void button::onRender() noexcept
{
	if (_status & elementFlags::visible)
		renderer2D::drawSquad(_pos, _size, _style->_color, _style->_texture, _style->_textureSize);
}

void button::onImGui() noexcept
{
	const char* elems_names[(uint32_t)buttonState::stateCount] = { "idle", "hover", "clicked" };
	const char* elem_name = elems_names[(uint32_t)_currentState];
	ImGui::SliderInt("Slider enum", (int*)&_currentState, 0, (int)buttonState::stateCount - 1, elem_name);

	if (ImGui::Button("UpdateStyle"))
		_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
	ImGui::NewLine();

	ImGui::SliderInt("Slider style", (int*)&_currentState, 0, (int)buttonState::stateCount - 1, _keys[(uint32_t)_currentState].c_str());
	ImGui::NewLine();

	ImGui::Text("Position: x=%f, y=%f, z=%f", _pos.x, _pos.y, _pos.z);
	ImGui::InputFloat3("Position", glm::value_ptr(_pos));
	ImGui::NewLine();

	ImGui::Text("Size: x=%f, y=%f", _size.x, _size.y);
	ImGui::InputFloat2("Size", glm::value_ptr(_size));
	ImGui::NewLine();

	ImGui::Text("Bounds: left=%f, up=%f, right=%f, bottom=%f", _bounds.x, _bounds.y, _bounds.z, _bounds.w);
	if (ImGui::Button("UpdateBounds"))
		updateBounds();
	ImGui::NewLine();

	if (ImGui::Button("Call function on press"))
		_funcOnPress(this);
	ImGui::NewLine();

	if (ImGui::Button("Call function on release"))
		_funcOnRelease(this);
}

void button::onEvent(overtime::event& event)
{
	if (!(_status & elementFlags::blocked) && (_status & elementFlags::active)) {
		eventDispatcher dispatcher(event);
		dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(button::onWindowResize));
		dispatcher.dispatch<mouseMovedEvent>(OT_BIND_EVENT_FN(button::onMouseMoved));
		dispatcher.dispatch<mouseButtonPressedEvent>(OT_BIND_EVENT_FN(button::onMouseButtonPressed));
		dispatcher.dispatch<mouseButtonReleasedEvent>(OT_BIND_EVENT_FN(button::onMouseButtonReleased));
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

	if (mousePos.x > _bounds.x && mousePos.y > _bounds.y &&
		mousePos.x < _bounds.z && mousePos.y < _bounds.w) {

		if (_currentState == buttonState::idle) {
			_currentState = buttonState::hover;
			_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
		}
		return true;
	}
	else if (_currentState != buttonState::idle) {
		_currentState = buttonState::idle;
		_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
	}
	return false;
}

bool button::onMouseButtonPressed(overtime::mouseButtonPressedEvent& event)
{
	switch (event.getMouseButton()) {
		case mouseButtonCodes::OT_MOUSE_BUTTON_LEFT:
			if (_currentState != buttonState::idle) {
				_currentState = buttonState::clicked;
				_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
				return _funcOnPress(this);
			}
	}
	return false;
}
bool button::onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event)
{
	switch (event.getMouseButton()) {
		case mouseButtonCodes::OT_MOUSE_BUTTON_LEFT:
			if (_currentState != buttonState::idle) {
				_currentState = buttonState::hover;
				_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
				return _funcOnRelease(this);
			}
	}
	return false;
}