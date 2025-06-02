#include "maskLayer.h"
#include "cameraWrapper.h"

using namespace overtime;


maskCell::maskCell(const overtime::ref<themeManager::style> style)
	:_style(style)
{

}

maskCell::~maskCell()
{}

maskLayer::maskLayer(const std::string& name, uint32_t rowCount, uint32_t columnCount, const glm::vec3& startPosition, const glm::vec2& cellSize, const std::vector<std::string>& keys,
	const std::function<bool(maskLayer*)>& funcOnRelease)
	:interactElement(name), _rowCount(rowCount), _columnCount(columnCount), _pos(startPosition), _size(cellSize), _funcOnRelease(funcOnRelease)
{
	uint32_t count = rowCount * columnCount;
	_cells.reserve(count);
	//todo add checking for row * column counts > 0
	for (uint32_t i = 0; i < count; ++i) {
		_cells.emplace_back(maskCell(themeManager::getStyle(keys.at(0))));
	}
	updateBounds();
}

maskLayer::~maskLayer()
{}

void maskLayer::updateBounds()
{
	glm::vec2 first = _pos;
	glm::vec2 end = { _pos.x + (_columnCount - 1) * _size.x,_pos.y - (_rowCount - 1) * _size.y };

	first = { first.x - _size.x / 2.0f, first.y + _size.y / 2.0f };
	end = { end.x + _size.x / 2.0f,end.y - _size.y / 2.0f };

	_bounds = {
		cameraWrapper::worldToScreen({ first.x,first.y }),//UpLeft
		cameraWrapper::worldToScreen({ end.x, end.y }) //BottomRight
	};
}

bool maskLayer::onWindowResize(overtime::windowResizeEvent& event)
{
	updateBounds();	return false;
}

inline void maskLayer::onRender()
{
	if (_status & elementFlags::visible) {
		uint32_t x = 0, y = 0;
		for (auto& cell : _cells) {
			if (cell._isVisible)
				renderer2D::drawSquad({ _pos.x + x * _size.x, _pos.y - y * _size.y, _pos.z }, _size,
					cell._style->_color, cell._style->_texture, cell._style->_textureSize);
			++x %= _columnCount;
			if (x == 0) y++;
		}
	}
}

inline void maskLayer::onEvent(event& event)
{
	if (!(_status & elementFlags::blocked)) {
		eventDispatcher dispatcher(event);
		dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(maskLayer::onWindowResize));
		dispatcher.dispatch<mouseMovedEvent>(OT_BIND_EVENT_FN(maskLayer::onMouseMoved));
		dispatcher.dispatch<mouseButtonPressedEvent>(OT_BIND_EVENT_FN(maskLayer::onMouseButtonPressed));
		dispatcher.dispatch<mouseButtonReleasedEvent>(OT_BIND_EVENT_FN(maskLayer::onMouseButtonReleased));
	}
}
void maskLayer::setCellVisibility(const glm::i32vec2& begin, const glm::i32vec2& end, bool newVisibility)
{
	for (int i = begin.y; i <= end.y; i++)
		for (int j = begin.x; j <= end.x; j++)
			_cells.at(i * _rowCount + j)._isVisible = newVisibility;
}
bool maskLayer::onMouseMoved(overtime::mouseMovedEvent& event)
{
	glm::vec2 mousePos(event.getX(), event.getY());

	if (mousePos.x >= _bounds.x && mousePos.y >= _bounds.y &&
		mousePos.x <= _bounds.z && mousePos.y <= _bounds.w) {
		glm::vec2 worldClick = cameraWrapper::screenToWorld({ mousePos.x, mousePos.y });

		glm::vec2 clickPos = { worldClick.x - _pos.x, worldClick.y - _pos.y };

		clickPos = abs(round(clickPos / _size));
		_cellClicked = clickPos.y * _rowCount + clickPos.x;
		if (_cells[_cellClicked]._currentState == state::clicked) return true;
		_cells[_cellClicked]._currentState = state::hover;
		return true;
	}
	else if (_cells[_cellClicked]._currentState != state::idle)
		_cells[_cellClicked]._currentState = state::idle;
	return false;

	return false;
}

bool maskLayer::onMouseButtonPressed(overtime::mouseButtonPressedEvent& event)
{

	switch (event.getMouseButton()) {
		case mouseButtonCodes::OT_MOUSE_BUTTON_LEFT:
			if (_cells[_cellClicked]._currentState) {
				_cells[_cellClicked]._currentState = state::clicked;
			}
	}
	return false;
}

bool maskLayer::onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event)
{
	if (_cells[_cellClicked]._currentState != state::idle) {
		_cells[_cellClicked]._currentState = state::hover;
		return _funcOnRelease(this);
	}
	return false;
}