#include "counter.h"

using namespace overtime;

counter::counter(const std::string& name, const glm::vec3& position, const glm::vec2& size, uint32_t startNumber, uint32_t endNumber, const std::vector<std::string>& keys)
	:interactElement(name), _pos(position),_size(size),_start(startNumber), _end(endNumber), _keys(keys)
{
	if (_end < _start) std::swap(_end, _start);
	_count = _start;
	updateStyle();
}


counter::~counter()
{}

void counter::onRender()
{
	if(_isVisible)
		renderer2D::drawSquad(_pos, _size, _style->_color, _style->_texture, _style->_textureSize);
}

void counter::onEvent(overtime::event & event)
{}
//todo sync count to styles
void counter::updateStyle()
{
	_style = themeManager::getStyle(_keys.at(_count));
}
