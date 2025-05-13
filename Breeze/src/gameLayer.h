#pragma once
#ifndef GAMELAYER_H
#define GAMELAYER_H

#include "ship.h"
#include "ui/gameUI.h"
#include "grid.h"

#include <overtime.h>

class button;

class gameLayer :public overtime::layer {
public:
	gameLayer();
	virtual void onAttach()override;
	virtual void onDetach() override;
	virtual void onUpdate(overtime::timeStep ts)override;
	virtual void onImGuiRender() override;
	virtual void onEvent(overtime::event& event) override;

private:
	void mainMenu();
	bool onWindowResize(overtime::windowResizeEvent& event);
	bool mainMenuBtn(button* btn);
	bool addShip4(button* btn);
	bool gridCalculate(ship* ship);
	bool placeShip(ship* ship);
	overtime::scope<grid> _playerGrid;
	gameUI _ui;
};

#endif