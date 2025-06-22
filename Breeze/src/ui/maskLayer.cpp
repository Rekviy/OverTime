#include "maskLayer.h"
#include "cameraWrapper.h"

#include <glm/gtc/type_ptr.hpp>

using namespace overtime;


maskCell::maskCell(const overtime::ref<style> style)
	:_style(style)
{}

maskLayer::maskLayer(const std::string& name, uint32_t rowCount, uint32_t columnCount, const glm::vec3& startPosition, const glm::vec2& cellSize,
	std::array<std::string, (uint32_t)maskCellState::stateCount>&& keys, std::function<bool(maskLayer*)>&& funcOnRelease, bool isActive)
	: interactElement(name), _rowCount(rowCount), _columnCount(columnCount), _pos(startPosition), _size(cellSize), _keys(keys), _funcOnRelease(funcOnRelease)
{
	uint32_t count = rowCount * columnCount;
	if (count < 0) throw std::invalid_argument("MaskLayer cells count cannot be lesser than 0!");
	if (_keys.empty()) throw std::invalid_argument("MaskLayer keys cannot be empty!");

	if (!themeManager::isKeysValid(std::vector<std::string>(_keys.cbegin(), _keys.cend())))
		throw std::invalid_argument("Style keys not valid! Name: " + _name);

	_storage.reserve(count);
	for (uint32_t i = 0; i < count; ++i)
		_storage.emplace_back(maskCell(themeManager::getStyle(_keys[(uint32_t)maskCellState::idle])));
	updateBounds();
	if (isActive)
		activate();
}

void maskLayer::onRender() noexcept
{
	if (_status & elementFlags::visible) {
		uint32_t x = 0, y = 0;
		for (auto& cell : _storage) {
			if (cell._isVisible)
				renderer2D::drawSquad({ _pos.x + x * _size.x, _pos.y - y * _size.y, _pos.z }, _size,
					cell._style->_color, cell._style->_texture, cell._style->_textureSize);
			++x %= _columnCount;
			if (x == 0) y++;
		}
	}
}

void maskLayer::onImGui() noexcept
{
	ImGui::Text("Cell clicked:%d", _cellClicked);
	ImGui::NewLine();

	ImGui::Text("Columns:%d rows:%d", _columnCount, _rowCount);
	ImGui::NewLine();

	ImGui::Text("Position: x=%f, y=%f, z=%f", _pos.x, _pos.y, _pos.z);
	ImGui::InputFloat3("Position", glm::value_ptr(_pos));
	ImGui::NewLine();

	ImGui::Text("Cell size: x=%f, y=%f", _size.x, _size.y);
	ImGui::InputFloat2("Cell size", glm::value_ptr(_size));
	ImGui::NewLine();

	ImGui::Text("Bounds: left=%f, up=%f, right=%f, bottom=%f", _bounds.x, _bounds.y, _bounds.z, _bounds.w);
	if (ImGui::Button("UpdateBounds"))
		updateBounds();
	ImGui::NewLine();
	//based on assetsBrowser from imgui_demo 
	size_t items = _storage.size();
	static bool stretchSpacing = true;
	static int iconSpacing = 0;
	static float layoutOuterPadding = 0.0f, layoutItemSpacing = 0.0f, iconSize = 32.0f;
	static ImVec2 layoutItemSize(_size.x, _size.y);
	static ImVec2 layoutItemStep(layoutItemSize.x + layoutItemSpacing, layoutItemSize.y + layoutItemSpacing);

	// Rendering parameters
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
	if (stretchSpacing == false)
		avail_width += floorf(layoutItemSpacing * 0.5f);
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
			const int item_max_idx_for_current_line = std::min((line_idx + 1) * (int)_columnCount, (int)items);
			for (int item_idx = item_min_idx_for_current_line; item_idx < item_max_idx_for_current_line; ++item_idx) {
				maskCell* item_data = &_storage[item_idx];
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
					draw_list->AddRectFilled(box_min, box_max, im_bg_color[item_data->_isVisible % IM_ARRAYSIZE(im_bg_color)]);

					if (display_label) {
						ImU32 label_col = ImGui::GetColorU32(item_is_selected ? ImGuiCol_Text : ImGuiCol_TextDisabled);
						char label[32];
						sprintf_s(label, "%d", item_idx);
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

			static bool changeVisibility = false;

			static maskCellState allCellState = maskCellState::idle;

			const char* elems_names[(uint32_t)maskCellState::stateCount] = { "idle", "hover", "clicked" };
			const char* elem_name = elems_names[(uint32_t)allCellState];

			void* it = NULL; ImGuiID id = 0;

			while (selection.GetNextSelectedItem(&it, &id)) {
				auto& item = _storage[id];

				ImGui::Text("Cell %d: visible %d, state %s\n style %s", id, item._isVisible, elems_names[(uint32_t)item._currentState], _keys[(uint32_t)item._currentState]);
			}
			ImGui::Separator();

			it = NULL; id = 0;
			if (ImGui::Checkbox("isVisible", &changeVisibility))
				while (selection.GetNextSelectedItem(&it, &id))
					_storage[id]._isVisible = changeVisibility;

			if (ImGui::SliderInt("Slider enum", (int*)&allCellState, 0, (int)maskCellState::stateCount - 1, elem_name))
				while (selection.GetNextSelectedItem(&it, &id))
					_storage[id]._currentState = allCellState;

			if (ImGui::Button("UpdateStyle"))
				while (selection.GetNextSelectedItem(&it, &id))
					_storage[id]._style = themeManager::getStyle(_keys[(uint32_t)_storage[id]._currentState]);
		}
		ImGui::EndChild();
		ImGui::EndPopup();
	}

	ms_io = ImGui::EndMultiSelect();
	selection.ApplyRequests(ms_io);
}

void maskLayer::reset()
{
	auto defStyle = themeManager::getStyle(_keys[(uint32_t)maskCellState::idle]);
	for (auto& cell : _storage) {
		cell._isVisible = true;
		cell._currentState = maskCellState::idle;
		cell._style = defStyle;
	}
	dropFlag(elementFlags::blocked);
	deactivate();
}

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

void maskLayer::setCellVisibility(uint32_t cell, bool newVisibility)
{
	if (cell<(uint32_t)0 || cell > _storage.size()) throw maskLayerOutOfRange("SetCellVisibility, index out of range!",
		{ cell % _rowCount ,cell / _rowCount }, { cell % _rowCount ,cell / _rowCount });
	_storage[cell]._isVisible = newVisibility;
}

void maskLayer::setCellVisibility(const glm::i32vec2& begin, const glm::i32vec2& end, bool newVisibility)
{
	if (begin.x < (uint32_t)0 || begin.y <  (uint32_t)0 || end.x > _columnCount || end.y > _rowCount)
		throw maskLayerOutOfRange("SetCellVisibility, indices out of range!", begin, end);
	for (int i = begin.y; i <= end.y; i++)
		for (int j = begin.x; j <= end.x; j++)
			_storage.at(i * _columnCount + j)._isVisible = newVisibility;
}

inline void maskLayer::onEvent(event& event)
{
	if (!(_status & elementFlags::blocked) && (_status & elementFlags::active)) {
		eventDispatcher dispatcher(event);
		dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(maskLayer::onWindowResize));
		dispatcher.dispatch<mouseMovedEvent>(OT_BIND_EVENT_FN(maskLayer::onMouseMoved));
		dispatcher.dispatch<mouseButtonPressedEvent>(OT_BIND_EVENT_FN(maskLayer::onMouseButtonPressed));
		dispatcher.dispatch<mouseButtonReleasedEvent>(OT_BIND_EVENT_FN(maskLayer::onMouseButtonReleased));
	}
}

bool maskLayer::onWindowResize(overtime::windowResizeEvent& event)
{
	updateBounds();	return false;
}
bool maskLayer::onMouseMoved(overtime::mouseMovedEvent& event)
{
	glm::vec2 mousePos(event.getX(), event.getY());

	if (mousePos.x > _bounds.x && mousePos.y > _bounds.y &&
		mousePos.x < _bounds.z && mousePos.y < _bounds.w) {
		glm::vec2 worldClick = cameraWrapper::screenToWorld({ mousePos.x, mousePos.y });

		glm::vec2 clickPos = { worldClick.x - _pos.x, worldClick.y - _pos.y };

		clickPos = abs(round(clickPos / _size));

		uint32_t newCellClicked = (uint32_t)(clickPos.y * _rowCount + clickPos.x);
		if (_cellClicked != newCellClicked) {
			auto& managedCell = _storage[_cellClicked];
			managedCell._currentState = maskCellState::idle;
			managedCell._style = themeManager::getStyle(_keys[(uint32_t)managedCell._currentState]);

			_cellClicked = newCellClicked;
		}

		auto& managedCell = _storage[_cellClicked];
		if (managedCell._currentState == maskCellState::idle && managedCell._isVisible) {
			managedCell._currentState = maskCellState::hover;
			managedCell._style = themeManager::getStyle(_keys[(uint32_t)managedCell._currentState]);
		}

		return true;
	}
	else if (_storage[_cellClicked]._currentState != maskCellState::idle) {
		auto& managedCell = _storage[_cellClicked];
		managedCell._currentState = maskCellState::idle;
		managedCell._style = themeManager::getStyle(_keys[(uint32_t)managedCell._currentState]);
	}
	return false;

	return false;
}
bool maskLayer::onMouseButtonPressed(overtime::mouseButtonPressedEvent& event)
{

	switch (event.getMouseButton()) {
		case mouseButtonCodes::OT_MOUSE_BUTTON_LEFT:
			auto& managedCell = _storage[_cellClicked];
			if (managedCell._currentState != maskCellState::idle) {
				managedCell._currentState = maskCellState::clicked;
				managedCell._style = themeManager::getStyle(_keys[(uint32_t)managedCell._currentState]);
				return true;
			}
	}
	return false;
}
bool maskLayer::onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event)
{
	auto& managedCell = _storage[_cellClicked];
	if (managedCell._currentState != maskCellState::idle) {
		managedCell._currentState = maskCellState::hover;
		managedCell._style = themeManager::getStyle(_keys[(uint32_t)managedCell._currentState]);
		return _funcOnRelease(this);
	}
	return false;
}