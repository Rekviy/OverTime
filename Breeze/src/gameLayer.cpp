#include "gameLayer.h"
#include "cameraWrapper.h"
#include "ui/counter.h"
#include "ui/button.h"
#include "ui/objectPool.h"
using namespace overtime;

gameLayer::gameLayer()
	:layer("gameLayer")
{

	themeManager::add("play", themeManager::style(texture2D::create("assets/resources/play.png"), 1.0f));
	themeManager::add("playHover", themeManager::style(texture2D::create("assets/resources/playHover.png"), 1.0f));
	themeManager::add("playClicked", themeManager::style(texture2D::create("assets/resources/playClicked.png"), 1.0f));

	themeManager::add("cherry", themeManager::style(texture2D::create("assets/resources/cherry.png")));
	themeManager::add("cherryDenied", themeManager::style(texture2D::create("assets/resources/cherry.png"), 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
	themeManager::add("cherryApproved", themeManager::style(texture2D::create("assets/resources/cherry.png"), 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
	themeManager::add("frame", themeManager::style(texture2D::create("assets/resources/frame.png")));

	_playerGrid = std::make_unique<grid>(
		grid(10, 10, { 0.0f, 200.0f,0.5f }, { 40.0f,40.0f },
			{ std::initializer_list<std::string>({ "frame" , "cherry" }) })
	);
}
void gameLayer::onAttach()
{
	uint32_t btnId = _ui.push<button>(button("mainMenuBtn",
		{ 0.0f, 130.0f, 0.0f }, { 200.0f, 75.0f }, { "play", "playHover", "playClicked" }, true,
		OT_BIND_EVENT_FN(gameLayer::mainMenuBtn)));

	uint32_t btnship4Id = _ui.push<button>(button("ship4AddBtn",
		{ -100.0f, -200.0f, 0.0f }, { 200.0f, 75.0f }, { "play", "playHover", "playClicked" }, false,
		[](button*) {return false; }, OT_BIND_EVENT_FN(gameLayer::addShip4)
	));
	auto& btn = _ui.get(btnId);
	glm::vec2 sizeCounter(15.0f, 15.0f);
	uint32_t countId = _ui.push<counter>(counter("ship4Counter",
		{ btn.getPos().x + btn.getSize().x / 2 - sizeCounter.x,btn.getPos().y - btn.getSize().y / 2 + sizeCounter.y,btn.getPos().z - 1.0f }, sizeCounter, 4, 0,
		{ "frame" , "cherry","cherry","cherry","cherry" }
	));
	_ui.bind(countId, btnship4Id);

	_ui.bind(btnship4Id, btnId);
	_ui.setTypeCap(interactElement::elementType::ship1, 4);
	_ui.setTypeCap(interactElement::elementType::ship2, 4);
	_ui.setTypeCap(interactElement::elementType::ship3, 4);
	_ui.setTypeCap(interactElement::elementType::ship4, 4);
	mainMenu();
}

void gameLayer::onDetach()
{}

void gameLayer::onUpdate(timeStep ts)
{
	//render happens here
	rendererAPI::setClearColor({ 0, 0.6f, 0.6f, 1 });
	rendererAPI::clear();

	renderer2D::beginScene(cameraWrapper::getCamera());

	_ui.onRender();
	_playerGrid->onRender();
	renderer2D::endScene();
}

void gameLayer::onImGuiRender()
{
	_ui.onImGuiRender();
}

void gameLayer::onEvent(event& event)
{
	eventDispatcher dispatcher(event);
	dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(gameLayer::onWindowResize));


	_ui.onEvent(event);

	_playerGrid->onEvent(event);
}

bool gameLayer::onWindowResize(windowResizeEvent& event)
{
	cameraWrapper::updateCamera(event.getWidth(), event.getHeight());
	return false;
}
void gameLayer::mainMenu()
{
	//_ui.deactivateAllElements();
	//_ui.activateElements("mainMenuBtn");
}
bool gameLayer::mainMenuBtn(button* btn)
{
	_ui.deactivateAll();
	//_ui.activateElements("ship4AddBtn");
	auto& bind = _ui.getBindings(btn->getId());
	for (auto& item : bind) {
		_ui.activate(item);
	}
	return true;
}
bool gameLayer::addShip4(button* btn)
{
	counter& count = static_cast<counter&>(_ui.get(*_ui.getBindings(btn->getId()).begin()));
	auto [mouseX, mouseY] = input::getMousePos();
	auto pos = cameraWrapper::screenToWorld({ mouseX,  mouseY });

	if (!_ui.isTypeCapReached(interactElement::ship4)) {
		std::string name = ("ship4_");
		name.append(1, '1' + _ui.checkTypeCap(interactElement::ship4));

		uint32_t ship4Id = _ui.push<ship>(ship(name, { pos, 1.0f },
			4, { 40.0f,40.0f }, { { "cherry","cherryApproved","cherryDenied","frame" } },
			OT_BIND_EVENT_FN(gameLayer::placeShip), OT_BIND_EVENT_FN(gameLayer::gridCalculate)
		));
		_ui.bind(count.getId(), ship4Id);
	}
	else {
		uint32_t id = _ui.activateFirst(interactElement::ship4);
		if (id != (uint32_t) - 1) {
			auto& item = _ui.get(id);
			item.setPos({ pos, item.getPos().z });
		}
	}
	++count;

	return true;
}

bool gameLayer::gridCalculate(ship* ship)
{
	const glm::vec2& size = _playerGrid->getSize();
	const glm::vec3& gridPos = _playerGrid->getPos();
	glm::i32vec2 begin = round(glm::vec2((ship->getPos().x - gridPos.x) / size.x, (gridPos.y - ship->getPos().y) / size.y));

	glm::i32vec2 end = round(glm::vec2(((ship->end() - 1)->_pos.x - gridPos.x) / size.x, (gridPos.y - (ship->end() - 1)->_pos.y) / size.y));

	if (begin.x > -1 && begin.y > -1 && begin.x < (int)_playerGrid->getColumnCount() && begin.y < (int)_playerGrid->getRowCount() &&
		end.x > -1 && end.y > -1 && end.x < (int)_playerGrid->getColumnCount() && end.y < (int)_playerGrid->getRowCount()) {
		ship->setPlaced(true);

		ship->setPos({ gridPos.x + begin.x * size.x, gridPos.y - begin.y * size.y, ship->getPos().z });

		glm::i32vec2 areaB = begin - 1, areaE = end + 1;

		areaB.x = std::max(0, std::min(areaB.x, (int32_t)_playerGrid->getRowCount() - 1));
		areaB.y = std::max(0, std::min(areaB.y, (int32_t)_playerGrid->getColumnCount() - 1));
		areaE.x = std::max(0, std::min(areaE.x, (int32_t)_playerGrid->getRowCount() - 1));
		areaE.y = std::max(0, std::min(areaE.y, (int32_t)_playerGrid->getColumnCount() - 1));

		ship::shipCell::state state = ship::shipCell::state::placingAllowed;
		if (_playerGrid->isOccupied(areaB, areaE))
			state = ship::shipCell::state::placingDenied;

		ship->changeState(state);
		return true;
	}
	ship->changeState(ship::shipCell::state::normal);
	ship->setPlaced(false);
	return false;
}

bool gameLayer::placeShip(ship* ship)
{
	ship->changeState(ship::shipCell::state::normal);
	if (!ship->isPlaced()) {
		//ship->setVisibility(false);
		//ship->setActive(false);
		auto& bind = _ui.getBindings(ship->getId()).begin();
		counter& count = static_cast<counter&>(_ui.get(*_ui.getBindings(ship->getId()).begin()));
		--count;
		//_ui.unBind(ship->getId(), count.getId());
		_ui.deactivate(ship->getId());
	}
	return true;
}