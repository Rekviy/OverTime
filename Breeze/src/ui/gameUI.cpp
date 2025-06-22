#include "gameUI.h"
#include "themeManager.h"

using namespace overtime;
gameUI::gameUI()
{

}

overtime::scope<interactElement> gameUI::pop(uint32_t id)
{
	try {
		return _pool.pop(id);
	}
	catch (const std::out_of_range& ex) {
		throw UIOutOfRange(ex.what(), id, _pool.size());
	}
	
}
interactElement* gameUI::get(uint32_t id)
{
	return _pool.get(id);
}
void gameUI::activate(const std::vector<uint32_t>& ids)
{
	for (auto& item : ids)
		_pool.activate(item);
}
bool gameUI::activate(uint32_t id)
{
	return _pool.activate(id);
}
uint32_t gameUI::activateFirst(elementType type)
{
	return _pool.activateFirst(type);
}
void gameUI::activateAll()
{
	_pool.activateAll();
}
void gameUI::deactivate(const std::vector<uint32_t>& ids)
{
	for (auto& item : ids)
		_pool.deactivate(item);
}
void gameUI::deactivate(uint32_t id)
{
	_pool.deactivate(id);
}
void gameUI::deactivateAll()
{
	_pool.deactivateAll();
}
bool gameUI::isExist(uint32_t id)
{
	return _pool.isExist(id);
}

std::vector<overtime::scope<interactElement>> gameUI::setTypeCap(elementType type, uint32_t newCap)
{
	try {
		return _pool.setTypeCap(type, newCap);
	}
	catch (const std::out_of_range& ex) {
		throw UIOutOfRange(ex.what(), type, elementType::unknown);
	}
	
}
std::vector<uint32_t> gameUI::setTypeActiveCap(elementType type, uint32_t newCap)
{
	try {
		return _pool.setTypeActiveCap(type, newCap);
	}
	catch (const std::out_of_range& ex) {
		throw UIOutOfRange(ex.what(), type, elementType::unknown);
	}
}

uint32_t gameUI::checkTypeCap(elementType type) const
{
	try {
		return _pool.checkTypeCap(type);
	}
	catch (const std::out_of_range& ex) {
		throw UIOutOfRange(ex.what(), type, elementType::unknown);
	}
}
uint32_t gameUI::checkTypeActiveCap(elementType type) const
{
	try {
		return _pool.checkTypeActiveCap(type);
	}
	catch (const std::out_of_range& ex) {
		throw UIOutOfRange(ex.what(), type, elementType::unknown);
	}
}

bool gameUI::bind(uint32_t childId, uint32_t ParentId)
{
	auto& child = _pool.find(childId), parent = _pool.find(ParentId);
	if (child != _pool.end() && parent != _pool.end()) {

		_bindings[parent->first].push_back(child->first);
		_parents[child->first].push_back(parent->first);

		return true;
	}
	return false;
}
bool gameUI::unBind(uint32_t childId, uint32_t ParentId)
{
	auto& childrensIt = _bindings.find(ParentId), parentsIt = _parents.find(childId);

	if (childrensIt != _bindings.end() && parentsIt != _parents.end()) {

		childrensIt->second.erase(std::remove(childrensIt->second.begin(), childrensIt->second.end(), childId), childrensIt->second.end());
		if (childrensIt->second.empty())
			_bindings.erase(ParentId);

		parentsIt->second.erase(std::remove(parentsIt->second.begin(), parentsIt->second.end(), ParentId), parentsIt->second.end());
		if (parentsIt->second.empty())
			_parents.erase(childId);
		return true;
	}
	return false;
}

bool gameUI::unBindAll(uint32_t unBindFrom)
{
	auto& it = _bindings.find(unBindFrom);
	if (it != _bindings.end()) {
		for (auto& child : it->second) {
			auto& childVec = _parents.find(child)->second;
			childVec.erase(std::remove(childVec.begin(), childVec.end(), unBindFrom), childVec.end());
			if (childVec.empty())
				_parents.erase(child);
		}
		_bindings.erase(it);
		return true;
	}
	return false;
}
const std::vector<uint32_t> gameUI::getBindings(uint32_t ParentId)
{
	std::vector<uint32_t> returnVec;
	auto& it = _bindings.find(ParentId);
	if (it != _bindings.end())
		returnVec = it->second;

	return returnVec;
}

const std::vector<uint32_t> gameUI::getParents(uint32_t childId)
{
	std::vector<uint32_t> returnVec;
	auto& it = _parents.find(childId);

	if (it != _parents.end())
		returnVec = it->second;
	return returnVec;
}

void gameUI::onRender()
{
	for (auto& it = _pool.begin(); it != _pool.end(); ++it) {
		it->second->onRender();
	}
}

struct itemsOnImGui {
	std::function<void(const char*, itemsOnImGui*)>func;
	bool _opened = true;
	int item_selected = 0;
};

void gameUI::onImGuiRender()
{
	static std::vector<itemsOnImGui*> activeWindows;
	static auto imGuiFunc = [this](const char* title, itemsOnImGui* items) {
		const char* preview_value = _pool.get(items->item_selected)->getName().c_str();

		ImGui::SetNextWindowSize(ImVec2(480.0f, 360.0f), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, &items->_opened, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings)) {
			ImGui::End();
			return;
		}
		if (ImGui::BeginCombo("combo (w/filter)", preview_value)) {
			static ImGuiTextFilter filter;
			if (ImGui::IsWindowAppearing()) {
				ImGui::SetKeyboardFocusHere();
				filter.Clear();
			}
			ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
			filter.Draw("##Filter", -FLT_MIN);

			for (auto& it = _pool.begin(); it != _pool.end(); ++it) {
				std::string name = it->second->getName() + " id: " + std::to_string(it->second->getId());
				const bool is_selected = (_pool.begin()->first == it->second->getId());
				if (filter.PassFilter(name.c_str()))
					if (ImGui::Selectable(name.c_str(), is_selected))
						items->item_selected = it->second->getId();
			}
			ImGui::EndCombo();
		}
		auto& item = *_pool.get(items->item_selected);

		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

		ImGui::Text("Item id: %u, name: %s", item.getId(), item.getName().c_str());
		ImGui::NewLine();

		ImGui::Text("Binded to ");
		auto it = _parents.find(items->item_selected);
		if (it != _parents.end())
			for (auto iter = it->second.cbegin(); iter != it->second.cend(); ++iter) {
				ImGui::Text("%d, ", *iter);
				ImGui::SameLine();
			}
		ImGui::NewLine();

		ImGui::Text("Parent to ");
		it = _bindings.find(items->item_selected);
		if (it != _bindings.end())
			for (auto iter = it->second.cbegin(); iter != it->second.cend(); ++iter){
				ImGui::Text("%d, ", *iter);
				ImGui::SameLine();
			}
		ImGui::NewLine();

		ImGui::Text("Visibility: %s", item.checkFlag(elementFlags::visible) ? "True" : "False");
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Show"))
			item.setFlag(elementFlags::visible);
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Hide"))
			item.dropFlag(elementFlags::visible);
		ImGui::NewLine();

		ImGui::Text("Blocked: %s", item.checkFlag(elementFlags::blocked) ? "True" : "False");
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Block"))
			item.setFlag(elementFlags::blocked);
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Unblock"))
			item.dropFlag(elementFlags::blocked);
		ImGui::NewLine();

		ImGui::Text("Active: %s", item.checkFlag(elementFlags::active) ? "True" : "False");
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Activate"))
			_pool.activate(item.getId());
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Deactivate"))
			_pool.deactivate(item.getId());
		ImGui::NewLine();

		item.onImGui();

		ImGui::End();
	};

	static std::string title = "UI interactive elems settings #";
	if (_pool.size() > 0) {
		if (ImGui::Button("Open UI debug window")) {
			auto* item = new itemsOnImGui();
			item->item_selected = _pool.begin()->second->getId();
			item->func = imGuiFunc;
			activeWindows.push_back(item);
		}
		for (uint32_t i = activeWindows.size(); i > 0;) {
			if (activeWindows[--i]->_opened)
				activeWindows[i]->func((title + std::to_string(i)).c_str(), activeWindows[i]);
			else {
				delete activeWindows[i];
				activeWindows.erase(activeWindows.begin() + i);
			}
		}
	}
}

void gameUI::onEvent(overtime::event& event)
{
	eventDispatcher dispatcher(event);
	for (auto it = _pool.activeBegin(); it != _pool.activeEnd(); ++it) {
		for (auto vecIt = it->second.end(); vecIt != it->second.begin();) {
			_pool.get(*(--vecIt))->onEvent(event);
			if (event.isHandled())
				break;
		}
	}
}