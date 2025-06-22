#include "counter.h"
#include <glm/gtc/type_ptr.hpp>

using namespace overtime;

counter::counter(const std::string& name, const glm::vec3& position, const glm::vec2& size, uint32_t startNumber, uint32_t endNumber, std::vector<std::string>&& keys, bool isActive)
	:interactElement(name), _pos(position), _size(size), _start(startNumber), _end(endNumber), _keys(keys)
{
	if (_end < _start) std::swap(_end, _start);

	if (_keys.size() < _end - _start)
		throw std::invalid_argument("Counter keys count cannot be lesser than counter range!");
	if (!themeManager::isKeysValid(_keys))
		throw std::invalid_argument("Style keys not valid! Name: " + _name);

	_keys.resize(_end - _start + 1);
	_count = _start;
	_offset = abs((long)(0 - _start));
	updateStyle();
	if (isActive)
		activate();
}

void counter::reset()
{
	_count = _start;
	updateStyle();
}

void counter::onRender() noexcept
{
	if (_status & elementFlags::visible)
		renderer2D::drawSquad(_pos, _size, _style->_color, _style->_texture, _style->_textureSize);
}

void counter::onImGui() noexcept
{
	ImGui::Text("Start=%d End=%d Offset=%d", _start, _end, _offset);
	ImGui::NewLine();

	ImGui::SliderInt("Count", (int*)&_count, _start, _end, _keys.at(_count - _offset).c_str());
	ImGui::NewLine();

	if (ImGui::Button("UpdateStyle"))
		_style = themeManager::getStyle(_keys[_count]);
	ImGui::NewLine();

	ImGui::Text("Position: x=%f, y=%f, z=%f", _pos.x, _pos.y, _pos.z);
	ImGui::InputFloat3("Position", glm::value_ptr(_pos));
	ImGui::NewLine();

	ImGui::Text("Size: x=%f, y=%f", _size.x, _size.y);
	ImGui::InputFloat2("Size", glm::value_ptr(_size));
	ImGui::NewLine();
}

void counter::updateStyle()
{
	_style = themeManager::getStyle(_keys[_count - _offset]);
}