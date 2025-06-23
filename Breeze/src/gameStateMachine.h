#pragma once
#ifndef GAMESTATEMACHINE_H
#define GAMESTATEMACHINE_H

#include "stateMachine.h"
#include "ui/gameUI.h"
#include "gridManager.h"

#include <stack>

namespace breeze {
	class button;
	class ship;
	class grid;

	class gsm : public stateMachine {
	public:
		gsm();
		virtual ~gsm() = default;
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
		void styleInit();
		void placeEnemyShips();
		void reset();
		bool mainMenuBtn(button* btn);
		bool finishPlanningBtn(button* btn);
		bool toMainMenuBtn(button* btn);
		bool resetBtn(button* btn);
		bool addShip(button* btn, elementType shipType);
		bool _isPlayerMove = true;
		overtime::ref<style> _winningTitle;
		overtime::ref<gameUI> _ui = std::make_shared<gameUI>();
		gridManager _gridManager = gridManager(_ui);
		std::stack<gameState> _stateStack;
		std::unordered_map<gameState, std::vector<uint32_t>> _stateUI;
	};
}
#endif