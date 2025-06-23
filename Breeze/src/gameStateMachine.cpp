#include "gameStateMachine.h"

#include "cameraWrapper.h"
#include "ui/objectPool.h"
#include "ui/grid.h"
#include "ui/ship.h"
#include "ui/counter.h"
#include "ui/button.h"
#include "ui/maskLayer.h"
namespace breeze {
#define SHIP4_AMOUNT 1
#define SHIP3_AMOUNT 2
#define SHIP2_AMOUNT 3
#define SHIP1_AMOUNT 4

#define PLAYER_GRID_START_POS glm::vec3(-620.0f, 240.0f, 0.0f)
#define ENEMY_GRID_START_POS glm::vec3(170.0f, 240.0f, 0.0f)
#define GRID_CELL_SIZE { 50.0f,50.0f }
#define COLUMN_COUNT 10
#define ROW_COUNT 10

#define MAIN_MENU_START_POS glm::vec3(0.0f, 130.0f, 0.0f)
#define MAIN_MENU_SIZE glm::vec2(200.0f, 100.0f)

#define ADD_SHIP_START_POS glm::vec3(80.0f, 140.0f, 0.1f)
#define ADD_SHIP_SIZE glm::vec2(200.0f, 100.0f)
#define COUNT_SHIP_SIZE glm::vec2(15.0f, 15.0f)

#define AUTO_PLACE_START_POS glm::vec3(-80.0f, 170.0f,0.1f)
#define AUTO_PLACE_SIZE glm::vec2(50.0f,50.0f)

#define FINISH_PLANING_START_POS glm::vec3(0.0f, 300.0f, 0.1f)
#define FINISH_PLANING_SIZE glm::vec2(200.0f, 100.0f)

#define AUTO_ATTACK_START_POS glm::vec3(0.0f, 320.0f, 0.1f)
#define AUTO_ATTACK_SIZE glm::vec2(50.0f, 50.0f)

#define TO_MAIN_MENU_START_POS glm::vec3(0.0f,0.0f, 1.0f)
#define TO_MAIN_MENU_SIZE glm::vec2(300.0f, 170.0f)

#define RETRY_START_POS glm::vec3(0.0f, TO_MAIN_MENU_SIZE.y + 10.0f, 1.0f)
#define RETRY_ATTACK_SIZE glm::vec2(300.0f, 170.0f)

	using namespace overtime;

	gsm::gsm()
	{
		_stateStack.push(gameState::unknown); _stateUI[gameState::unknown] = {};
		styleInit();
	#pragma region mainMenu
		uint32_t mainMenuBtnId = _ui->push<button>(button("mainMenuBtn",
			MAIN_MENU_START_POS, MAIN_MENU_SIZE, { "play", "playHover", "playClicked" },
			OT_BIND_EVENT_FN(gsm::mainMenuBtn)));
		_stateUI[gameState::mainMenu] = { mainMenuBtnId };
	#pragma endregion

	#pragma region gameplay
		_ui->setTypeCap(elementType::ship1Element, SHIP1_AMOUNT);
		_ui->setTypeCap(elementType::ship2Element, SHIP2_AMOUNT);
		_ui->setTypeCap(elementType::ship3Element, SHIP3_AMOUNT);
		_ui->setTypeCap(elementType::ship4Element, SHIP4_AMOUNT);

		_gridManager.setShipRequirement(_ui->getTypeActiveCap(elementType::ship1Element) + _ui->getTypeActiveCap(elementType::ship2Element)
			+ _ui->getTypeActiveCap(elementType::ship3Element) + _ui->getTypeActiveCap(elementType::ship4Element));

		_gridManager.setPlayerGrid(
			_ui->push<grid>(grid("playerGrid", ROW_COUNT, COLUMN_COUNT, PLAYER_GRID_START_POS, GRID_CELL_SIZE, { {"sea" , "seaShot"} }))
		);
		uint32_t playerGridId = _gridManager.getPlayerGrid();

		_stateUI[gameState::gameplay] = {
			playerGridId
		};
	#pragma endregion

	#pragma region planning
		uint32_t btnAutoPlaceId = _ui->push<button>(button("autoPlaceBtn", AUTO_PLACE_START_POS, AUTO_PLACE_SIZE, { "autoPlace", "autoPlaceHover", "autoPlaceClicked" },
			[this](button* btn)->bool {_gridManager.autoPlace(_gridManager.getPlayerGrid()); return true; }));

		uint32_t finishPlanningId = _ui->push<button>(button("finishPlanningBtn",
			FINISH_PLANING_START_POS, FINISH_PLANING_SIZE, { "finishPlanning", "finishPlanningHover", "finishPlanningClicked" },
			OT_BIND_EVENT_FN(gsm::finishPlanningBtn)
		));

		_stateUI[gameState::planning] = { btnAutoPlaceId,finishPlanningId };

		const std::vector<std::vector<std::string>> shipStyleKeys[elementType::ship4Element - elementType::ship1Element + 1] = {
			{
				{"ship1Normal","ship1Approved","ship1Denied","ship1Shot"}
			},

			{
				{"ship2RearNormal","ship2RearApproved","ship2RearDenied","ship2RearShot"},
				{"ship2FrontNormal","ship2FrontApproved","ship2FrontDenied","ship2FrontShot"}
			},

			{
				{"ship3RearNormal","ship3RearApproved","ship3RearDenied","ship3RearShot"},
				{"ship3BaseNormal","ship3BaseApproved","ship3BaseDenied","ship3BaseShot"},
				{"ship3FrontNormal","ship3FrontApproved","ship3FrontDenied","ship3FrontShot"}
			},

			{
				{ "ship4RearNormal","ship4RearApproved","ship4RearDenied","ship4RearShot" },
				{"ship4BaseNormal","ship4BaseApproved","ship4BaseDenied","ship4BaseShot"},
				{"ship4BaseNormal","ship4BaseApproved","ship4BaseDenied","ship4BaseShot"},
				{"ship4FrontNormal","ship4FrontApproved","ship4FrontDenied","ship4FrontShot"}
			}
		};

		glm::vec3 addBtnPos = ADD_SHIP_START_POS;
		glm::vec2 addBntSize = ADD_SHIP_SIZE, countSize = COUNT_SHIP_SIZE;

		const std::array<std::string, (size_t)buttonState::stateCount> btnStyles[elementType::ship4Element - elementType::ship1Element + 1] = {
			{ "addShip1", "addShip1Hover", "addShip1Clicked" },
			{ "addShip2", "addShip2Hover", "addShip2Clicked" },
			{ "addShip3", "addShip3Hover", "addShip3Clicked" },
			{ "addShip4", "addShip4Hover", "addShip4Clicked" }
		};
		const std::vector<std::string> countStyles = {
			{ "four", "three", "two", "one", "zero"}
		};

		for (int i = elementType::ship4Element - elementType::ship1Element; i >= 0; i--) {
			elementType type = (elementType)(elementType::ship1Element + i);
			std::string baseName = "ship";
			baseName.append(1, '0' + i + 1);
			std::array<std::string, (size_t)buttonState::stateCount> tempAddBntStyle(btnStyles[i]);

			uint32_t addBtnId = _ui->push<button>(button(baseName + "AddBtn",
				{ addBtnPos.x, addBtnPos.y, addBtnPos.z }, addBntSize, std::move(tempAddBntStyle),
				[](button*) {return false; }, [this, type](button* btn) { return addShip(btn, type); }
			));

			std::vector<std::string> tempCountStyle(countStyles.cbegin() + i, countStyles.cend(), countStyles.get_allocator());

			uint32_t countId = _ui->push<counter>(counter(baseName + "Counter",
				{ addBtnPos.x + addBntSize.x / 2,addBtnPos.y + addBntSize.y / 2,addBtnPos.z + 0.1f }, countSize, (elementType::ship4Element - elementType::ship1Element - i) + 1, 0,
				std::move(tempCountStyle)
			));

			_ui->bind(countId, addBtnId);

			while (!_ui->isTypeCapReached(type)) {
				uint32_t id = _gridManager.createShip(playerGridId, type, shipStyleKeys[type - elementType::ship1Element], true);
				_ui->bind(countId, id);
			}
			_stateUI[gameState::planning].push_back(addBtnId);
			_stateUI[gameState::planning].push_back(countId);
			addBtnPos.y -= addBntSize.y + 35.0f;
		}
	#pragma endregion

	#pragma region playing
		_gridManager.setEnemyGrid(
			_ui->push<grid>(grid("enemyGrid", ROW_COUNT, COLUMN_COUNT, ENEMY_GRID_START_POS, GRID_CELL_SIZE, { {"sea" , "seaShot"} }))
		);

		auto& enemyGrid = *_ui->get<grid>(_gridManager.getEnemyGrid());
		auto& enemyGridPos = enemyGrid.getPos();
		uint32_t maskId = _ui->push<maskLayer>(maskLayer("enemyMask", enemyGrid.getRowCount(), enemyGrid.getColumnCount(), { enemyGridPos.x, enemyGridPos.y,enemyGridPos.z + 0.2f }, enemyGrid.getSize(),
			{ "fog","fogHover","fogClicked" },
			[this](maskLayer* mask) {
			uint32_t cellAttacked = mask->getCellClicked(), maskId = mask->getId(), rowCount = mask->getRowCount();
			try {
				_isPlayerMove = _gridManager.attack(_ui->getParents(maskId).front(), maskId, cellAttacked % rowCount, cellAttacked / rowCount);
			}
			catch (const std::runtime_error& ex) {
				OT_ERROR(ex.what());
			}
			catch (const std::exception& ex) {
				OT_ERROR(ex.what());
				_isPlayerMove = false;
			}
			if (_gridManager.isAllDestroyed(_gridManager.getEnemyGrid())) {
				_winningTitle = themeManager::getStyle("playerWon");
				pushState(gameState::gameOver);
			}
			return true;
		}
		));

		_ui->bind(maskId, enemyGrid.getId());

		uint32_t autoAttackBtn = _ui->push<button>(button("autoAttackBtn",
			AUTO_ATTACK_START_POS, AUTO_ATTACK_SIZE, { "autoAttack", "autoAttackHover", "autoAttackClicked" },
			[this](button* mask) {
			_isPlayerMove = (bool)_gridManager.autoAttack(_gridManager.getEnemyGrid(), *_ui->getBindings(_gridManager.getEnemyGrid()).begin());
			if (_gridManager.isAllDestroyed(_gridManager.getEnemyGrid())) {
				_winningTitle = themeManager::getStyle("playerWon");
				pushState(gameState::gameOver);
			}
			return true;
		}
		));
		for (uint32_t i = elementType::ship1Element; i < elementType::ship4Element + 1; i++) {
			_ui->setTypeCap((elementType)i, _ui->getTypeCap((elementType)i) * 2);
			_ui->setTypeActiveCap((elementType)i, _ui->getTypeCap((elementType)i) / 2);
		}

		for (int type = elementType::ship4Element; type >= elementType::ship1Element; type--)
			while (!_ui->isTypeCapReached((elementType)type))
				_gridManager.createShip(_gridManager.getEnemyGrid(), (elementType)type, shipStyleKeys[type - elementType::ship1Element], false);

		_stateUI[gameState::playing] = {
			enemyGrid.getId(),
			maskId,
			autoAttackBtn
		};
	#pragma endregion

	#pragma region pause
		uint32_t btnRetryId = _ui->push<button>(button("retry", RETRY_START_POS, RETRY_ATTACK_SIZE, { "retry", "retryHover", "retryClicked" },
			OT_BIND_EVENT_FN(gsm::resetBtn)
		));
		uint32_t btnToMainMenuId = _ui->push<button>(button("toMainMenu", TO_MAIN_MENU_START_POS, TO_MAIN_MENU_SIZE, { "toMainMenu", "toMainMenuHover", "toMainMenuClicked" },
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

	}

	int gsm::init()
	{
		_ui->deactivateAll();
		enterState(gameState::mainMenu);
		return 0;
	}

	int gsm::shutdown()
	{
		_ui->deactivateAll();
		while (_stateStack.size() > 0)
			popState();
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
				for (auto itemId : _ui->get<grid>(_gridManager.getPlayerGrid())->getAllItems())
					_ui->get(itemId)->setFlag(elementFlags::blocked);

				placeEnemyShips();
				break;
			case gameState::pause:
				break;
			case gameState::gameOver:
				auto& enemyGrid = *_ui->get<grid>(_gridManager.getEnemyGrid());
				enemyGrid.setFlag(elementFlags::blocked);
				auto* mask = *_ui->get<maskLayer>(_ui->getBindings(_gridManager.getEnemyGrid())).begin();
				if (mask != nullptr)
					mask->setFlag(elementFlags::blocked);
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
				for (auto itemId : _ui->get<grid>(_gridManager.getPlayerGrid())->getAllItems())
					_ui->get(itemId)->dropFlag(elementFlags::blocked);

				for (uint32_t i = elementType::ship1Element; i < elementType::ship4Element + 1; i++)
					_ui->setTypeActiveCap((elementType)i, _ui->getTypeActiveCap((elementType)i) / 2);
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

					_isPlayerMove = !_gridManager.autoAttack(_gridManager.getPlayerGrid(), UINT32_MAX);
					if (_gridManager.isAllDestroyed(_gridManager.getPlayerGrid())) {
						pushState(gameState::gameOver);
						_winningTitle = themeManager::getStyle("playerLose");
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
				renderer2D::drawSquad({ 0.0f,0.0f,0.9f }, { application::getInst().getWindow().getWidth(),application::getInst().getWindow().getHeight() }, { 0.7f,0.7f,0.7f,0.5f });
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

		std::vector<const char*> stateStackStr;
		for (auto state : _stateStack._Get_container()) {
			switch (state) {
				case gameState::mainMenu:
					stateStackStr.push_back("mainMenu");
					break;
				case gameState::pause:
					stateStackStr.push_back("pause");
					break;
				case gameState::planning:
					stateStackStr.push_back("planning");
					break;
				case gameState::playing:
					stateStackStr.push_back("playing");
					break;
				case gameState::gameplay:
					stateStackStr.push_back("gameplay");
					break;
				case gameState::gameOver:
					stateStackStr.push_back("gameOver");
					break;
				default:
					stateStackStr.push_back("error");
					break;
			}
		}
		ImGui::Text("Current state stack: ");
		for (auto& state : stateStackStr)
			ImGui::Text("- %s", state);
		ImGui::NewLine();

		ImGui::Text("Is player move: %d", _isPlayerMove);
		ImGui::NewLine();

		const char* elemsTypeNames[elementType::unknown] = {
			"buttonElement", "counterElement", "gridElement", "ship1Element", "ship2Element", "ship3Element", "ship4Element", "maskOverlay"
		};

		ImGui::Text("Ship Req: %d", _gridManager.checkRequirement());
		ImGui::NewLine();

		ImGui::Text("Limits:");
		for (uint32_t i = elementType::buttonElement; i < elementType::unknown; i++) {
			ImGui::Text("%s: %u, Active: %u", elemsTypeNames[i], _ui->getTypeCap((elementType)i), _ui->getTypeActiveCap((elementType)i));
		}
		ImGui::NewLine();

		auto [mouseX, mouseY] = input::getMousePos();
		ImGui::Text("Mouse position: x=%f, y=%f", mouseX, mouseY);
		ImGui::NewLine();

		ImGui::PopItemFlag();

		_ui->onImGuiRender();

		ImGui::End();
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
	void gsm::styleInit()
	{
		themeManager::add("play", style(texture2D::create("assets/resources/play.png"), 1.0f));
		themeManager::add("playHover", style(texture2D::create("assets/resources/playHover.png"), 1.0f));
		themeManager::add("playClicked", style(texture2D::create("assets/resources/playClicked.png"), 1.0f));

		themeManager::add("retry", style(texture2D::create("assets/resources/retry.png"), 1.0f));
		themeManager::add("retryHover", style(texture2D::create("assets/resources/retryHover.png"), 1.0f));
		themeManager::add("retryClicked", style(texture2D::create("assets/resources/retryClicked.png"), 1.0f));

		themeManager::add("toMainMenu", style(texture2D::create("assets/resources/toMainMenu.png"), 1.0f));
		themeManager::add("toMainMenuHover", style(texture2D::create("assets/resources/toMainMenuHover.png"), 1.0f));
		themeManager::add("toMainMenuClicked", style(texture2D::create("assets/resources/toMainMenuClicked.png"), 1.0f));

		themeManager::add("autoPlace", style(texture2D::create("assets/resources/autoPlace.png"), 1.0f));
		themeManager::add("autoPlaceHover", style(texture2D::create("assets/resources/autoPlaceHover.png"), 1.0f));
		themeManager::add("autoPlaceClicked", style(texture2D::create("assets/resources/autoPlaceClicked.png"), 1.0f));

		themeManager::add("finishPlanning", style(texture2D::create("assets/resources/start.png"), 1.0f));
		themeManager::add("finishPlanningHover", style(texture2D::create("assets/resources/startHover.png"), 1.0f));
		themeManager::add("finishPlanningClicked", style(texture2D::create("assets/resources/startClicked.png"), 1.0f));

		themeManager::add("autoAttack", style(texture2D::create("assets/resources/autoAttack.png"), 1.0f));
		themeManager::add("autoAttackHover", style(texture2D::create("assets/resources/autoAttackHover.png"), 1.0f));
		themeManager::add("autoAttackClicked", style(texture2D::create("assets/resources/autoAttackClicked.png"), 1.0f));
		themeManager::add("fog", style(texture2D::create("assets/resources/fog.png"), 1.0f));
		themeManager::add("fogHover", style(texture2D::create("assets/resources/fogHover.png"), 1.0f));
		themeManager::add("fogClicked", style(texture2D::create("assets/resources/fogClicked.png"), 1.0f));

		themeManager::add("sea", style(texture2D::create("assets/resources/sea.png"), 1.0f));
		themeManager::add("seaShot", style(texture2D::create("assets/resources/seaShot.png"), 1.0f));

	#pragma region shipTextures
		themeManager::add("addShip1", style(texture2D::create("assets/resources/addShip1.png"), 1.0f));
		themeManager::add("addShip1Hover", style(texture2D::create("assets/resources/addShip1Hover.png"), 1.0f));
		themeManager::add("addShip1Clicked", style(texture2D::create("assets/resources/addShip1Clicked.png"), 1.0f));


		themeManager::add("addShip2", style(texture2D::create("assets/resources/addShip2.png"), 1.0f));
		themeManager::add("addShip2Hover", style(texture2D::create("assets/resources/addShip2Hover.png"), 1.0f));
		themeManager::add("addShip2Clicked", style(texture2D::create("assets/resources/addShip2Clicked.png"), 1.0f));


		themeManager::add("addShip3", style(texture2D::create("assets/resources/addShip3.png"), 1.0f));
		themeManager::add("addShip3Hover", style(texture2D::create("assets/resources/addShip3Hover.png"), 1.0f));
		themeManager::add("addShip3Clicked", style(texture2D::create("assets/resources/addShip3Clicked.png"), 1.0f));


		themeManager::add("addShip4", style(texture2D::create("assets/resources/addShip4.png"), 1.0f));
		themeManager::add("addShip4Hover", style(texture2D::create("assets/resources/addShip4Hover.png"), 1.0f));
		themeManager::add("addShip4Clicked", style(texture2D::create("assets/resources/addShip4Clicked.png"), 1.0f));

		auto ship1Tex = texture2D::create("assets/resources/ship1.png");
		themeManager::add("ship1Normal", style(ship1Tex, 1.0f));
		themeManager::add("ship1Denied", style(ship1Tex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship1Approved", style(ship1Tex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship1Shot", style(texture2D::create("assets/resources/ship1Shot.png"), 1.0f));

		auto ship2FrontTex = texture2D::create("assets/resources/ship2front.png");
		themeManager::add("ship2FrontNormal", style(ship2FrontTex, 1.0f));
		themeManager::add("ship2FrontDenied", style(ship2FrontTex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship2FrontApproved", style(ship2FrontTex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship2FrontShot", style(texture2D::create("assets/resources/ship2frontShot.png"), 1.0f));

		auto ship2RearTex = texture2D::create("assets/resources/ship2rear.png");
		themeManager::add("ship2RearNormal", style(ship2RearTex, 1.0f));
		themeManager::add("ship2RearDenied", style(ship2RearTex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship2RearApproved", style(ship2RearTex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship2RearShot", style(texture2D::create("assets/resources/ship2rearShot.png"), 1.0f));

		auto ship3FrontTex = texture2D::create("assets/resources/ship3front.png");
		themeManager::add("ship3FrontNormal", style(ship3FrontTex, 1.0f));
		themeManager::add("ship3FrontDenied", style(ship3FrontTex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship3FrontApproved", style(ship3FrontTex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship3FrontShot", style(texture2D::create("assets/resources/ship3frontShot.png"), 1.0f));

		auto ship3BaseTex = texture2D::create("assets/resources/ship3base.png");
		themeManager::add("ship3BaseNormal", style(ship3BaseTex, 1.0f));
		themeManager::add("ship3BaseDenied", style(ship3BaseTex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship3BaseApproved", style(ship3BaseTex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship3BaseShot", style(texture2D::create("assets/resources/ship3baseShot.png"), 1.0f));

		auto ship3RearTex = texture2D::create("assets/resources/ship3rear.png");
		themeManager::add("ship3RearNormal", style(ship3RearTex, 1.0f));
		themeManager::add("ship3RearDenied", style(ship3RearTex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship3RearApproved", style(ship3RearTex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship3RearShot", style(texture2D::create("assets/resources/ship3rearShot.png")));

		auto ship4FrontTex = texture2D::create("assets/resources/ship4front.png");
		themeManager::add("ship4FrontNormal", style(ship4FrontTex, 1.0f));
		themeManager::add("ship4FrontDenied", style(ship4FrontTex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship4FrontApproved", style(ship4FrontTex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship4FrontShot", style(texture2D::create("assets/resources/ship4frontShot.png"), 1.0f));

		auto ship4BaseTex = texture2D::create("assets/resources/ship4base.png");
		themeManager::add("ship4BaseNormal", style(ship4BaseTex, 1.0f));
		themeManager::add("ship4BaseDenied", style(ship4BaseTex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship4BaseApproved", style(ship4BaseTex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship4BaseShot", style(texture2D::create("assets/resources/ship4baseShot.png"), 1.0f));

		auto ship4RearTex = texture2D::create("assets/resources/ship4rear.png");
		themeManager::add("ship4RearNormal", style(ship4RearTex, 1.0f));
		themeManager::add("ship4RearDenied", style(ship4RearTex, 1.0f, { 0.9f,0.1f,0.1f,0.8f }));
		themeManager::add("ship4RearApproved", style(ship4RearTex, 1.0f, { 0.1f,0.9f,0.1f,0.8f }));
		themeManager::add("ship4RearShot", style(texture2D::create("assets/resources/ship4rearShot.png"), 1.0f));
	#pragma endregion

		themeManager::add("five", style(texture2D::create("assets/resources/five.png"), 1.0f));
		themeManager::add("four", style(texture2D::create("assets/resources/four.png"), 1.0f));
		themeManager::add("three", style(texture2D::create("assets/resources/three.png"), 1.0f));
		themeManager::add("two", style(texture2D::create("assets/resources/two.png"), 1.0f));
		themeManager::add("one", style(texture2D::create("assets/resources/one.png"), 1.0f));
		themeManager::add("zero", style(texture2D::create("assets/resources/zero.png"), 1.0f));

		themeManager::add("playerWon", style(texture2D::create("assets/resources/playerWon.png"), 1.0f));
		themeManager::add("playerLose", style(texture2D::create("assets/resources/playerLose.png"), 1.0f));
	}

	void gsm::placeEnemyShips()
	{
		for (uint32_t i = elementType::ship1Element; i < elementType::ship4Element + 1; i++)
			_ui->setTypeActiveCap((elementType)i, _ui->getTypeActiveCap((elementType)i) * 2);
		try {
			_gridManager.autoPlace(_gridManager.getEnemyGrid());
		}
		catch (const std::exception& e) {
			OT_ERROR(e.what());
			_ui->get<grid>(_gridManager.getEnemyGrid())->reset();
		}

		for (auto itemId : _ui->get<grid>(_gridManager.getEnemyGrid())->getAllItems())
			_ui->get(itemId)->setFlag(elementFlags::blocked);

	}
	void gsm::reset()
	{
		uint32_t playerGridId = _gridManager.getPlayerGrid();
		uint32_t enemyGridId = _gridManager.getEnemyGrid();

		(*_ui->get<maskLayer>(_ui->getBindings(enemyGridId)).begin())->reset();
		auto counters = _ui->get<counter>(_stateUI[gameState::planning]);
		for (auto count : counters)
			count->reset();

		_gridManager.resetShips(_ui->get<grid>(enemyGridId)->getAllItems());
		_gridManager.resetShips(_ui->get<grid>(playerGridId)->getAllItems());

		_gridManager.resetGrid(enemyGridId);
		_gridManager.resetGrid(playerGridId);
	}
	bool gsm::mainMenuBtn(button* btn)
	{
		enterState(gameState::gameplay);
		pushState(gameState::planning);
		return true;
	}

	bool gsm::toMainMenuBtn(button* btn)
	{
		if (_stateStack.top() == gameState::pause || _stateStack.top() == gameState::gameOver)
			popState();
		popState();//removes child state

		reset();

		enterState(gameState::mainMenu);
		return true;
	}

	bool gsm::resetBtn(button* btn)
	{
		if (_stateStack.top() == gameState::pause || _stateStack.top() == gameState::gameOver)
			popState();

		reset();
		_ui->activate(_gridManager.getPlayerGrid());
		enterState(gameState::planning);
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
		counter& count = *(_ui->get<counter>(*_ui->getBindings(btn->getId()).begin()));
		auto [mouseX, mouseY] = input::getMousePos();
		auto pos = cameraWrapper::screenToWorld({ mouseX,  mouseY });
		if (!_ui->isTypeActiveCapReached(shipType)) {
			uint32_t id = _ui->activateFirst(shipType);
			auto& item = *_ui->get(id);
			item.setPos({ pos, _ui->get(_gridManager.getPlayerGrid())->getPos().z });
			++count;
		}

		return true;
	}
}