#include "uiLayer.h"
#include "ui/themeManager.h"
#include "ui/button.h"

using namespace overtime;

uiLayer::uiLayer() :layer("uiLayer")
{
	themeManager::add("cherry", themeManager::style(texture2D::create("assets/resources/cherry.png")));
	themeManager::add("otj", themeManager::style(texture2D::create("assets/resources/proxy555.jpg"), 1.0f));
	themeManager::add("stonks", themeManager::style(texture2D::create("assets/resources/Screenshot.png"), 1.0f, glm::vec4(0.4f, 0.3f, 0.2f, 1.0f)));
	std::function<bool()> fn;
	elems.emplace_back(
		new button(glm::vec3(100.0f, 100.0f, 0.0f), glm::vec2(100.0f, 100.0f), { "cherry", "otj", "stonks" },
		[]() {OT_WARN("Button released!"); return false; },
		[]() {OT_WARN("Button pressed!"); return false; })
	);

}
void uiLayer::onAttach()
{}

void uiLayer::onDetach()
{
	themeManager::shutdown();
}

void uiLayer::onUpdate(timeStep ts)
{
	renderer2D::beginScene(cameraWrapper::getCamera());
	for (auto& item : elems) {
		item->onRender();
	}

	renderer2D::endScene();
}

void uiLayer::onImGuiRender()
{
	ImGui::Begin("UI interactive elems settings");
	static int counter = 0;
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

	ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { if (counter > 0) counter--; }
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { if (counter < elems.size() - 1)  counter++; }
	ImGui::SameLine();
	ImGui::Text("item Id: %d", counter);
	ImGui::NewLine();

	glm::vec3 pos = elems.at(counter)->getPos();

	ImGui::Text("item position: x=%f y=%f z=%f", pos.x, pos.y, pos.z);
	ImGui::NewLine();
	if (ImGui::ArrowButton("##posLeft", ImGuiDir_Left)) { elems.at(counter)->setPos({ pos.x - 1.0f,pos.y,pos.z }); }
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##posUp", ImGuiDir_Up)) { elems.at(counter)->setPos({ pos.x ,pos.y + 1.0f,pos.z }); }
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##posDown", ImGuiDir_Down)) { elems.at(counter)->setPos({ pos.x ,pos.y - 1.0f,pos.z }); }
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##posRight", ImGuiDir_Right)) { elems.at(counter)->setPos({ pos.x + 1.0f,pos.y,pos.z }); }

	ImGui::NewLine();

	glm::vec2 size = elems.at(counter)->getSize();

	ImGui::Text("item size: x=%f y=%f", size.x, size.y);
	ImGui::NewLine();
	if (ImGui::ArrowButton("##xInc", ImGuiDir_Left)) { elems.at(counter)->setSize({ size.x + 1.0f ,size.y }); }
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##xDec", ImGuiDir_Right)) { elems.at(counter)->setSize({ size.x - 1.0f,size.y }); }
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##yInc", ImGuiDir_Up)) { elems.at(counter)->setSize({ size.x  ,size.y + 1.0f }); }
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##yDec", ImGuiDir_Down)) { elems.at(counter)->setSize({ size.x ,size.y - 1.0f }); }
	ImGui::PopItemFlag();
	ImGui::End();
}

void uiLayer::onEvent(overtime::event& event)
{
	eventDispatcher dispatcher(event);
	dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(uiLayer::onWindowResize));
	for (auto it = elems.end(); it != elems.begin();) {
		(*--it)->onEvent(event);
		if (event.isHandled())
			break;
	}
}

bool uiLayer::onWindowResize(overtime::windowResizeEvent& event)
{
	cameraWrapper::updateCamera(event.getWidth(), event.getHeight());
	return false;
}

