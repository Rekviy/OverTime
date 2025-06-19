#include "ship.h"
#include "cameraWrapper.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace overtime;

//shipCell
#pragma region shipCell
ship::shipCell::shipCell(glm::vec3 position, const std::vector<std::string>& keys)
	:_pos(position), _keys(keys)
{
	if (keys.size() < (uint32_t)shipCellState::stateCount)
		throw std::invalid_argument("Insufficient keys for ship cell");
	_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
}

void ship::shipCell::changeState(shipCellState newState)
{
	_currentState = newState;
	_style = themeManager::getStyle(_keys[(uint32_t)_currentState]);
}
#pragma endregion

//ship
ship::ship(const std::string& name, const glm::vec3& position, uint32_t length, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
		std::function<bool(ship*)>&& funcOnPress, std::function<bool(ship*)>&& funcOnRelease, std::function<bool(ship*)>&& funcOnMoving, bool isActive)
	:interactElement(name), _size(cellSize), _funcOnPress(funcOnPress), _funcOnRelease(funcOnRelease), _funcOnMove(funcOnMoving)
{
	if (length < 0)
		throw std::invalid_argument("Ship length must be > 0");

	_storage.reserve(length);

	for (uint32_t i = 0, style = 0; i < length; ++i, ++style) {
		style %= keys.size();
		_storage.emplace_back(shipCell({ position.x + _size.x * i,position.y,position.z }, keys.at(style)));
	}
	updateBounds();
	_cellClicked = (uint32_t)ceil((float)length / 2.0f) - 1;
	if (isActive)
		activate();
}

void ship::reset()
{
	changeState(shipCellState::normal);
	changeShipState(shipState::normal);
}

void ship::onRender() noexcept
{
	if (_status & elementFlags::visible) {
		for (auto& item : _storage) {
			renderer2D::drawRotatedSquad(item._pos, _size, -_angle, item._style->_color, item._style->_texture, item._style->_textureSize);
		}
	}
}

void ship::onImGui() noexcept
{
	ImGui::Text("Cell clicked %d", _cellClicked);
	ImGui::NewLine();

	const char* ship_elems_names[(uint32_t)shipState::stateCount] = { "normal", "dragging", "destroyed" };
	const char* ship_elems_name = ship_elems_names[(uint32_t)_currentState];
	ImGui::SliderInt("Slider ship enum", (int*)&_currentState, 0, (int)shipState::stateCount - 1, ship_elems_name);
	ImGui::NewLine();
	glm::vec3 newPos = _storage.front()._pos;
	ImGui::Text("Position: x=%f, y=%f, z=%f", newPos.x, newPos.y, newPos.z);
	if (ImGui::InputFloat3("Position", glm::value_ptr(newPos)))
		setPos(newPos);
	ImGui::NewLine();

	ImGui::Text("Rotation: %f rad", _angle);
	if (ImGui::Button("Rotate"))
		rotate();
	ImGui::NewLine();

	ImGui::Text("Bounds: left=%f, up=%f, right=%f, bottom=%f", _bounds.x, _bounds.y, _bounds.z, _bounds.w);
	if (ImGui::Button("UpdateBounds"))
		updateBounds();
	ImGui::NewLine();

	ImGui::Text("Cell size: x=%f, y=%f", _size.x, _size.y);
	ImGui::InputFloat2("Cell size", glm::value_ptr(_size));
	ImGui::NewLine();

	if (ImGui::Button("Call function on press"))
		_funcOnPress(this);
	ImGui::NewLine();

	if (ImGui::Button("Call function on release"))
		_funcOnRelease(this);
	ImGui::NewLine();

	if (ImGui::Button("Call function on move"))
		_funcOnMove(this);
	ImGui::NewLine();

	//based on assetsBrowser from imgui_demo 
	int items = _storage.size();
	static int iconSpacing = 4, borderDistance = 32;
	static float layoutOuterPadding = 0.0f, layoutItemSpacing = 0.0f, iconSize = 48.0f;
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
			ImGui::SliderFloat("Icon Size", &iconSize, 16.0f, 64.0f, "%.0f");
			ImGui::SliderInt("Icon Spacing", &iconSpacing, 0, 32);
			ImGui::SliderInt("border Distance", &borderDistance, 0, 100);
			ImGui::PopItemWidth();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowContentSize(ImVec2(0.0f, layoutOuterPadding + items * layoutItemSize.y));
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	layoutItemSpacing = (float)iconSpacing;

	layoutItemSize = ImVec2(floorf(iconSize), floorf(iconSize));

	layoutItemStep = ImVec2(layoutItemSize.x + layoutItemSpacing, layoutItemSize.y);

	float LayoutSelectableSpacing = std::max(floorf(layoutItemSpacing), 0.0f);
	layoutOuterPadding = floorf(layoutItemSpacing * 0.5f);

	// Calculate and store start position.
	ImVec2 start_pos = ImGui::GetCursorScreenPos();
	start_pos = ImVec2(borderDistance + start_pos.x + layoutOuterPadding, start_pos.y);
	ImGui::SetCursorScreenPos(start_pos);

	// Multi-select
	ImGuiMultiSelectFlags ms_flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_ClearOnClickVoid | ImGuiMultiSelectFlags_BoxSelect2d;
	ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(ms_flags, selection.Size, items);

	selection.UserData = this;
	selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self_, int idx) { return (ImGuiID)idx; };
	selection.ApplyRequests(ms_io);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(LayoutSelectableSpacing, 0.0f));

	ImGuiListClipper clipper;
	clipper.Begin(items, layoutItemStep.y);

	while (clipper.Step()) {
		for (int item_idx = clipper.DisplayStart; item_idx < clipper.DisplayEnd; item_idx++) {

			shipCell* item_data = &_storage[item_idx];
			ImGui::PushID(item_idx);

			ImVec2 pos = ImVec2(start_pos.x + item_idx * layoutItemStep.x, start_pos.y + layoutItemStep.y);
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
				draw_list->AddRectFilled(box_min, box_max, im_bg_color[(int)item_data->_currentState % IM_ARRAYSIZE(im_bg_color)]);

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
	clipper.End();
	ImGui::PopStyleVar(); // ImGuiStyleVar_ItemSpacing

	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::BeginChild("Cells panel", ImVec2(0.0f, -ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY,
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_HorizontalScrollbar)) {

			ImGui::Text("Selection: %d items", selection.Size);

			static shipCellState allCellState = shipCellState::normal;

			const char* elems_names[(uint32_t)shipCellState::stateCount] = { "normal", "placingAllowed","placingDenied", "shot" };
			const char* elem_name = elems_names[(uint32_t)allCellState];

			void* it = NULL; ImGuiID id = 0;

			while (selection.GetNextSelectedItem(&it, &id)) {
				auto& item = _storage[id];

				ImGui::Text("Cell %d: position x=%f y=%f z=%f\n state %s\n style %s", id, item._pos.x, item._pos.y, item._pos.z,
					elems_names[(uint32_t)item._currentState],
					item._keys[(uint32_t)item._currentState].c_str());

				ImGui::NewLine();
			}
			ImGui::Separator();

			it = NULL; id = 0;

			if (ImGui::SliderInt("Slider enum", (int*)&allCellState, 0, (int)shipCellState::stateCount - 1, elem_name)) {
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

void ship::onEvent(event& event)
{
	if (!(_status & elementFlags::blocked) && (_status & elementFlags::active)) {
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
	return _storage.begin()->_pos;
}

void ship::setPos(const glm::vec3& newPos)
{
	calculateNewPos(0, newPos);

	updateBounds();
}

shipCellState ship::getState() const noexcept
{
	int32_t temp = 0;
	for (auto& item : _storage)
		temp |= (uint32_t)item._currentState;
	return (shipCellState)temp;
}
shipCellState ship::getState(uint32_t cell) const noexcept
{
	if (cell > _storage.size())
		return shipCellState::stateCount;
	return _storage[cell]._currentState;
}

void ship::changeState(shipCellState newState)
{
	for (auto& item : _storage) {
		item.changeState(newState);
	}
}
void ship::changeState(uint32_t cell, shipCellState newState)
{
	try {
		_storage.at(cell).changeState(newState);
	}
	catch (const std::exception& ex) {
		OT_ERROR("ship::changeState Error: {0}", ex.what());
		return;
	}
	if (_currentState == shipState::normal) {
		uint32_t tempState = (uint32_t)shipCellState::shot;
		for (auto& cell : _storage)
			tempState &= (uint32_t)cell._currentState;
		if (tempState == (uint32_t)shipCellState::shot)
			changeShipState(shipState::destroyed);
	}
}

void ship::rotate()
{
	_angle += glm::radians(90.0f);
	if (_angle >= glm::two_pi<float>()) _angle -= glm::two_pi<float>();

	calculateNewPos(_cellClicked, _storage[_cellClicked]._pos);
}

void ship::changeShipState(shipState newState)
{
	_currentState = newState;
}

void ship::updateBounds()
{
	glm::vec2 first = _storage.front()._pos, end = (_storage.cend() - 1)->_pos;
	if (end.x < first.x) std::swap(first.x, end.x);
	if (end.y > first.y) std::swap(first.y, end.y);


	first = { first.x - _size.x / 2.0f, first.y + _size.y / 2.0f };
	end = { end.x + _size.x / 2.0f,end.y - _size.y / 2.0f };

	_bounds = {
		cameraWrapper::worldToScreen({ first.x,first.y }),//UpLeft
		cameraWrapper::worldToScreen({ end.x, end.y }) //BottomRight
	};
}
void ship::calculateNewPos(uint32_t cellClicked, const glm::vec3& newPos)
{
	glm::vec3 basePos = _storage[cellClicked]._pos = newPos;
	float c = glm::cos(_angle), s = glm::sin(_angle);
	for (uint32_t i = 0; i < _storage.size(); ++i) {
		int delta = (int)i - (int)cellClicked;

		glm::vec3 newPos = basePos;

		newPos.y -= (float)delta * _size.y * s; newPos.x += (float)delta * _size.x * c;
		_storage[i]._pos = newPos;
	}
}

bool ship::onWindowResize(windowResizeEvent& event)
{
	updateBounds();
	return false;
}
bool ship::onMouseMoved(mouseMovedEvent& event)
{
	if (_currentState == shipState::dragging) {
		calculateNewPos(_cellClicked, { cameraWrapper::screenToWorld({ event.getX(),event.getY() }), _storage[_cellClicked]._pos.z });

		return _funcOnMove(this);
	}

	return false;
}
bool ship::onMouseButtonPressed(mouseButtonPressedEvent& event)
{
	auto [mouseX, mouseY] = input::getMousePos();

	if (mouseX > _bounds.x && mouseY > _bounds.y &&
		mouseX < _bounds.z && mouseY < _bounds.w) {

		glm::vec2 worldClick = cameraWrapper::screenToWorld({ mouseX, mouseY });
		const glm::vec3& pivotPoint = (_storage.begin())->_pos;
		float c = cos(_angle), s = -sin(_angle);

		glm::vec2 delta = { worldClick.x - pivotPoint.x, worldClick.y - pivotPoint.y };
		glm::vec2 rotatedClickPos = { delta.x * c - delta.y * s,delta.x * s + delta.y * c };

		delta = abs(round(rotatedClickPos / _size));
		_cellClicked = (uint32_t)delta.x;
		//_clickOffset = { _storage.at(_cellClicked)._pos.x - mousePos.x,_storage.at(_cellClicked)._pos.y - mousePos.y };
		_currentState = shipState::dragging;
		return _funcOnPress(this);
	}
	return false;
}
bool ship::onMouseButtonReleased(mouseButtonReleasedEvent& event)
{
	switch (event.getMouseButton()) {
		case mouseButtonCodes::OT_MOUSE_BUTTON_LEFT:
			if (_currentState == shipState::dragging) {
				_currentState = shipState::normal;
				updateBounds();
				return _funcOnRelease(this);
			}
			break;
	}
	return false;
}
bool ship::onKeyPressed(keyPressedEvent& event)
{
	if (_currentState == shipState::dragging) {
		switch (event.getKeyCode()) {
			case keyCodes::OT_KEY_R: {
				rotate();
				//TEMP todo add onKeyPressedFunc!!!
				return _funcOnMove(this);
			}
		}
		return true;
	}
	return false;
}

ship1::ship1(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	std::function<bool(ship*)>&& funcOnPress, std::function<bool(ship*)>&& funcOnRelease, std::function<bool(ship*)>&& funcOnMoving, bool isActive)
	:ship(name, position, 1, cellSize, keys, std::move(funcOnPress), std::move(funcOnRelease), std::move(funcOnMoving), isActive)
{}

ship2::ship2(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	std::function<bool(ship*)>&& funcOnPress, std::function<bool(ship*)>&& funcOnRelease, std::function<bool(ship*)>&& funcOnMoving, bool isActive)
	:ship(name, position, 2, cellSize, keys, std::move(funcOnPress), std::move(funcOnRelease), std::move(funcOnMoving), isActive)
{}

ship3::ship3(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	std::function<bool(ship*)>&& funcOnPress, std::function<bool(ship*)>&& funcOnRelease, std::function<bool(ship*)>&& funcOnMoving, bool isActive)
	:ship(name, position, 3, cellSize, keys, std::move(funcOnPress), std::move(funcOnRelease), std::move(funcOnMoving), isActive)
{}

ship4::ship4(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
	std::function<bool(ship*)>&& funcOnPress, std::function<bool(ship*)>&& funcOnRelease, std::function<bool(ship*)>&& funcOnMoving, bool isActive)
	:ship(name, position, 4, cellSize, keys, std::move(funcOnPress), std::move(funcOnRelease), std::move(funcOnMoving), isActive)
{}