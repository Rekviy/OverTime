#pragma once
#ifndef GRIDMANAGER_H

#include "ui/ship.h"
#include "ui/grid.h"
#include "ui/button.h"
#include "ui/gameUI.h"
namespace breeze {
	class gridManager {
	public:
		gridManager(std::shared_ptr<gameUI> ui);
		~gridManager() = default;

		inline void setPlayerGrid(uint32_t gridId) { _playerGridId = gridId; }
		inline void setEnemyGrid(uint32_t gridId) { _enemyGridId = gridId; }
		inline uint32_t getPlayerGrid() const { return _playerGridId; }
		inline uint32_t getEnemyGrid()const { return _enemyGridId; }
		inline void setShipRequirement(uint32_t newReq) { _shipReq = newReq; }
		inline uint32_t getShipRequirement()const { return _shipReq; }
		bool gridCalculate(ship* ship);
		bool checkRequirement() { return (_ui->get<grid>(_playerGridId)->placementCount() == _shipReq); }
		bool isAllDestroyed(uint32_t gridId);
		bool placeShip(ship* ship);
		bool removeShip(ship* ship);
		uint32_t createShip(uint32_t gridId, elementType shipType, const std::vector<std::vector<std::string>>& keys, bool isPlayer);
		void autoPlace(uint32_t gridId);

		bool attack(uint32_t gridId, uint32_t maskId, uint32_t x, uint32_t y);
		bool autoAttack(uint32_t gridId, uint32_t maskId);

		void resetGrid(uint32_t gridId);
		void resetShips(const std::vector<uint32_t>& ships);
	private:
		std::shared_ptr<gameUI> _ui;
		uint32_t _playerGridId = -1;
		uint32_t _enemyGridId = -1;
		uint32_t _shipReq = -1;
	};
}
#endif