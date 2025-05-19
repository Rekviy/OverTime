#pragma once
#ifndef GAMESTATEMACHINE_H
#define GAMESTATEMACHINE_H

#include "stateMachine.h"
#include "ui/gameUI.h"
#include "gridManager.h"

#include <stack>

class button;
class ship;
class grid;

class gsm : public stateMachine {
public:
	gsm();
	virtual void enterState(gameState newState) override;
	void pushState(gameState newState);
	void popState();
	virtual void onEnter(gameState state) override;
	virtual void onExit(gameState state) override;
	virtual void onUpdate(overtime::timeStep ts) override;
	virtual void onRender() override;
	virtual void onImGuiRender() override;
	virtual void onEvent(overtime::event& event) override;
private:
	bool mainMenuBtn(button* btn);
	bool finishPlanningBtn(button* btn);
	bool addShip4(button* btn);
	bool addShip3(button* btn);
	bool addShip2(button* btn);
	bool addShip1(button* btn);
	gridManager _gridManager;
	overtime::ref<gameUI> _ui;
	//gameState _currentState = gameState::unknown;
	std::stack<gameState> _stateStack;
	//todo add state stack
	std::unordered_map<gameState, std::vector<uint32_t>> _stateUI;
};


#endif