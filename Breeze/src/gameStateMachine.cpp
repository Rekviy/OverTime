#include "gameStateMachine.h"

#include "cameraWrapper.h"
#include "ui/objectPool.h"
#include "ui/grid.h"
#include "ui/ship.h"
#include "ui/counter.h"
#include "ui/button.h"
#include "ui/maskLayer.h"
using namespace overtime;

gsm::gsm()
{
	_stateStack.push(gameState::unknown); _stateUI[gameState::unknown] = {};
	_ui = std::make_shared<gameUI>();
	_gridManager = gridManager(_ui);
	themeManager::add("play", style(texture2D::create("assets/resources/play.png"), 1.0f));
	themeManager::add("playHover", style(texture2D::create("assets/resources/playHover.png"), 1.0f));
	themeManager::add("playClicked", style(texture2D::create("assets/resources/playClicked.png"), 1.0f));

	themeManager::add("cherry", style(texture2D::create("assets/resources/cherry.png")));

	themeManager::add("frame", style(texture2D::create("assets/resources/frame.png")));
	themeManager::add("darkFrame", style(texture2D::create("assets/resources/frame.png"), 1.0f, { 0.1f,0.1f,0.1f,1.0f }));

	themeManager::add("five", style(texture2D::create("assets/resources/five.png")));
	themeManager::add("fiveDenied", style(texture2D::create("assets/resources/five.png"), 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
	themeManager::add("fiveApproved", style(texture2D::create("assets/resources/five.png"), 1.0f, { 0.1f,0.9f,0.1f,0.8f }));

	themeManager::add("four", style(texture2D::create("assets/resources/four.png")));
	themeManager::add("fourDenied", style(texture2D::create("assets/resources/four.png"), 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
	themeManager::add("fourApproved", style(texture2D::create("assets/resources/four.png"), 1.0f, { 0.1f,0.9f,0.1f,0.8f }));

	themeManager::add("three", style(texture2D::create("assets/resources/three.png")));
	themeManager::add("threeDenied", style(texture2D::create("assets/resources/three.png"), 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
	themeManager::add("threeApproved", style(texture2D::create("assets/resources/three.png"), 1.0f, { 0.1f,0.9f,0.1f,0.8f }));

	themeManager::add("two", style(texture2D::create("assets/resources/two.png")));
	themeManager::add("twoDenied", style(texture2D::create("assets/resources/two.png"), 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
	themeManager::add("twoApproved", style(texture2D::create("assets/resources/two.png"), 1.0f, { 0.1f,0.9f,0.1f,0.8f }));

	themeManager::add("one", style(texture2D::create("assets/resources/one.png")));
	themeManager::add("oneDenied", style(texture2D::create("assets/resources/one.png"), 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
	themeManager::add("oneApproved", style(texture2D::create("assets/resources/one.png"), 1.0f, { 0.1f,0.9f,0.1f,0.8f }));

	themeManager::add("playerWon", style(texture2D::create("assets/resources/cherry.png")));
	themeManager::add("enemyWon", style(texture2D::create("assets/resources/frame.png")));
#pragma region mainMenu
	uint32_t mainMenuBtnId = _ui->push<button>(button("mainMenuBtn",
		{ 0.0f, 130.0f, 0.0f }, { 200.0f, 75.0f }, { "play", "playHover", "playClicked" }, true,
		OT_BIND_EVENT_FN(gsm::mainMenuBtn)));
	_stateUI[gameState::mainMenu] = { mainMenuBtnId };
#pragma endregion

#pragma region gameplay
	_ui->setTypeCap(elementType::ship1Element, 4 * 2); _ui->setTypeActiveCap(elementType::ship1Element, _ui->getTypeCap(elementType::ship1Element) / 2);
	_ui->setTypeCap(elementType::ship2Element, 3 * 2); _ui->setTypeActiveCap(elementType::ship2Element, _ui->getTypeCap(elementType::ship2Element) / 2);
	_ui->setTypeCap(elementType::ship3Element, 2 * 2); _ui->setTypeActiveCap(elementType::ship3Element, _ui->getTypeCap(elementType::ship3Element) / 2);
	_ui->setTypeCap(elementType::ship4Element, 1 * 2); _ui->setTypeActiveCap(elementType::ship4Element, _ui->getTypeCap(elementType::ship4Element) / 2);

	_gridManager.setShipRequirement(_ui->getTypeActiveCap(elementType::ship1Element) + _ui->getTypeActiveCap(elementType::ship2Element)
		+ _ui->getTypeActiveCap(elementType::ship3Element) + _ui->getTypeActiveCap(elementType::ship4Element));

	_gridManager.setPlayerGrid(
		_ui->push<grid>(grid("playerGrid", 10, 10, { -620.0f, 240.0f,0.5f }, { 40.0f,40.0f }, { {"frame" , "cherry"} }))
	);
	uint32_t playerGridId = _gridManager.getPlayerGrid();

	_stateUI[gameState::gameplay] = {
		playerGridId
	};
#pragma endregion

#pragma region planning
	glm::vec3 addBtnPos = { -100.0f, -200.0f, 0.0f };
	glm::vec2 addBntSize = { 200.0f, 75.0f };

	uint32_t btnAutoPlaceId = _ui->push<button>(button("autoPlace", { -200.0f,-200.0f,0.7f }, { 100.0f,100.0f }, { "one","two","three" }, false,
		[this](button* btn)->bool {_gridManager.autoPlace(_gridManager.getPlayerGrid()); return true; }));

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
			uint32_t id = _gridManager.createPlayerShip((elementType)type);
			switch (type) {
				case ship1Element: _ui->bind(count1Id, id); break;
				case ship2Element: _ui->bind(count2Id, id); break;
				case ship3Element: _ui->bind(count3Id, id); break;
				case ship4Element: _ui->bind(count4Id, id); break;
			}
		}
	}

	_stateUI[gameState::planning] = {
		btnAutoPlaceId,
		btnship4Id, count4Id,
		btnship3Id, count3Id,
		btnship2Id, count2Id,
		btnship1Id, count1Id,
		finishPlanningId
	};


#pragma endregion

#pragma region playing
	_gridManager.setEnemyGrid(
		_ui->push<grid>(grid("enemyGrid", 10, 10, { 260.0f, 240.0f,0.5f }, { 40.0f,40.0f }, { {"frame" , "cherry"} }))
	);



	auto& enemyGrid = _ui->get<grid>(_gridManager.getEnemyGrid());
	auto& enemyGridPos = enemyGrid.getPos();
	uint32_t maskId = _ui->push<maskLayer>(maskLayer("enemyMask", enemyGrid.getRowCount(), enemyGrid.getColumnCount(), { enemyGridPos.x, enemyGridPos.y,enemyGridPos.z + 0.2f }, enemyGrid.getSize(),
		{ "darkFrame" },
		[this](maskLayer* mask) {
		uint32_t cellAttacked = mask->getCellClicked(), maskId = mask->getId(), rowCount = mask->getRowCount();
		_isPlayerMove = (bool)_gridManager.attack(*_ui->getParents(maskId).begin(), maskId, cellAttacked % rowCount, cellAttacked / rowCount);
		if (_gridManager.isAllDestroyed(_gridManager.getEnemyGrid())) {
			_winningTitle = themeManager::getStyle("playerWon");
			pushState(gameState::gameOver);
		}
		return true;
	}
	));

	_ui->bind(maskId, enemyGrid.getId());

	uint32_t autoAttackBtn = _ui->push<button>(button("autoAttackBtn",
		{ 0.0f, 320.0f, 0.0f }, { 200.0f, 75.0f }, { "play", "playHover", "playClicked" }, true,
		[this](button* mask) {
		_isPlayerMove = (bool)_gridManager.autoAttack(_gridManager.getEnemyGrid(), *_ui->getBindings(_gridManager.getEnemyGrid()).begin());
		if (_gridManager.isAllDestroyed(_gridManager.getEnemyGrid())) {
			_winningTitle = themeManager::getStyle("playerWon");
			pushState(gameState::gameOver);
		}
		return true;
	}
	));

	for (int type = elementType::ship4Element; type >= elementType::ship1Element; type--)
		while (!_ui->isTypeCapReached((elementType)type))
			_gridManager.createShip(_gridManager.getEnemyGrid(), (elementType)type);

	_stateUI[gameState::playing] = {
		enemyGrid.getId(),
		maskId,
		autoAttackBtn
	};
#pragma endregion

#pragma region pause
	uint32_t btnRetryId = _ui->push<button>(button("retry", { 0.0f,0.0f,0.7f }, { 100.0f,100.0f }, { "play", "playHover", "playClicked" }, false,
		OT_BIND_EVENT_FN(gsm::resetBtn)
	));
	uint32_t btnToMainMenuId = _ui->push<button>(button("toMainMenu", { -100.0f,0.0f,0.7f }, { 100.0f,100.0f }, { "one","two","three" }, false,
		OT_BIND_EVENT_FN(gsm::toMainMenuBtn)
	));
	_stateUI[gameState::pause] = {
		btnRetryId,btnToMainMenuId
	};
#pragma endregion

#pragma region gameOver

	_stateUI[gameState::gameOver] = {
		btnRetryId,btnToMainMenuId
	};
#pragma endregion
	_ui->deactivateAll();
	enterState(gameState::mainMenu);
}

gsm::~gsm()
{}

int gsm::init()
{
	return 0;
}

int gsm::shutdown()
{
	return 0;
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
			for (auto itemId : _ui->get<grid>(_gridManager.getEnemyGrid()).getAllItems()) {
				_ui->get(itemId).setFlag(elementFlags::blocked);
			}

			_ui->setTypeActiveCap(elementType::ship1Element, _ui->getTypeActiveCap(elementType::ship1Element) * 2);
			_ui->setTypeActiveCap(elementType::ship2Element, _ui->getTypeActiveCap(elementType::ship2Element) * 2);
			_ui->setTypeActiveCap(elementType::ship3Element, _ui->getTypeActiveCap(elementType::ship3Element) * 2);
			_ui->setTypeActiveCap(elementType::ship4Element, _ui->getTypeActiveCap(elementType::ship4Element) * 2);


			_gridManager.autoPlace(_gridManager.getEnemyGrid());

			for (auto itemId : _ui->get<grid>(_gridManager.getEnemyGrid()).getAllItems()) {
				_ui->get(itemId).setFlag(elementFlags::blocked);
			}
			break;
		case gameState::pause:
			break;
		case gameState::gameOver:
			break;

	}
}

void gsm::onExit(gameState state)
{
	switch (state) {
		case gameState::mainMenu:
			break;
		case gameState::gameplay:
			break;
		case gameState::planning:
			break;
		case gameState::playing:
			for (auto itemId : _ui->get<grid>(_gridManager.getEnemyGrid()).getAllItems()) {
				_ui->get(itemId).dropFlag(elementFlags::blocked);
			}
			_ui->setTypeActiveCap(elementType::ship1Element, _ui->getTypeCap(elementType::ship1Element) / 2);
			_ui->setTypeActiveCap(elementType::ship2Element, _ui->getTypeCap(elementType::ship2Element) / 2);
			_ui->setTypeActiveCap(elementType::ship3Element, _ui->getTypeCap(elementType::ship3Element) / 2);
			_ui->setTypeActiveCap(elementType::ship4Element, _ui->getTypeCap(elementType::ship4Element) / 2);
			break;
		case gameState::pause:
			break;
		case gameState::gameOver:
			break;

	}

}

void gsm::onUpdate(overtime::timeStep ts)
{
	switch (_stateStack.top()) {

		case gameState::playing:
			if (!_isPlayerMove) {
				auto* mask = *_ui->get<maskLayer>(_ui->getBindings(_gridManager.getEnemyGrid())).begin();
				mask->setFlag(elementFlags::blocked);
				//todo there will be autoAttack button deactivation
				_isPlayerMove = !_gridManager.autoAttack(_gridManager.getPlayerGrid(), -1);
				if (_gridManager.isAllDestroyed(_gridManager.getPlayerGrid())) {
					pushState(gameState::gameOver);
					_winningTitle = themeManager::getStyle("enemyWon");
				}
			}
			else {
				auto* mask = *_ui->get<maskLayer>(_ui->getBindings(_gridManager.getEnemyGrid())).begin();
				mask->dropFlag(elementFlags::blocked);

			}
	}

}

void gsm::onRender()
{
	_ui->onRender();
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
			renderer2D::drawSquad({ 0.0f,0.0f,0.9f }, { application::getInst().getWindow().getWidth(),application::getInst().getWindow().getHeight() }, { 0.7f,0.7f,0.7f,0.5f });
			renderer2D::drawSquad({ 0.0f,0.0f,1.0f }, { 200.0f, 50.0f }, _winningTitle->_color, _winningTitle->_texture, _winningTitle->_textureSize);
			break;
		default:
			break;
	}
}

void gsm::onImGuiRender()
{
	ImGui::Begin("gsm menu");
	ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);

	const char* stateStr;
	switch (_stateStack.top()) {
		case gameState::mainMenu:
			stateStr = "mainMenu";
			break;
		case gameState::pause:
			stateStr = "pause";
			break;
		case gameState::planning:
			stateStr = "planning";
			break;
		case gameState::playing:
			stateStr = "playing";
			break;
		case gameState::gameplay:
			stateStr = "gameplay";
			break;
		case gameState::gameOver:
			stateStr = "gameOver";
			break;
		default:
			stateStr = "error";
			break;
	}
	ImGui::Text("Current state: %s", stateStr);

	static auto& playerGrid = _ui->get<grid>(_gridManager.getPlayerGrid());
	if (ImGui::CollapsingHeader("Player Grid Info", ImGuiTreeNodeFlags_None)) {
		ImGui::Text("Columns:%d rows:%d", playerGrid.getColumnCount(), playerGrid.getRowCount());
		for (auto& item : playerGrid.getAllItems()) {
			auto& placement = playerGrid.getPlacement(item);
			ImGui::Text("ShipId:%d; Positions on grid: x1:%d y1:%d; x2:%d y2:%d;", item, placement.first.x, placement.first.y, placement.second.x, placement.second.y);
		}
	}
	static auto& enemyGrid = _ui->get<grid>(_gridManager.getEnemyGrid());
	if (ImGui::CollapsingHeader("Enemy Grid Info", ImGuiTreeNodeFlags_None)) {
		ImGui::Text("Columns:%d rows:%d", enemyGrid.getColumnCount(), enemyGrid.getRowCount());
		for (auto& item : enemyGrid.getAllItems()) {
			auto& placement = enemyGrid.getPlacement(item);
			ImGui::Text("ShipId:%d; Positions on grid: x1:%d y1:%d; x2:%d y2:%d;", item, placement.first.x, placement.first.y, placement.second.x, placement.second.y);
		}
	}

	ImGui::NewLine();
	ImGui::PopItemFlag();
	ImGui::End();
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

//todo replace enter state and reseting with push and etc. to implement game continuation
bool gsm::toMainMenuBtn(button* btn)
{
	if (_stateStack.top() == gameState::pause)
		popState();
	popState();

	reset();

	enterState(gameState::mainMenu);
	return true;
}

bool gsm::resetBtn(button* btn)
{
	if (_stateStack.top() == gameState::pause)
		popState();

	reset();
	_ui->activate(_gridManager.getPlayerGrid());
	enterState(gameState::planning);
	return true;
}

void gsm::reset()
{
	uint32_t playerGridId = _gridManager.getPlayerGrid();
	uint32_t enemyGridId = _gridManager.getEnemyGrid();

	(*_ui->get<maskLayer>(_ui->getBindings(enemyGridId)).begin())->reset();
	auto counters = _ui->get<counter>(_stateUI[gameState::planning]);
	for (auto count : counters)
		count->reset();

	_gridManager.resetShips(_ui->get<grid>(enemyGridId).getAllItems());
	_gridManager.resetShips(_ui->get<grid>(playerGridId).getAllItems());

	_gridManager.resetGrid(enemyGridId);
	_gridManager.resetGrid(playerGridId);
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





