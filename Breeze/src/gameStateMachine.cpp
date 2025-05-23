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
	_stateStack.push(gameState::unknown); _stateUI[gameState::unknown] = {};
	_ui = std::make_shared<gameUI>();
	_gridManager = gridManager(_ui);
	themeManager::add("play", themeManager::style(texture2D::create("assets/resources/play.png"), 1.0f));
	themeManager::add("playHover", themeManager::style(texture2D::create("assets/resources/playHover.png"), 1.0f));
	themeManager::add("playClicked", themeManager::style(texture2D::create("assets/resources/playClicked.png"), 1.0f));

	themeManager::add("cherry", themeManager::style(texture2D::create("assets/resources/cherry.png")));
	themeManager::add("cherryDenied", themeManager::style(texture2D::create("assets/resources/cherry.png"), 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
	themeManager::add("cherryApproved", themeManager::style(texture2D::create("assets/resources/cherry.png"), 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
	themeManager::add("frame", themeManager::style(texture2D::create("assets/resources/frame.png")));

	themeManager::add("five", themeManager::style(texture2D::create("assets/resources/five.png")));
	themeManager::add("four", themeManager::style(texture2D::create("assets/resources/four.png")));
	themeManager::add("three", themeManager::style(texture2D::create("assets/resources/three.png")));
	themeManager::add("two", themeManager::style(texture2D::create("assets/resources/two.png")));
	themeManager::add("one", themeManager::style(texture2D::create("assets/resources/one.png")));


#pragma region mainMenu
	uint32_t mainMenuBtnId = _ui->push<button>(button("mainMenuBtn",
		{ 0.0f, 130.0f, 0.0f }, { 200.0f, 75.0f }, { "play", "playHover", "playClicked" }, true,
		OT_BIND_EVENT_FN(gsm::mainMenuBtn)));
	_stateUI[gameState::mainMenu] = { mainMenuBtnId };
#pragma endregion

#pragma region gameplay
	_ui->setTypeCap(elementType::ship1Element, 4);
	_ui->setTypeCap(elementType::ship2Element, 3);
	_ui->setTypeCap(elementType::ship3Element, 2);
	_ui->setTypeCap(elementType::ship4Element, 1);
	_gridManager.setShipRequirement(_ui->getTypeActiveCap(elementType::ship1Element) + _ui->getTypeActiveCap(elementType::ship2Element)
		+ _ui->getTypeActiveCap(elementType::ship3Element) + _ui->getTypeActiveCap(elementType::ship4Element));
	_gridManager.setPlayerGrid(
		_ui->push<grid>(grid("playerGrid", 10, 10, { 0.0f, 240.0f,0.5f }, { 40.0f,40.0f }, { {"frame" , "cherry"} }))
	);
	uint32_t playerGridId = _gridManager.getPlayerGrid();
	uint32_t btnAuto = _ui->push<button>(button("auto", { -200.0f,-200.0f,0.7f }, { 100.0f,100.0f }, { "one","two","three" }, false,
		[this](button* btn)->bool {_gridManager.autoPlace(_gridManager.getPlayerGrid()); return true; }));
	_stateUI[gameState::gameplay] = {
		btnAuto,
		playerGridId
	};
#pragma endregion

#pragma region planning
	glm::vec3 addBtnPos = { -100.0f, -200.0f, 0.0f };
	glm::vec2 addBntSize = { 200.0f, 75.0f };

	uint32_t btnship4Id = _ui->push<button>(button("ship4AddBtn",
		{ addBtnPos.x, addBtnPos.y, addBtnPos.z }, addBntSize, { "play", "playHover", "playClicked" }, false,
		[](button*) {return false; }, [this](button* btn) { return addShip(btn, elementType::ship4Element); }
	));

	uint32_t count4Id = _ui->push<counter>(counter("ship4Counter",
		{ addBtnPos.x + addBntSize.x / 2,addBtnPos.y + addBntSize.y / 2,addBtnPos.z + 0.1f }, { 15.0f, 15.0f }, 4, 0,
		{ "four","three","two","one" }
	));
	_ui->bind(count4Id, btnship4Id);

	addBtnPos.x += addBntSize.x / 2 + 10.0f;

	uint32_t btnship3Id = _ui->push<button>(button("ship3AddBtn",
		{ addBtnPos.x, addBtnPos.y, addBtnPos.z }, addBntSize, { "play", "playHover", "playClicked" }, false,
		[](button*) {return false; }, [this](button* btn) { return addShip(btn, elementType::ship3Element); }
	));
	uint32_t count3Id = _ui->push<counter>(counter("ship3Counter",
		{ addBtnPos.x + addBntSize.x / 2,addBtnPos.y + addBntSize.y / 2,addBtnPos.z + 0.1f }, { 15.0f, 15.0f }, 4, 0,
		{ "four","three","two","one" }
	));
	_ui->bind(count3Id, btnship3Id);

	addBtnPos.x += addBntSize.x / 2 + 10.0f;

	uint32_t btnship2Id = _ui->push<button>(button("ship2AddBtn",
		{ addBtnPos.x, addBtnPos.y, addBtnPos.z }, addBntSize, { "play", "playHover", "playClicked" }, false,
		[](button*) {return false; }, [this](button* btn) { return addShip(btn, elementType::ship2Element); }
	));
	uint32_t count2Id = _ui->push<counter>(counter("ship2Counter",
		{ addBtnPos.x + addBntSize.x / 2,addBtnPos.y + addBntSize.y / 2,addBtnPos.z + 0.1f }, { 15.0f, 15.0f }, 4, 0,
		{ "four","three","two","one" }
	));
	_ui->bind(count2Id, btnship2Id);

	addBtnPos.x += addBntSize.x / 2 + 10.0f;

	uint32_t btnship1Id = _ui->push<button>(button("ship1AddBtn",
		{ addBtnPos.x, addBtnPos.y, addBtnPos.z }, addBntSize, { "play", "playHover", "playClicked" }, false,
		[](button*) {return false; }, [this](button* btn) { return addShip(btn, elementType::ship1Element); }
	));
	uint32_t count1Id = _ui->push<counter>(counter("ship1Counter",
		{ addBtnPos.x + addBntSize.x / 2,addBtnPos.y + addBntSize.y / 2,addBtnPos.z + 0.1f }, { 15.0f, 15.0f }, 4, 0,
		{ "five","four","three","two","one" }
	));
	_ui->bind(count1Id, btnship1Id);

	uint32_t finishPlanningId = _ui->push<button>(button("finishPlanningBtn",
		{ 0.0f, 300.0f, 1.0f }, { 200.0f, 75.0f }, { "play", "playHover", "playClicked" }, false,
		OT_BIND_EVENT_FN(gsm::finishPlanningBtn)
	));

	for (int type = elementType::ship4Element; type >= elementType::ship1Element; type--) {
		while (!_ui->isTypeCapReached((elementType)type)) {
			uint32_t id = createPlayerShip((elementType)type);
			switch (type) {
				case ship1Element: _ui->bind(count1Id, id); break;
				case ship2Element: _ui->bind(count2Id, id); break;
				case ship3Element: _ui->bind(count3Id, id); break;
				case ship4Element: _ui->bind(count4Id, id); break;
			}
		}
	}

	_stateUI[gameState::planning] = {
		btnship4Id, count4Id,
		btnship3Id, count3Id,
		btnship2Id, count2Id,
		btnship1Id, count1Id,
		finishPlanningId
	};


#pragma endregion

#pragma region playing
	_stateUI[gameState::playing] = {

	};
#pragma endregion
#pragma region pause
	_stateUI[gameState::pause] = {

	};
#pragma endregion

	_ui->deactivateAll();
	enterState(gameState::mainMenu);
}

void gsm::enterState(gameState newState)
{
	if (_stateStack.top() != newState) {
		_ui->deactivate(_stateUI.at(_stateStack.top()));
		onExit(_stateStack.top());

		_stateStack.pop();

		_stateStack.push(newState);
		onEnter(_stateStack.top());

		_ui->activate(_stateUI.at(_stateStack.top()));

	}

}

void gsm::pushState(gameState state)
{
	if (_stateStack.top() != state) {
		_stateStack.push(state);
		onEnter(_stateStack.top());

		_ui->activate(_stateUI.at(_stateStack.top()));
	}
}

void gsm::popState()
{
	_ui->deactivate(_stateUI.at(_stateStack.top()));
	onExit(_stateStack.top());

	_stateStack.pop();
}

void gsm::onEnter(gameState state)
{
	switch (state) {
		case gameState::mainMenu:
			break;
		case gameState::gameplay:
			break;
		case gameState::planning:
			break;
		case gameState::playing:

			break;
		case gameState::pause:
			break;
		case gameState::gameOver:
			break;

	}
}

void gsm::onExit(gameState state)
{}

void gsm::onUpdate(overtime::timeStep ts)
{}

void gsm::onRender()
{
	switch (_stateStack.top()) {
		case gameState::mainMenu:
			renderer2D::drawSquad({ 0.0f,0.0f,-0.9f }, { application::getInst().getWindow().getWidth(),application::getInst().getWindow().getHeight() }, { 0.5f,0.5f,0.1f,1.0f });
			break;
		case gameState::pause:
			renderer2D::drawSquad({ 0.0f,0.0f,1.0f }, { application::getInst().getWindow().getWidth(),application::getInst().getWindow().getHeight() }, { 0.7f,0.7f,0.7f,0.5f });
			break;
		case gameState::planning:
			break;
		case gameState::gameplay:
			break;
		case gameState::gameOver:
			break;
		default:
			break;
	}
	_ui->onRender();
}

void gsm::onImGuiRender()
{
	_ui->onImGuiRender();
}

void gsm::onEvent(overtime::event& event)
{
	_ui->onEvent(event);
	eventDispatcher dispatcher(event);
	switch (_stateStack.top()) {
		case gameState::planning:
		case gameState::playing:
		case gameState::pause:
			dispatcher.dispatch<keyReleasedEvent>([this](keyReleasedEvent& event)->bool {
				if (event.getKeyCode() == keyCodes::OT_KEY_ESCAPE)
					(_stateStack.top() == gameState::pause) ? popState() : pushState(gameState::pause);
				return false;
			});
			break;
	}
}

bool gsm::mainMenuBtn(button* btn)
{
	enterState(gameState::gameplay);
	pushState(gameState::planning);
	return true;
}
bool gsm::finishPlanningBtn(button* btn)
{
	if (_gridManager.checkRequirement())
		enterState(gameState::playing);
	return true;
}

bool gsm::addShip(button* btn, elementType shipType)
{
	counter& count = (_ui->get<counter>(*_ui->getBindings(btn->getId()).begin()));
	auto [mouseX, mouseY] = input::getMousePos();
	auto pos = cameraWrapper::screenToWorld({ mouseX,  mouseY });
	if (!_ui->isTypeActiveCapReached(shipType)) {
		uint32_t id = _ui->activateFirst(shipType);
		auto& item = _ui->get(id);
		item.setPos({ pos, _ui->get(_gridManager.getPlayerGrid()).getPos().z });
		++count;
	}

	return true;
}

uint32_t gsm::createPlayerShip(elementType shipType)
{
	auto& curGrid = _ui->get<grid>(_gridManager.getPlayerGrid());
	const glm::vec2& size = curGrid.getSize();
	const glm::vec3& gridPos = curGrid.getPos();

	uint32_t shipId = -1;
	std::string name = (curGrid.getName());
	switch (shipType) {
		case ship1Element:
			name.append("ship1_");
			name.append(1, '1' + _ui->checkTypeCap(shipType));
			shipId = _ui->push<ship1>(ship1(name, gridPos,
				size, { { "cherry","cherryApproved","cherryDenied","frame" } },
				[this](ship* ship) {return _gridManager.removeShip(ship); },
				[this](ship* ship) {return _gridManager.placeShip(ship); },
				[this](ship* ship) {return _gridManager.gridCalculate(ship); }
			));
			break;
		case ship2Element:
			name.append("ship2_");
			name.append(1, '1' + _ui->checkTypeCap(shipType));
			shipId = _ui->push<ship2>(ship2(name, gridPos,
				size, { { "cherry","cherryApproved","cherryDenied","frame" } },
				[this](ship* ship) {return _gridManager.removeShip(ship); },
				[this](ship* ship) {return _gridManager.placeShip(ship); },
				[this](ship* ship) {return _gridManager.gridCalculate(ship); }
			));
			break;
		case ship3Element:
			name.append("ship3_");
			name.append(1, '1' + _ui->checkTypeCap(shipType));
			shipId = _ui->push<ship3>(ship3(name, gridPos,
				size, { { "cherry","cherryApproved","cherryDenied","frame" } },
				[this](ship* ship) {return _gridManager.removeShip(ship); },
				[this](ship* ship) {return _gridManager.placeShip(ship); },
				[this](ship* ship) {return _gridManager.gridCalculate(ship); }
			));
			break;
		case ship4Element:
			name.append("ship4_");
			name.append(1, '1' + _ui->checkTypeCap(shipType));
			shipId = _ui->push<ship4>(ship4(name, gridPos,
				size, { { "cherry","cherryApproved","cherryDenied","frame" } },
				[this](ship* ship) {return _gridManager.removeShip(ship); },
				[this](ship* ship) {return _gridManager.placeShip(ship); },
				[this](ship* ship) {return _gridManager.gridCalculate(ship); }
			));
			break;
	}
	return shipId;
}




