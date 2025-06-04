#include "ship.h"
#include "cameraWrapper.h"

#include <glm/gtc/constants.hpp>

using namespace overtime;

//shipCell
#pragma region shipCell
ship::shipCell::shipCell(glm::vec3 position, const std::vector<std::string>& keys)
	:_pos(position), _keys(keys)
{
	_style = themeManager::getStyle(_keys.at(_currentState));
}

ship::shipCell::~shipCell()
{}

void ship::shipCell::changeState(shipCellState newState)
{
	_currentState = newState;
	_style = themeManager::getStyle(_keys.at(_currentState));
}
#pragma endregion

//ship
ship::ship(const std::string& name, const glm::vec3& position, uint32_t length, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving)
	:interactElement(name), _size(cellSize), _funcOnPress(funcOnPress), _funcOnRelease(funcOnRelease), _funcOnMoving(funcOnMoving)
{
	_cells.reserve(length);

	for (uint32_t i = 0, style = 0; i < length; ++i, ++style) {
		style %= keys.size();
		_cells.emplace_back(shipCell({ position.x + _size.x * i,position.y,position.z }, keys.at(style)));
	}
	updateBounds();
	_cellClicked = (uint32_t)ceil((float)length / 2.0f) - 1;
	deactivate();
}

ship::~ship()
{}

void ship::reset()
{
	changeState(shipCellState::normal);
	changeShipState(shipState::normal);
}

void ship::onRender()
{
	if (_status & elementFlags::visible) {
		for (auto& item : _cells) {
			renderer2D::drawRotatedSquad(item._pos, _size, _angle, item._style->_color, item._style->_texture, item._style->_textureSize);
		}
	}
}

void ship::onEvent(overtime::event& event)
{
	if (!(_status & elementFlags::blocked)) {
		eventDispatcher dispatcher(event);
		dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(ship::onWindowResize));
		dispatcher.dispatch<mouseMovedEvent>(OT_BIND_EVENT_FN(ship::onMouseMoved));
		dispatcher.dispatch<mouseButtonPressedEvent>(OT_BIND_EVENT_FN(ship::onMouseButtonPressed));
		dispatcher.dispatch<mouseButtonReleasedEvent>(OT_BIND_EVENT_FN(ship::onMouseButtonReleased));
		dispatcher.dispatch<keyPressedEvent>(OT_BIND_EVENT_FN(ship::onKeyPressed));
	}
}

void ship::activate()
{
	interactElement::activate();
	_currentState = shipState::dragging;
	updateBounds();
}


void ship::deactivate()
{
	interactElement::deactivate();
	_currentState = shipState::normal;
}

const glm::vec3& ship::getPos() const
{
	return _cells.begin()->_pos;
}

void ship::setPos(const glm::vec3& newPos)
{
	calculateNewPos(0, newPos);

	updateBounds();
}

shipCellState ship::getState() const
{
	int32_t temp = 0;
	for (auto& item : _cells)
		temp |= item.getState();
	return (shipCellState)temp;
}
shipCellState ship::getState(uint32_t cell) const
{
	return _cells.at(cell).getState();
}

void ship::changeState(shipCellState newState)
{
	for (auto& item : _cells) {
		item.changeState(newState);
	}
}
void ship::changeState(uint32_t cell, shipCellState newState)
{
	_cells.at(cell).changeState(newState);
	if (_currentState == shipState::normal) {
		uint32_t tempState = shipCellState::shot;
		for (auto& cell : _cells)
			tempState &= cell.getState();
		if (tempState == shipCellState::shot)
			changeShipState(shipState::destroyed);
	}
}

void ship::changeShipState(shipState newState)
{
	_currentState = newState;
}

void ship::updateBounds()
{
	glm::vec2 first = (*_cells.cbegin())._pos, end = (_cells.cend() - 1)->_pos;
	if (end.x < first.x ) std::swap(first.x, end.x);
	if ( end.y > first.y) std::swap(first.y, end.y);


	first = { first.x - _size.x / 2.0f, first.y + _size.y / 2.0f };
	end = { end.x + _size.x / 2.0f,end.y - _size.y / 2.0f };

	_bounds = {
		cameraWrapper::worldToScreen({ first.x,first.y }),//UpLeft
		cameraWrapper::worldToScreen({ end.x, end.y }) //BottomRight
	};
}
void ship::calculateNewPos(uint32_t cellClicked, const glm::vec3& newPos)
{
	glm::vec3 basePos = _cells[cellClicked]._pos = newPos;
	float c = glm::cos(_angle), s = glm::sin(_angle);
	for (uint32_t i = 0; i < _cells.size(); ++i) {
		int delta = (int)i - (int)cellClicked;

		glm::vec3 newPos = basePos;

		newPos.y -= (float)delta * _size.y * s; newPos.x += (float)delta * _size.x * c;
		_cells[i]._pos = newPos;
	}
}

bool ship::onWindowResize(overtime::windowResizeEvent& event)
{
	updateBounds();
	return false;
}
bool ship::onMouseMoved(overtime::mouseMovedEvent& event)
{
	if (_currentState == shipState::dragging) {
		calculateNewPos(_cellClicked, { cameraWrapper::screenToWorld({ event.getX(),event.getY() }), _cells[_cellClicked]._pos.z });

		return _funcOnMoving(this);
	}

	return false;
}

bool ship::onMouseButtonPressed(overtime::mouseButtonPressedEvent& event)
{
	auto [mouseX, mouseY] = input::getMousePos();

	if (mouseX >= _bounds.x && mouseY >= _bounds.y &&
		mouseX <= _bounds.z && mouseY <= _bounds.w) {

		glm::vec2 worldClick = cameraWrapper::screenToWorld({ mouseX, mouseY });
		const glm::vec3& pivotPoint = (_cells.begin())->_pos;
		float c = cos(_angle), s = -sin(_angle);

		glm::vec2 delta = { worldClick.x - pivotPoint.x, worldClick.y - pivotPoint.y };
		glm::vec2 rotatedClickPos = { delta.x * c - delta.y * s,delta.x * s + delta.y * c };

		delta = abs(round(rotatedClickPos / _size));
		_cellClicked = (uint32_t)delta.x;
		//_clickOffset = { _cells.at(_cellClicked)._pos.x - mousePos.x,_cells.at(_cellClicked)._pos.y - mousePos.y };
		_currentState = shipState::dragging;
		return _funcOnPress(this);
	}
	return false;
}

bool ship::onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event)
{
	if (_currentState == shipState::dragging) {
		_currentState = shipState::normal;
		updateBounds();
		return _funcOnRelease(this);
	}
	return false;
}

bool ship::onKeyPressed(overtime::keyPressedEvent& event)
{
	if (_currentState == shipState::dragging) {
		switch (event.getKeyCode()) {
			case keyCodes::OT_KEY_R:
				_angle += glm::radians(90.0f);
				if (_angle >= glm::two_pi<float>()) _angle -= glm::two_pi<float>();

				calculateNewPos(_cellClicked, _cells[_cellClicked]._pos);
		}
		return true;
	}
	return false;
}

ship1::ship1(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving)
	:ship(name, position, 1, cellSize, keys, funcOnPress, funcOnRelease, funcOnMoving)
{}

ship2::ship2(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving)
	:ship(name, position, 2, cellSize, keys, funcOnPress, funcOnRelease, funcOnMoving)
{}

ship3::ship3(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving)
	:ship(name, position, 3, cellSize, keys, funcOnPress, funcOnRelease, funcOnMoving)
{}

ship4::ship4(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving)
	:ship(name, position, 4, cellSize, keys, funcOnPress, funcOnRelease, funcOnMoving)
{}
