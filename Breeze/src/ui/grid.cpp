#include "grid.h"

using namespace overtime;

grid::gridCell::gridCell(const std::vector<std::string>& keys)
	:_keys(keys)
{
	_style = themeManager::getStyle(_keys.at(_currentState));
}

grid::gridCell::~gridCell()
{}

void grid::gridCell::changeState(state newState)
{
	_currentState = newState;
	_style = themeManager::getStyle(_keys.at(_currentState));
}

grid::grid(const std::string& name, uint32_t rowCount, uint32_t columnCount, const glm::vec3& startPosition, const glm::vec2& gridCellSize, const std::vector<std::vector<std::string>>& keys)
	:interactElement(name), _rowCount(rowCount), _columnCount(columnCount), _pos(startPosition), _size(gridCellSize)
{
	uint32_t count = rowCount * columnCount;
	_storage.reserve(count);
	//todo add checking for row * column counts > 0
	for (uint32_t i = 0, style = 0; i < count; ++i, ++style) {
		style %= keys.size();
		_storage.emplace_back(gridCell(keys.at(style)));
	}
	deactivate();
}

grid::~grid()
{}

void grid::onRender()
{
	if (_status & elementFlags::visible) {
		uint32_t x = 0, y = 0;

		for (auto& item : _storage) {
			renderer2D::drawSquad({ _pos.x + x * _size.x, _pos.y - y * _size.y, _pos.z }, _size,
				item._style->_color, item._style->_texture, item._style->_textureSize);

			++x %= _columnCount;
			if (x == 0) y++;
		}
	}
}



void grid::setOccupation(const std::vector<gridCell>::iterator& begin, const std::vector<gridCell>::iterator& end, bool newOccupation)
{
	while (begin != end)
		begin->_isOccupied = newOccupation;

}

void grid::setOccupation(const glm::i32vec2& begin, const glm::i32vec2& end, bool newOccupation)
{
	for (int i = begin.y; i <= end.y; i++)
		for (int j = begin.x; j <= end.x; j++)
			_storage.at(i * _rowCount + j)._isOccupied = newOccupation;
}

bool grid::isOccupied(const std::vector<gridCell>::iterator& begin, const std::vector<gridCell>::iterator& end)
{
	while (begin != end) {
		if (begin->_isOccupied)
			return true;
	}
	return false;
}

bool grid::isOccupied(const glm::i32vec2& begin, const glm::i32vec2& end)
{
	for (int i = begin.y; i <= end.y; i++) {
		for (int j = begin.x; j <= end.x; j++) {
			if (_storage.at(i * _rowCount + j)._isOccupied)
				return true;
		}
	}
	return false;
}

bool grid::addPlacement(uint32_t itemId, const glm::i32vec2& begin, const glm::i32vec2& end)
{
	if (!isOccupied(begin, end)) {
		_placings[itemId] = { begin,end };
		setOccupation(begin, end, true);
		return true;
	}
	return false;
}

void grid::removePlacement(uint32_t itemId)
{
	auto it = _placings.find(itemId);
	if (it != _placings.end()) {
		setOccupation(it->second.first, it->second.second, false);
		_placings.erase(it);
	}
}

const std::pair<glm::i32vec2, glm::i32vec2>& grid::getPlacement(uint32_t itemId) const
{
	auto& item = _tempPlacings.find(itemId);
	if (item == _tempPlacings.end())
		item = _placings.find(itemId);

	return item->second;
}

uint32_t grid::getItemAt(const glm::i32vec2& position) const
{
	for (const auto& [shipId, placement] : _placings) {
		if (position.x >= placement.first.x && position.x <= placement.second.x &&
			position.y >= placement.first.y && position.y <= placement.second.y) {
			return shipId;
		}
	}
	return -1;
}

bool grid::addTempPlacement(uint32_t itemId, const glm::i32vec2& begin, const glm::i32vec2& end)
{
	if (!isOccupied(begin, end)) {
		_tempPlacings[itemId] = { begin,end };
		return true;
	}
	return false;
}

void grid::removeTempPlacement(uint32_t itemId)
{
	auto it = _tempPlacings.find(itemId);
	if (it != _tempPlacings.end())
		_tempPlacings.erase(it);

}

bool grid::acceptPlacing(uint32_t itemId)
{
	auto it = _tempPlacings.find(itemId);
	if (it != _tempPlacings.end()) {
		if (addPlacement(it->first, it->second.first, it->second.second))
			_tempPlacings.erase(itemId);
	}
	return false;
}

void grid::rejectPlacing(uint32_t itemId)
{
	auto it = _tempPlacings.find(itemId);
	if (it != _tempPlacings.end()) {
		_tempPlacings.erase(itemId);
	}
}

void grid::changeState(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end, gridCell::state newState)
{
	while (begin != end) {
		begin->changeState(newState);
	};
}

void grid::changeState(const glm::i32vec2& begin, const glm::i32vec2& end, gridCell::state newState)
{
	for (int i = begin.y; i < end.y; i++) {
		for (int j = begin.x; j < end.x; j++) {
			_storage[i * _rowCount + j].changeState(newState);
		}
	}
}

void grid::changeState(const glm::i32vec2& cellPos, gridCell::state newState)
{
	_storage[cellPos.y * _rowCount + cellPos.x].changeState(newState);
}

void grid::changeState(uint32_t cellIndex, gridCell::state newState)
{
	_storage[cellIndex].changeState(newState);
}

void grid::onEvent(overtime::event& event)
{
	if (!(_status & elementFlags::blocked)) {
		eventDispatcher dispatcher(event);

	}
}

bool grid::onWindowResize(overtime::windowResizeEvent& event)
{
	return false;
}

bool grid::onMouseMoved(overtime::mouseMovedEvent& event)
{
	return false;
}

bool grid::onMouseButtonPressed(overtime::mouseButtonPressedEvent& event)
{
	return false;
}

bool grid::onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event)
{
	return false;
}
