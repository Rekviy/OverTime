#include "grid.h"
#include <glm/gtc/type_ptr.hpp>
using namespace overtime;

grid::gridCell::gridCell(const std::array<std::string, (uint32_t)gridCellState::stateCount>& keys)
	:_keys(keys)
{
	_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
}


void grid::gridCell::changeState(gridCellState newState)
{
	_currentState = newState;
	_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
}

grid::grid(const std::string& name, uint32_t rowCount, uint32_t columnCount, const glm::vec3& startPosition, const glm::vec2& gridCellSize,
	const std::vector<std::array<std::string, (uint32_t)gridCellState::stateCount>>& keys, bool isActive)
	:interactElement(name), _rowCount(rowCount), _columnCount(columnCount), _pos(startPosition), _size(gridCellSize)
{
	uint32_t count = rowCount * columnCount;
	if (count < 0) throw std::invalid_argument("Grid cells count cannot be lesser than 0!");
	if (keys.empty()) throw std::invalid_argument("Grid keys cannot be empty!");
	if (keys.front().size() < (uint32_t)gridCellState::stateCount) throw std::invalid_argument("Grid cell keys cannot be lesser than gridCellState::stateCount!");

	_storage.reserve(count);

	for (uint32_t i = 0, style = 0; i < count; ++i, ++style) {
		style %= keys.size();
		_storage.emplace_back(gridCell(keys[style]));
	}
	if (isActive)
		activate();
}


void grid::reset()
{
	changeState(_storage.begin(), _storage.end(), gridCellState::normal);
	while (!_placings.empty())
		removePlacement(_placings.begin()->first);
	while (!_tempPlacings.empty())
		rejectPlacing(_tempPlacings.begin()->first);

}

void grid::onRender() noexcept
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

void grid::onImGui() noexcept
{
	ImGui::Text("Columns:%d rows:%d", _columnCount, _rowCount);
	ImGui::NewLine();

	ImGui::Text("Position: x=%f, y=%f, z=%f", _pos.x, _pos.y, _pos.z);
	ImGui::InputFloat3("Position", glm::value_ptr(_pos));
	ImGui::NewLine();

	ImGui::Text("Cell size: x=%f, y=%f", _size.x, _size.y);
	ImGui::InputFloat2("Cell size", glm::value_ptr(_size));
	ImGui::NewLine();

	//based on assetsBrowser from imgui_demo 
	int items = _storage.size();
	static bool stretchSpacing = true;
	static int iconSpacing = 0;
	static float layoutOuterPadding = 0.0f, layoutItemSpacing = 0.0f, iconSize = 32.0f;
	static ImVec2 layoutItemSize(_size.x, _size.y);
	static ImVec2 layoutItemStep(layoutItemSize.x + layoutItemSpacing, layoutItemSize.y + layoutItemSpacing);

	// Rendering parameters
	const ImU32 im_colors[2] = { IM_COL32(50, 50, 170, 255), IM_COL32(50, 170, 50, 255) };
	const ImU32 im_bg_color[2] = { IM_COL32(35, 35, 35, 220), IM_COL32(200, 70, 70, 255) };
	const ImVec2 icon_type_overlay_size = ImVec2(4.0f, 4.0f);
	const bool display_label = (layoutItemSize.x >= ImGui::CalcTextSize("999").x);
	static ImGuiSelectionBasicStorage selection;

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("Options")) {
			ImGui::PushItemWidth(ImGui::GetFontSize() * 10);
			ImGui::SeparatorText("Layout");
			ImGui::Checkbox("Stretch Spacing", &stretchSpacing);
			ImGui::SliderFloat("Icon Size", &iconSize, 16.0f, 128.0f, "%.0f");
			ImGui::SliderInt("Icon Spacing", &iconSpacing, 0, 32);
			ImGui::PopItemWidth();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowContentSize(ImVec2(0.0f, layoutOuterPadding + _rowCount * (layoutItemSize.y + layoutItemSpacing)));
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float avail_width = ImGui::GetContentRegionAvail().x;

	layoutItemSpacing = (float)iconSpacing;

	layoutItemSize = ImVec2(floorf(iconSize), floorf(iconSize));

	if (stretchSpacing && _columnCount > 1)
		layoutItemSpacing = floorf(avail_width - layoutItemSize.x * _columnCount) / _columnCount;

	layoutItemStep = ImVec2(layoutItemSize.x + layoutItemSpacing, layoutItemSize.y + layoutItemSpacing);
	float LayoutSelectableSpacing = std::max(floorf(layoutItemSpacing), 0.0f);
	layoutOuterPadding = floorf(layoutItemSpacing * 0.5f);

	// Calculate and store start position.
	ImVec2 start_pos = ImGui::GetCursorScreenPos();
	start_pos = ImVec2(start_pos.x + layoutOuterPadding, start_pos.y + layoutOuterPadding);
	ImGui::SetCursorScreenPos(start_pos);

	// Multi-select
	ImGuiMultiSelectFlags ms_flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_ClearOnClickVoid | ImGuiMultiSelectFlags_BoxSelect2d;
	ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(ms_flags, selection.Size, items);

	selection.UserData = this;
	selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self_, int idx) { return (ImGuiID)idx; };
	selection.ApplyRequests(ms_io);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(LayoutSelectableSpacing, LayoutSelectableSpacing));

	ImGuiListClipper clipper;
	clipper.Begin(_rowCount, layoutItemStep.y);

	if (ms_io->RangeSrcItem != -1)
		clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem / _columnCount); // Ensure RangeSrc item line is not clipped.

	while (clipper.Step()) {
		for (int line_idx = clipper.DisplayStart; line_idx < clipper.DisplayEnd; line_idx++) {
			const int item_min_idx_for_current_line = line_idx * _columnCount;
			const int item_max_idx_for_current_line = std::min((line_idx + 1) * (int)_columnCount, items);
			for (int item_idx = item_min_idx_for_current_line; item_idx < item_max_idx_for_current_line; ++item_idx) {
				gridCell* item_data = &_storage[item_idx];
				ImGui::PushID(item_idx);

				ImVec2 pos = ImVec2(start_pos.x + (item_idx % _columnCount) * layoutItemStep.x, start_pos.y + line_idx * layoutItemStep.y);
				ImGui::SetCursorScreenPos(pos);

				ImGui::SetNextItemSelectionUserData(item_idx);
				bool item_is_selected = selection.Contains((ImGuiID)item_idx);
				bool item_is_visible = ImGui::IsRectVisible(layoutItemSize);
				ImGui::Selectable("", item_is_selected, ImGuiSelectableFlags_None, layoutItemSize);

				if (ImGui::IsItemToggledSelection())
					item_is_selected = !item_is_selected;

				if (item_is_visible) {
					ImVec2 box_min(pos.x - 1, pos.y - 1);
					ImVec2 box_max(box_min.x + layoutItemSize.x + 2, box_min.y + layoutItemSize.y + 2); // Dubious

					// Background color
					draw_list->AddRectFilled(box_min, box_max, im_bg_color[item_data->_isOccupied % IM_ARRAYSIZE(im_bg_color)]);

					if (getItemAt({ item_idx % _rowCount, item_idx / _rowCount }) != UINT32_MAX) {
						ImU32 type_col = im_colors[IM_ARRAYSIZE(im_colors) - 1];
						draw_list->AddRectFilled(ImVec2(box_max.x - 2 - icon_type_overlay_size.x, box_min.y + 2), ImVec2(box_max.x - 2, box_min.y + 2 + icon_type_overlay_size.y), type_col);
					}
					if (getTempItemAt({ item_idx % _rowCount, item_idx / _rowCount }) != UINT32_MAX) {
						ImU32 type_col = im_colors[IM_ARRAYSIZE(im_colors) - 2];
						draw_list->AddRectFilled(ImVec2(box_max.x + 2 + icon_type_overlay_size.x, box_min.y + 2), ImVec2(box_max.x - 2, box_min.y - 2 - icon_type_overlay_size.y), type_col);
					}
					if (display_label) {
						ImU32 label_col = ImGui::GetColorU32(item_is_selected ? ImGuiCol_Text : ImGuiCol_TextDisabled);
						char label[32];
						sprintf(label, "%d", item_idx);
						draw_list->AddText(ImVec2(box_min.x, box_max.y - ImGui::GetFontSize()), label_col, label);
					}
				}
				ImGui::PopID();
			}
		}
	}
	clipper.End();
	ImGui::PopStyleVar(); // ImGuiStyleVar_ItemSpacing

	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::BeginChild("Cells panel", ImVec2(0.0f, -ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY,
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_HorizontalScrollbar)) {

			ImGui::Text("Selection: %d items", selection.Size);

			static bool changeOccupation = false;

			static gridCellState allCellState = gridCellState::normal;

			const char* elems_names[(uint32_t)gridCellState::stateCount] = { "normal", "shot" };
			const char* elem_name = elems_names[(uint32_t)allCellState];

			const ImVec4 colorText = { 0.8f,0.5f,0.5f,1.0f };

			void* it = NULL; ImGuiID id = 0;

			while (selection.GetNextSelectedItem(&it, &id)) {
				auto& item = _storage[id];

				ImGui::Text("Cell %d: occupation %d, state %s\n style %s", id, item._isOccupied, elems_names[(uint32_t)item._currentState], item._keys[(uint32_t)item._currentState].c_str());

				if (item._isOccupied) {
					uint32_t placed = getItemAt({ id % _rowCount, id / _rowCount });
					if (placed != UINT32_MAX)
						ImGui::TextColored(colorText, "Item's placed id = %d", placed);
					else
						ImGui::TextColored({ 0.8f,0.1f,0.1f,1.0f }, "NO ITEM PLACED! BUG DETECTED!");
				}
				ImGui::NewLine();
			}
			ImGui::Separator();

			it = NULL; id = 0;
			if (ImGui::Checkbox("isOccupied", &changeOccupation))
				while (selection.GetNextSelectedItem(&it, &id))
					_storage[id]._isOccupied = changeOccupation;

			if (ImGui::SliderInt("Slider enum", (int*)&allCellState, 0, (int)gridCellState::stateCount - 1, elem_name)) {
				while (selection.GetNextSelectedItem(&it, &id)) {
					auto& item = _storage[id];

					ImGui::Text("Style %s", item._keys[(uint32_t)item._currentState].c_str());
					ImGui::NewLine();

					item._currentState = allCellState;
				}

			}

			if (ImGui::Button("UpdateStyle"))
				while (selection.GetNextSelectedItem(&it, &id))
					_storage[id].changeState(_storage[id]._currentState);
		}
		ImGui::EndChild();
		ImGui::EndPopup();
	}

	ms_io = ImGui::EndMultiSelect();
	selection.ApplyRequests(ms_io);
}

void grid::setOccupation(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end, bool newOccupation)
{
	for (; begin != end; ++begin)
		begin->_isOccupied = true;
}
void grid::setOccupation(const glm::i32vec2& begin, const glm::i32vec2& end, bool newOccupation)
{
	if (begin.x < (uint32_t)0 || begin.y <  (uint32_t)0 || end.x > _columnCount || end.y > _rowCount)
		throw std::out_of_range("Indices out of range!");
	for (uint32_t i = begin.y; i <= end.y; i++)
		for (uint32_t j = begin.x; j <= end.x; j++)
			_storage[i * _columnCount + j]._isOccupied = newOccupation;
}

bool grid::isOccupied(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end) noexcept
{
	for (; begin != end; ++begin)
		if (begin->_isOccupied)
			return true;
	return false;
}
bool grid::isOccupied(const glm::i32vec2& begin, const glm::i32vec2& end) noexcept
{
	if (!(begin.x <  (uint32_t)0 || begin.y <  (uint32_t)0 || end.x > _columnCount || end.y > _rowCount)) {
		for (uint32_t i = begin.y; i <= end.y; i++) {
			for (uint32_t j = begin.x; j <= end.x; j++) {
				if (_storage[i * _columnCount + j]._isOccupied)
					return true;
			}
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
bool grid::addPlacement(uint32_t itemId, const std::pair<glm::i32vec2, glm::i32vec2>& position)
{
	return addPlacement(itemId, position.first, position.second);
}
void grid::removePlacement(uint32_t itemId)
{
	auto it = _placings.find(itemId);
	if (it != _placings.end()) {
		setOccupation(it->second.first, it->second.second, false);
		_placings.erase(it);
	}
}

const std::pair<glm::i32vec2, glm::i32vec2>* grid::getPlacement(uint32_t itemId) const noexcept
{
	const std::pair<glm::i32vec2, glm::i32vec2>* retVal = nullptr;
	auto& item = _tempPlacings.find(itemId);
	if (item != _tempPlacings.cend())
		retVal = &item->second;

	item = _placings.find(itemId);
	if (item != _placings.cend())
		retVal = &item->second;

	return retVal;
}
uint32_t grid::getItemAt(const glm::i32vec2& position) const noexcept
{
	for (const auto& [shipId, placement] : _placings) {
		if (position.x >= placement.first.x && position.x <= placement.second.x &&
			position.y >= placement.first.y && position.y <= placement.second.y) {
			return shipId;
		}
	}
	return -1;
}
uint32_t grid::getItemAt(uint32_t index) const noexcept
{
	return getItemAt({ index % _rowCount, index / _rowCount });
}
std::vector<uint32_t> grid::getAllItems() const noexcept
{
	std::vector<uint32_t> itemIds;
	itemIds.reserve(_placings.size());
	for (auto& it = _placings.cbegin(); it != _placings.cend(); ++it) {
		itemIds.push_back(it->first);
	}
	return itemIds;
}

bool grid::addTempPlacement(uint32_t itemId, const glm::i32vec2& begin, const glm::i32vec2& end)
{
	if (!isOccupied(begin, end)) {
		_tempPlacings[itemId] = { begin,end };
		return true;
	}
	return false;
}
bool grid::addTempPlacement(uint32_t itemId, const std::pair<glm::i32vec2, glm::i32vec2>& position)
{
	return addTempPlacement(itemId, position.first, position.second);
}
bool grid::acceptPlacing(uint32_t itemId) noexcept
{
	auto it = _tempPlacings.find(itemId);
	if (it != _tempPlacings.end()) {
		if (addPlacement(it->first, it->second.first, it->second.second)) {
			_tempPlacings.erase(itemId);
			return true;
		}
	}
	return false;
}
void grid::rejectPlacing(uint32_t itemId) noexcept
{
	auto it = _tempPlacings.find(itemId);
	if (it != _tempPlacings.end())
		_tempPlacings.erase(it);
}

uint32_t grid::getTempItemAt(const glm::i32vec2& position) const noexcept
{
	for (const auto& [shipId, placement] : _tempPlacings)
		if (position.x >= placement.first.x && position.x <= placement.second.x &&
			position.y >= placement.first.y && position.y <= placement.second.y)
			return shipId;
	return -1;
}
uint32_t grid::getTempItemAt(uint32_t index) const noexcept
{
	return getTempItemAt({ index % _rowCount, index / _rowCount });
}

void grid::changeState(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end, gridCellState newState)
{
	for (; begin != end; ++begin)
		begin->changeState(newState);
}
void grid::changeState(const glm::i32vec2& begin, const glm::i32vec2& end, gridCellState newState)
{
	if (begin.x <  (uint32_t)0 || begin.y <  (uint32_t)0 || end.x > _columnCount || end.y > _rowCount)
		throw std::out_of_range("Indices out of range!");
	for (uint32_t i = begin.y; i <= end.y; i++)
		for (uint32_t j = begin.x; j <= end.x; j++)
			_storage[i * _columnCount + j].changeState(newState);

}
void grid::changeState(const glm::i32vec2& cellPos, gridCellState newState)
{
	_storage.at(cellPos.y * _columnCount + cellPos.x).changeState(newState);
}
void grid::changeState(uint32_t cellIndex, gridCellState newState)
{
	_storage.at(cellIndex).changeState(newState);
}