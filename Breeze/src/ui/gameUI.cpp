#include "gameUI.h"
#include "themeManager.h"

using namespace overtime;
gameUI::gameUI()
{

}

gameUI::~gameUI()
{}
overtime::scope<interactElement> gameUI::pop(uint32_t id)
{
	return _pool.pop(id);
}
interactElement& gameUI::get(uint32_t id)
{
	return _pool.get(id);
}
void gameUI::activate(const std::vector<uint32_t>& ids)
{
	for (auto& item : ids)
		_pool.activate(item);

}
void gameUI::activate(uint32_t id)
{
	_pool.activate(id);
}
uint32_t gameUI::activateFirst(elementType type)
{
	return _pool.activateFirst(type);
}
void gameUI::deactivate(uint32_t id)
{
	_pool.deactivate(id);
}
void gameUI::deactivateAll()
{
	for (auto& it = _pool.begin(); it != _pool.end(); ++it) {
		it->second->deactivate();
		it->second->setVisibility(false);
	}
}
bool gameUI::isExist(uint32_t id)
{
	return _pool.isExist(id);
}

void gameUI::setTypeCap(elementType type, uint32_t newCap)
{
	_pool.setTypeCap(type, newCap);
}
void gameUI::setTypeActiveCap(elementType type, uint32_t newCap)
{
	_pool.setTypeActiveCap(type, newCap);
}

uint32_t gameUI::checkTypeCap(elementType type) const
{
	return _pool.checkTypeCap(type);
}
uint32_t gameUI::checkTypeActiveCap(elementType type) const
{
	return _pool.checkTypeActiveCap(type);
}

bool gameUI::bind(uint32_t elementId, uint32_t bindTo)
{
	auto& itE = _pool.find(elementId);
	auto& itB = _pool.find(bindTo);
	if (itE == _pool.end() || itB == _pool.end())
		return false;
	_bindings[itB->first].push_back(itE->first);

	return true;
}
bool gameUI::unBind(uint32_t elementId, uint32_t unBindFrom)
{
	auto& itB = _bindings.find(unBindFrom);
	if (itB == _bindings.end())
		return false;

	for (auto& it = itB->second.begin(); it != itB->second.end(); ++it) {
		if (*it == elementId) {
			itB->second.erase(it);
			return true;
		}
	}

	return false;
}

bool gameUI::unBindAll(uint32_t unBindFrom)
{
	auto& it = _bindings.find(unBindFrom);
	if (it == _bindings.end())
		return false;
	it->second.clear();
	return true;
}
const std::vector<uint32_t>& gameUI::getBindings(uint32_t element)
{
	auto& it = _bindings.find(element);

	OT_ASSERT(it != _bindings.end(), "Element doesn't binded!");
	return it->second;
}
//todo add game status to parameters
void gameUI::onRender()
{
	for (auto& it = _pool.begin(); it != _pool.end(); ++it) {
		it->second->onRender();
	}
}

void gameUI::onImGuiRender()
{
	ImGui::Begin("UI interactive elems settings");
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);

	static int item_selected = _pool.begin()->second->getId();

	if (_pool.size() > 0) {
		const char* preview_value = _pool.get(item_selected).getName().c_str();
		if (ImGui::BeginCombo("combo 2 (w/ filter)", preview_value)) {
			static ImGuiTextFilter filter;
			if (ImGui::IsWindowAppearing()) {
				ImGui::SetKeyboardFocusHere();
				filter.Clear();
			}
			ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
			filter.Draw("##Filter", -FLT_MIN);
			uint32_t i = 0;
			for (auto& it = _pool.begin(); it != _pool.end(); ++it) {
				const char* name = it->second->getName().c_str();
				const bool is_selected = (_pool.begin()->first == it->second->getId());
				if (filter.PassFilter(name))
					if (ImGui::Selectable(name, is_selected))
						item_selected = it->second->getId();
			}

			ImGui::EndCombo();
		}
		auto& item = _pool.get(item_selected);
		ImGui::Text("Item id: %u", item.getId());

		ImGui::NewLine();

		ImGui::Text("Visibility: %s", item.isVisible() ? "True" : "False");
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Show"))
			item.setVisibility(true);
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Hide"))
			item.setVisibility(false);
		ImGui::NewLine();
		ImGui::Text("Active: %s", item.isActive() ? "True" : "False");
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Activate"))
			_pool.activate(item.getId());
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Deactivate"))
			_pool.deactivate(item.getId());
		ImGui::NewLine();

		glm::vec3 pos = item.getPos();

		ImGui::Text("Position: x=%f y=%f z=%f", pos.x, pos.y, pos.z);

		ImGui::NewLine();

		if (ImGui::ArrowButton("##posLeft", ImGuiDir_Left)) { item.setPos({ pos.x - 1.0f,pos.y,pos.z }); }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##posUp", ImGuiDir_Up)) { item.setPos({ pos.x ,pos.y + 1.0f,pos.z }); }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##posDown", ImGuiDir_Down)) { item.setPos({ pos.x ,pos.y - 1.0f,pos.z }); }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##posRight", ImGuiDir_Right)) { item.setPos({ pos.x + 1.0f,pos.y,pos.z }); }
		ImGui::SameLine(0.0f, spacing+5.0f);
		if (ImGui::ArrowButton("##posTo", ImGuiDir_Up)) { item.setPos({ pos.x, pos.y,pos.z + 0.1f }); }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##posFrom", ImGuiDir_Down)) { item.setPos({ pos.x,pos.y,pos.z - 0.1f }); }

		ImGui::NewLine();

		glm::vec2 size = item.getSize();

		ImGui::Text("Size: x=%f y=%f", size.x, size.y);

		ImGui::NewLine();

		if (ImGui::ArrowButton("##xInc", ImGuiDir_Left)) { item.setSize({ size.x + 1.0f ,size.y }); }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##xDec", ImGuiDir_Right)) { item.setSize({ size.x - 1.0f,size.y }); }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##yInc", ImGuiDir_Up)) { item.setSize({ size.x  ,size.y + 1.0f }); }
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::ArrowButton("##yDec", ImGuiDir_Down)) { item.setSize({ size.x ,size.y - 1.0f }); }

	}
	ImGui::PopItemFlag();
	ImGui::End();
}

void gameUI::onEvent(overtime::event& event)
{
	eventDispatcher dispatcher(event);
	for (auto it = _pool.end(); it != _pool.begin();) {
		(*--it).second->onEvent(event);
		if (event.isHandled())
			break;
	}
}



