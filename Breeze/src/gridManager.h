#pragma once
#ifndef GRIDMANAGER_H

#include "ui/ship.h"
#include "ui/grid.h"
#include "ui/button.h"
#include "ui/gameUI.h"
#include <map>
class gridManager {
public:
	gridManager(std::shared_ptr<gameUI> ui);
	gridManager() = default;
	~gridManager();
	//void init()
	inline void setPlayerGrid(uint32_t gridId) { _playerGridId = gridId; }
	inline void setEnemyGrid(uint32_t gridId) { _enemyGridId = gridId; }
	inline uint32_t getPlayerGrid() const { return _playerGridId; }
	inline uint32_t getEnemyGrid()const { return _shipReq; }
	inline void setShipRequirement(uint32_t newReq) { _shipReq = newReq; }
	inline uint32_t getShipRequirement()const { return _enemyGridId; }
	bool gridCalculate(ship* ship);
	bool placeShip(ship* ship);
	bool removeShip(ship* ship);
	void autoPlace(uint32_t gridId);
	bool checkRequirement() { return (_shipPlacings.size() == _shipReq); }
	inline bool isPlaced(uint32_t id) { return (_shipPlacings.find(id) != _shipPlacings.end()); }
private:
	std::shared_ptr<gameUI> _ui;
	uint32_t _playerGridId = -1;
	uint32_t _enemyGridId = -1;
	uint32_t _shipReq = -1;
	
	std::map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>> _shipPlacings;
};

#endif