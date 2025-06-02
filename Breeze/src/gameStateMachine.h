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
	~gsm();
	virtual int init() override;
	virtual int shutdown() override;

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

	bool addShip(button* btn, elementType shipType);
	gridManager _gridManager;
	overtime::ref<gameUI> _ui;
	//gameState _currentState = gameState::unknown;
	std::stack<gameState> _stateStack;
	std::unordered_map<gameState, std::vector<uint32_t>> _stateUI;
};


#endif