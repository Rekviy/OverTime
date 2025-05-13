#include "ship.h"
#include "cameraWrapper.h"
using namespace overtime;

ship::shipCell::shipCell(glm::vec3 position, const std::vector<std::string>& keys)
	:_pos(position), _keys(keys)
{
	_style = themeManager::getStyle(_keys.at(_currentState));
}

ship::shipCell::~shipCell()
{}

void ship::shipCell::changeState(state newState)
{
	_currentState = newState;
	_style = themeManager::getStyle(_keys.at(_currentState));
}

ship::ship(const std::string& name, const glm::vec3& position, uint32_t length, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving)
	:interactElement(name), _size(cellSize), _funcOnRelease(funcOnRelease), _funcOnMoving(funcOnMoving)
{
	switch (length) {
		case 1:
			_type = elementType::ship1;
			break;
		case 2:
			_type = elementType::ship2;
			break;
		case 3:
			_type = elementType::ship3;
			break;
		case 4:
			_type = elementType::ship4;
			break;
	}

	_cells.reserve(length);

	for (uint32_t i = 0, style = 0; i < _cells.capacity(); ++i, ++style) {
		style %= keys.size();
		_cells.emplace_back(shipCell({ position.x + _size.x * i,position.y,position.z }, keys.at(style)));
	}
	updateBounds();
	_cellClicked = (uint32_t)ceil(length / 2) - 1;
	_isDragging = true;
}

ship::~ship()
{}

void ship::onRender()
{
	if (_isVisible) {
		for (auto& item : _cells) {
			renderer2D::drawRotatedSquad(item._pos, _size, _rotation, item._style->_color, item._style->_texture, item._style->_textureSize);
		}
	}
}

void ship::onEvent(overtime::event& event)
{
	eventDispatcher dispatcher(event);
	dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(ship::onWindowResize));
	if (_isActive) {
		dispatcher.dispatch<mouseMovedEvent>(OT_BIND_EVENT_FN(ship::onMouseMoved));
		dispatcher.dispatch<mouseButtonPressedEvent>(OT_BIND_EVENT_FN(ship::onMouseButtonPressed));
		dispatcher.dispatch<mouseButtonReleasedEvent>(OT_BIND_EVENT_FN(ship::onMouseButtonReleased));
		dispatcher.dispatch<keyPressedEvent>(OT_BIND_EVENT_FN(ship::onKeyPressed));
	}
}

void ship::activate()
{
	_isVisible = _isActive = _isDragging = true;
}
	

void ship::deactivate()
{
	_isVisible = _isActive = _isDragging = false;
}

const glm::vec3& ship::getPos() const
{
	return (*_cells.begin())._pos;
}

void ship::setPos(const glm::vec3& newPos)
{
	auto& first = (*_cells.begin());
	first._pos = newPos;
	for (uint32_t i = 0, incX = 0, incY = 0; i < _cells.size(); i++) {
		_cells.at(i)._pos = { first._pos.x + _size.x * incX, first._pos.y - _size.x * incY, first._pos.z };
		!(bool)_rotation ? incX++ : incY++;
	}
	updateBounds();
}

void ship::changeState(shipCell::state newState)
{
	for (auto& item : _cells) {
		item.changeState(newState);
	}
}

void ship::changeState(uint32_t cell, shipCell::state newState)
{
	_cells.at(cell).changeState(newState);
}

bool ship::onWindowResize(overtime::windowResizeEvent& event)
{
	updateBounds();
	return false;
}

void ship::updateBounds()
{
	_bounds = {
		cameraWrapper::worldToScreen({ (*_cells.cbegin())._pos.x - _size.x / 2.0f, (*_cells.cbegin())._pos.y + _size.y / 2.0f }),//UpLeft
		cameraWrapper::worldToScreen({ (*(_cells.cend() - 1))._pos.x + _size.x / 2.0f, (*(_cells.cend() - 1))._pos.y - _size.y / 2.0f }) //BottomRight
	};
}
bool ship::onMouseMoved(overtime::mouseMovedEvent& event)
{
	if (_isDragging) {

		glm::vec3 temp = _cells.at(_cellClicked)._pos = { cameraWrapper::screenToWorld({ event.getX(),event.getY() }), _cells.at(_cellClicked)._pos.z };
		uint32_t ccX = 0, ccY = 0;
		!(bool)_rotation ? ccX = _cellClicked : ccY = _cellClicked;
		for (uint32_t i = 0, incX = 0, incY = 0; i < _cells.size(); i++) {
			if (i < _cellClicked) {
				_cells.at(i)._pos = { temp.x - _size.x * (ccX - incX), {temp.y + _size.y * (ccY - incY)},temp.z };
			}
			else if (i > _cellClicked) {
				_cells.at(i)._pos = { temp.x + _size.x * (incX - ccX), {temp.y - _size.y * (incY - ccY)},temp.z };
			}
			!(bool)_rotation ? incX++ : incY++;
		}
		return _funcOnMoving(this);
	}

	return false;
}

bool ship::onMouseButtonPressed(overtime::mouseButtonPressedEvent& event)
{
	auto [mouseX, mouseY] = input::getMousePos();

	if (mouseX >= _bounds.x && mouseY >= _bounds.y &&
		mouseX <= _bounds.z && mouseY <= _bounds.w) {

		glm::vec2 clickPos = { mouseX - _bounds.x, mouseY - _bounds.y };

		clickPos = floor(clickPos / _size);
		_cellClicked = (uint32_t)std::max(clickPos.x, clickPos.y);
		//_clickOffset = { _cells.at(_cellClicked)._pos.x - mousePos.x,_cells.at(_cellClicked)._pos.y - mousePos.y };
		_isDragging = true;
		return true;
	}
	return false;
}

bool ship::onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event)
{
	if (_isDragging) {
		_isDragging = false;
		updateBounds();
		return _funcOnRelease(this);
	}
	return false;
}

bool ship::onKeyPressed(overtime::keyPressedEvent& event)
{
	if (_isDragging) {
		switch (event.getKeyCode()) {
			case keyCodes::OT_KEY_R:
				_rotation = (bool)_rotation ? 0.0f : glm::radians(90.0f);
				glm::vec3 temp = _cells.at(_cellClicked)._pos;
				uint32_t ccX = 0, ccY = 0;
				!(bool)_rotation ? ccX = _cellClicked : ccY = _cellClicked;
				for (uint32_t i = 0, incX = 0, incY = 0; i < _cells.size(); i++) {
					if (i < _cellClicked) {
						_cells.at(i)._pos = { temp.x - _size.x * (ccX - incX), {temp.y + _size.y * (ccY - incY)},temp.z };
					}
					else if (i > _cellClicked) {
						_cells.at(i)._pos = { temp.x + _size.x * (incX - ccX), {temp.y - _size.y * (incY - ccY)},temp.z };
					}
					!(bool)_rotation ? incX++ : incY++;
				}
				break;
		}
		return true;
	}
	return false;
}


