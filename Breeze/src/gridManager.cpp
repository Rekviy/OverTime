#include "gridManager.h"

#include "cameraWrapper.h"
#include "ui/objectPool.h"
#include "ui/counter.h"
#include "ui/button.h"
gridManager::gridManager(std::shared_ptr<gameUI> ui)
	:_ui(ui)
{}

gridManager::~gridManager()
{}

bool gridManager::gridCalculate(ship* ship)
{
	auto& playerGrid = _ui->get<grid>(_playerGridId);
	const glm::vec2& size = playerGrid.getSize();
	const glm::vec3& gridPos = playerGrid.getPos();
	glm::i32vec2 begin = round(glm::vec2((ship->getPos().x - gridPos.x) / size.x, (gridPos.y - ship->getPos().y) / size.y));

	glm::i32vec2 end = round(glm::vec2(((ship->end() - 1)->_pos.x - gridPos.x) / size.x, (gridPos.y - (ship->end() - 1)->_pos.y) / size.y));

	if (begin.x > -1 && begin.y > -1 && begin.x < (int)playerGrid.getColumnCount() && begin.y < (int)playerGrid.getRowCount() &&
		end.x > -1 && end.y > -1 && end.x < (int)playerGrid.getColumnCount() && end.y < (int)playerGrid.getRowCount()) {

		ship->setPos({ gridPos.x + begin.x * size.x, gridPos.y - begin.y * size.y, gridPos.z + 0.2f });

		glm::i32vec2 areaB = begin - 1, areaE = end + 1;

		areaB.x = std::max(0, std::min(areaB.x, (int32_t)playerGrid.getRowCount() - 1));
		areaB.y = std::max(0, std::min(areaB.y, (int32_t)playerGrid.getColumnCount() - 1));
		areaE.x = std::max(0, std::min(areaE.x, (int32_t)playerGrid.getRowCount() - 1));
		areaE.y = std::max(0, std::min(areaE.y, (int32_t)playerGrid.getColumnCount() - 1));

		ship::shipCell::state state = ship::shipCell::state::placingAllowed;
		if (playerGrid.isOccupied(areaB, areaE))
			state = ship::shipCell::state::placingDenied;

		ship->changeState(state);
		_shipPlacings[ship->getId()].first = begin;
		_shipPlacings[ship->getId()].second = end;
		return true;
	}
	if(isPlaced(ship->getId()))
		_shipPlacings.erase(ship->getId());
	ship->changeState(ship::shipCell::state::normal);
	return false;
}

bool gridManager::placeShip(ship* ship)
{
	if (!isPlaced(ship->getId()) || ship->getState() == ship::shipCell::placingDenied) {
		auto& bind = _ui->getBindings(ship->getId()).begin();
		auto& count = _ui->get<counter>(*_ui->getBindings(ship->getId()).begin());
		--count;
		_ui->deactivate(ship->getId());
	}
	else {
		auto& playerGrid = _ui->get<grid>(_playerGridId);
		auto& [begin, end] = _shipPlacings.at(ship->getId());
		const auto& shipPos = ship->getPos();
		ship->setPos({ shipPos.x, shipPos.y, shipPos.z - 0.1f });
		playerGrid.setOccupation(begin, end, true);
	}
	ship->changeState(ship::shipCell::state::normal);
	return true;
}

bool gridManager::removeShip(ship* ship)
{
	if (isPlaced(ship->getId())) {

		auto& playerGrid = _ui->get<grid>(_playerGridId);
		auto& [begin, end] = _shipPlacings.at(ship->getId());
		playerGrid.setOccupation(begin, end, false);

	}
	return true;
}

void gridManager::autoPlace(uint32_t gridId)
{

}
