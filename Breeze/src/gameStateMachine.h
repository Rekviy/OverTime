#pragma once
#ifndef BREEZEGSM_H
#define BREEZEGSM_H

#include "stateMachine.h"
#include "ui/gameUI.h"

#include <overtime.h>

class button;
class ship;
class grid;

class gsm : public stateMachine {
public:
	gsm();
	virtual void enterState(gameState newState) override;
	virtual void onEnter(gameState state) override;
	virtual void onExit(gameState state) override;
	virtual void onUpdate(overtime::timeStep ts) override;
	virtual void onRender() override;
	virtual void onImGuiRender() override;
	virtual void onEvent(overtime::event& event) override;
private:
	bool mainMenuBtn(button* btn);
	bool addShip4(button* btn);
	bool gridCalculate(ship* ship);
	bool placeShip(ship* ship);
	bool removeShip(ship* ship);
	uint32_t _playerGridId;
	gameUI _ui;
	gameState _currentState = gameState::unknown;
	std::unordered_map<gameState, std::vector<uint32_t>> _stateUI;
};


#endif