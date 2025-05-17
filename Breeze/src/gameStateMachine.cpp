#include "gameStateMachine.h"

#include "cameraWrapper.h"
#include "ui/objectPool.h"
#include "ui/grid.h"
#include "ui/ship.h"
#include "ui/counter.h"
#include "ui/button.h"
using namespace overtime;
gsm::gsm()
{
	themeManager::add("play", themeManager::style(texture2D::create("assets/resources/play.png"), 1.0f));
	themeManager::add("playHover", themeManager::style(texture2D::create("assets/resources/playHover.png"), 1.0f));
	themeManager::add("playClicked", themeManager::style(texture2D::create("assets/resources/playClicked.png"), 1.0f));

	themeManager::add("cherry", themeManager::style(texture2D::create("assets/resources/cherry.png")));
	themeManager::add("cherryDenied", themeManager::style(texture2D::create("assets/resources/cherry.png"), 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
	themeManager::add("cherryApproved", themeManager::style(texture2D::create("assets/resources/cherry.png"), 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
	themeManager::add("frame", themeManager::style(texture2D::create("assets/resources/frame.png")));

	_playerGridId = _ui.push<grid>(grid("playerGrid", 10, 10, { 0.0f, 200.0f,0.5f }, { 40.0f,40.0f }, { { "frame" , "cherry" } }));
	uint32_t mainMenuBtnId = _ui.push<button>(button("mainMenuBtn",
		{ 0.0f, 130.0f, 0.0f }, { 200.0f, 75.0f }, { "play", "playHover", "playClicked" }, true,
		OT_BIND_EVENT_FN(gsm::mainMenuBtn)));

	uint32_t btnship4Id = _ui.push<button>(button("ship4AddBtn",
		{ -100.0f, -200.0f, 0.0f }, { 200.0f, 75.0f }, { "play", "playHover", "playClicked" }, false,
		[](button*) {return false; }, OT_BIND_EVENT_FN(gsm::addShip4)
	));
	auto& btn = _ui.get(mainMenuBtnId);
	glm::vec2 sizeCounter(15.0f, 15.0f);
	uint32_t countId = _ui.push<counter>(counter("ship4Counter",
		{ btn.getPos().x + btn.getSize().x / 2 - sizeCounter.x,btn.getPos().y - btn.getSize().y / 2 + sizeCounter.y,btn.getPos().z - 1.0f }, sizeCounter, 4, 0,
		{ "frame" , "cherry","cherry","cherry","cherry" }
	));
	_ui.bind(countId, btnship4Id);

	_ui.bind(btnship4Id, mainMenuBtnId);
	_ui.setTypeCap(elementType::ship1Element, 4);
	_ui.setTypeCap(elementType::ship2Element, 4);
	_ui.setTypeCap(elementType::ship3Element, 4);
	_ui.setTypeCap(elementType::ship4Element, 4);
	_stateUI[gameState::mainMenu] = { mainMenuBtnId };
	_stateUI[gameState::planning] = { btnship4Id, countId, _playerGridId };
	enterState(gameState::mainMenu);
}

void gsm::enterState(gameState newState)
{
	if (_currentState != newState) {
		_ui.deactivateAll();
		onExit(_currentState);
		_currentState = newState;
		onEnter(_currentState);
		for (auto& item : _stateUI.at(_currentState))
			_ui.activate(item);
	}

}

void gsm::onEnter(gameState state)
{}

void gsm::onExit(gameState state)
{}

void gsm::onUpdate(overtime::timeStep ts)
{}

void gsm::onRender()
{
	_ui.onRender();
}

void gsm::onImGuiRender()
{
	_ui.onImGuiRender();
}

void gsm::onEvent(overtime::event& event)
{
	_ui.onEvent(event);
}



bool gsm::mainMenuBtn(button* btn)
{
	enterState(gameState::planning);
	return true;
}
bool gsm::addShip4(button* btn)
{
	counter& count = (_ui.get<counter>(*_ui.getBindings(btn->getId()).begin()));
	auto [mouseX, mouseY] = input::getMousePos();
	auto pos = cameraWrapper::screenToWorld({ mouseX,  mouseY });

	if (!_ui.isTypeCapReached(elementType::ship4Element)) {
		std::string name = ("ship4_");
		name.append(1, '1' + _ui.checkTypeCap(elementType::ship4Element));

		uint32_t ship4Id = _ui.push<ship4>(ship4(name, { pos, 1.0f },
			{ 40.0f,40.0f }, { { "cherry","cherryApproved","cherryDenied","frame" } },
			OT_BIND_EVENT_FN(gsm::removeShip), OT_BIND_EVENT_FN(gsm::placeShip), OT_BIND_EVENT_FN(gsm::gridCalculate)
		));
		_ui.bind(count.getId(), ship4Id);
	}
	else {
		uint32_t id = _ui.activateFirst(elementType::ship4Element);
		if (id != (uint32_t)-1) {
			auto& item = _ui.get(id);
			item.setPos({ pos, item.getPos().z });
		}
	}
	++count;

	return true;
}

bool gsm::gridCalculate(ship* ship)
{
	auto& playerGrid = _ui.get<grid>(_playerGridId);
	const glm::vec2& size = playerGrid.getSize();
	const glm::vec3& gridPos = playerGrid.getPos();
	glm::i32vec2 begin = round(glm::vec2((ship->getPos().x - gridPos.x) / size.x, (gridPos.y - ship->getPos().y) / size.y));

	glm::i32vec2 end = round(glm::vec2(((ship->end() - 1)->_pos.x - gridPos.x) / size.x, (gridPos.y - (ship->end() - 1)->_pos.y) / size.y));

	if (begin.x > -1 && begin.y > -1 && begin.x < (int)playerGrid.getColumnCount() && begin.y < (int)playerGrid.getRowCount() &&
		end.x > -1 && end.y > -1 && end.x < (int)playerGrid.getColumnCount() && end.y < (int)playerGrid.getRowCount()) {
		ship->setPlaced(true);

		ship->setPos({ gridPos.x + begin.x * size.x, gridPos.y - begin.y * size.y, ship->getPos().z });

		glm::i32vec2 areaB = begin - 1, areaE = end + 1;

		areaB.x = std::max(0, std::min(areaB.x, (int32_t)playerGrid.getRowCount() - 1));
		areaB.y = std::max(0, std::min(areaB.y, (int32_t)playerGrid.getColumnCount() - 1));
		areaE.x = std::max(0, std::min(areaE.x, (int32_t)playerGrid.getRowCount() - 1));
		areaE.y = std::max(0, std::min(areaE.y, (int32_t)playerGrid.getColumnCount() - 1));

		ship::shipCell::state state = ship::shipCell::state::placingAllowed;
		if (playerGrid.isOccupied(areaB, areaE))
			state = ship::shipCell::state::placingDenied;

		ship->changeState(state);
		return true;
	}
	ship->changeState(ship::shipCell::state::normal);
	ship->setPlaced(false);
	return false;
}

bool gsm::placeShip(ship* ship)
{
	if (!ship->isPlaced() || ship->getState() == ship::shipCell::placingDenied) {
		auto& bind = _ui.getBindings(ship->getId()).begin();
		auto& count = _ui.get<counter>(*_ui.getBindings(ship->getId()).begin());
		--count;
		_ui.deactivate(ship->getId());
	}
	else {
		auto& playerGrid = _ui.get<grid>(_playerGridId);
		const glm::vec2& size = playerGrid.getSize();
		const glm::vec3& gridPos = playerGrid.getPos();
		glm::i32vec2 begin = round(glm::vec2((ship->getPos().x - gridPos.x) / size.x, (gridPos.y - ship->getPos().y) / size.y));

		glm::i32vec2 end = round(glm::vec2(((ship->end() - 1)->_pos.x - gridPos.x) / size.x, (gridPos.y - (ship->end() - 1)->_pos.y) / size.y));
		playerGrid.setOccupation(begin, end, true);
	}
	ship->changeState(ship::shipCell::state::normal);
	return true;
}

bool gsm::removeShip(ship* ship)
{


	return true;
}


