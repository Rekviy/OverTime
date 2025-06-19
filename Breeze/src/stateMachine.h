#pragma once
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <overtime.h>
enum class gameState {
	unknown = 0, mainMenu, gameplay, planning, playing, pause, gameOver
};

class stateMachine {
public:
	virtual ~stateMachine() = default;
	virtual int init() = 0;
	virtual int shutdown() = 0;

	virtual void enterState(gameState newState) = 0;
	virtual void onEnter(gameState state) = 0;
	virtual void onExit(gameState state) = 0;
	virtual void onUpdate(overtime::timeStep ts) = 0;
	virtual void onRender() = 0;
	virtual void onImGuiRender() = 0;
	virtual void onEvent(overtime::event& event) = 0;
};
#endif