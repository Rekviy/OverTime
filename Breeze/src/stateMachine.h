#pragma once
#ifndef GAMESTATEMACHINE_H
#define GAMESTATEMACHINE_H

enum class gameState {
	unknown = -1, mainMenu, pause, planning, gameplay, gameOver
};
namespace overtime {
	class timeStep;
	class event;
}

class stateMachine {
public:
	virtual ~stateMachine() = default;
	virtual void enterState(gameState newState) = 0;
	virtual void onEnter(gameState state) = 0;
	virtual void onExit(gameState state) = 0;
	virtual void onUpdate(overtime::timeStep ts) = 0;
	virtual void onRender() = 0;
	virtual void onImGuiRender() = 0;
	virtual void onEvent(overtime::event& event) = 0;
};


#endif