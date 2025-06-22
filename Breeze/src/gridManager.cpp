#include "gridManager.h"

#include "cameraWrapper.h"
#include "ui/objectPool.h"
#include "ui/counter.h"
#include "ui/button.h"
#include "ui/maskLayer.h"
gridManager::gridManager(std::shared_ptr<gameUI> ui)
	:_ui(ui)
{}

bool gridManager::gridCalculate(ship* s)
{
	uint32_t shipId = s->getId();
	auto& playerGrid = *_ui->get<grid>(_playerGridId);
	uint32_t rows = playerGrid.getRowCount(), columns = playerGrid.getColumnCount();
	const glm::vec2& size = playerGrid.getSize();
	if (size.x == 0.0f || size.y == 0.0f)
		return false;//throw std::runtime_error("gridCalculate: playerGrid cellSize = 0!");

	const glm::vec3& gridPos = playerGrid.getPos();
	bool swapped = false;
	glm::vec2 sBegin = s->getPos(), sEnd = (s->end() - 1)->_pos;
	if (sEnd.x < sBegin.x) { std::swap(sBegin.x, sEnd.x); swapped = true; }
	if (sEnd.y > sBegin.y) { std::swap(sBegin.y, sEnd.y); swapped = true; }

	glm::i32vec2 startIdx = round(glm::vec2((sBegin.x - gridPos.x) / size.x, (gridPos.y - sBegin.y) / size.y));
	glm::i32vec2 endIdx = round(glm::vec2((sEnd.x - gridPos.x) / size.x, (gridPos.y - sEnd.y) / size.y));

	if (startIdx.x > -1 && startIdx.y > -1 && startIdx.x < (int)columns && startIdx.y < (int)rows &&
		endIdx.x > -1 && endIdx.y > -1 && endIdx.x < (int)columns && endIdx.y < (int)rows) {

		if (swapped)
			s->setPos({ gridPos.x + endIdx.x * size.x, gridPos.y - endIdx.y * size.y, gridPos.z + 0.2f });
		else
			s->setPos({ gridPos.x + startIdx.x * size.x, gridPos.y - startIdx.y * size.y, gridPos.z + 0.2f });

		glm::i32vec2 areaB = startIdx - 1, areaE = endIdx + 1;

		areaB.x = std::clamp(areaB.x, 0, (int)columns - 1);
		areaB.y = std::clamp(areaB.y, 0, (int)rows - 1);
		areaE.x = std::clamp(areaE.x, 0, (int)columns - 1);
		areaE.y = std::clamp(areaE.y, 0, (int)rows - 1);

		shipCellState state = shipCellState::placingAllowed;
		if (playerGrid.isOccupied(areaB, areaE))
			state = shipCellState::placingDenied;

		s->changeState(state);

		playerGrid.addTempPlacement(shipId, startIdx, endIdx);
		return true;
	}
	else if (playerGrid.isPlaced(shipId))
		playerGrid.rejectPlacing(shipId);
	s->changeState(shipCellState::normal);
	return false;
}

bool gridManager::placeShip(ship* s)
{
	auto* playerGrid = _ui->get<grid>(_playerGridId);
	if (playerGrid != nullptr) {
		uint32_t shipId = s->getId();
		auto countVec = _ui->get<counter>(_ui->getBindings(shipId));
		
		if (!playerGrid->isPlaced(shipId) || s->getState() == shipCellState::placingDenied) {
			playerGrid->rejectPlacing(shipId);
			_ui->deactivate(shipId);
			if (!countVec.empty())
				--(*countVec.front());
		}
		else {
			const auto& shipPos = s->getPos();
			s->setPos({ shipPos.x, shipPos.y, playerGrid->getPos().z + 0.1f });
			playerGrid->acceptPlacing(shipId);
		}
		s->changeState(shipCellState::normal);
		return true;
	}
	return false;
}

bool gridManager::removeShip(ship* s)
{
	uint32_t shipId = s->getId();
	auto* playerGrid = _ui->get<grid>(_playerGridId);
	if (playerGrid != nullptr) {
		if (playerGrid->isPlaced(shipId)) {
			auto tempPlace = *playerGrid->getPlacement(shipId);
			playerGrid->removePlacement(shipId);
			playerGrid->addTempPlacement(shipId, tempPlace);
		}
		return true;
	}
	return false;
}

bool gridManager::isAllDestroyed(uint32_t gridId)
{
	auto* curGrid = _ui->get<grid>(gridId);

	if (curGrid != nullptr) {
		auto ships = curGrid->getAllItems();
		if (!ships.empty()) {
			for (auto id : ships) {
				auto& curShip = static_cast<ship&>(*_ui->get(id));
				if (curShip.getShipState() != shipState::destroyed)
					return false;
			}
			return true;
		}
	}
	return false;
}

uint32_t gridManager::createShip(uint32_t gridId, elementType shipType, const std::vector<std::vector<std::string>>& keys, bool isPlayer)
{
	auto* curGrid = _ui->get<grid>(gridId);

	if (curGrid == nullptr)
		throw std::runtime_error("grid doesn't provided for createShip!");

	const glm::vec2& size = curGrid->getSize();
	const glm::vec3& gridPos = curGrid->getPos();

	uint32_t shipId = -1;

	std::string baseName = curGrid->getName();
	baseName.append("ship").append(1, '0' + (uint32_t)shipType - (uint32_t)ship1Element + (uint32_t)1);
	baseName.append("_").append(1, '1' + _ui->checkTypeCap(shipType));

	std::function<bool(ship*)> onPress, onRelease, onMoving;
	(isPlayer) ? onPress = [this](ship* s) { return removeShip(s); } : onPress = [this](ship* s) { return false; };
	(isPlayer) ? onRelease = [this](ship* s) { return placeShip(s); } : onRelease = [this](ship* s) { return false; };
	(isPlayer) ? onMoving = [this](ship* s) { return gridCalculate(s); } : onMoving = [this](ship* s) { return false; };

	switch (shipType) {
		case ship1Element:
			shipId = _ui->push<ship1>(
				ship1(baseName, gridPos, size, keys, std::move(onPress), std::move(onRelease), std::move(onMoving))
			);
			break;
		case ship2Element:
			shipId = _ui->push<ship2>(
				ship2(baseName, gridPos, size, keys, std::move(onPress), std::move(onRelease), std::move(onMoving))
			);
			break;
		case ship3Element:
			shipId = _ui->push<ship3>(
				ship3(baseName, gridPos, size, keys, std::move(onPress), std::move(onRelease), std::move(onMoving))
			);
			break;
		case ship4Element:
			shipId = _ui->push<ship4>(
				ship4(baseName, gridPos, size, keys, std::move(onPress), std::move(onRelease), std::move(onMoving))
			);
			break;
		default:
			throw std::invalid_argument("createShip: invalid shipType");
	}
	return shipId;
}

void gridManager::autoPlace(uint32_t gridId)
{
	auto* curGrid = _ui->get<grid>(gridId);
	if (curGrid == nullptr)
		throw std::runtime_error("grid doesn't provided for autoPlace!");

	uint32_t rows = curGrid->getRowCount(), columns = curGrid->getColumnCount();
	const glm::vec2& size = curGrid->getSize();
	const glm::vec3& gridPos = curGrid->getPos();

	for (int type = elementType::ship1Element; type <= elementType::ship4Element; type++) {
		while (true) {
			uint32_t id = _ui->activateFirst((elementType)type);
			if (id == UINT32_MAX) break;
			_ui->bind(id, gridId);

		}
	}
	auto& idVec = _ui->getBindings(gridId);
	for (uint32_t i = idVec.size(); i > 0; i--) {
		uint32_t id = idVec[i - 1];
		ship* item = dynamic_cast<ship*>(_ui->get(id));
		if (item != nullptr) {
			item->changeShipState(shipState::normal);
			glm::i32vec2 startIdx(0), endIdx(0);
			float rotation = 0.0f;
			bool isPlaced = false;
			const int MAX_ATTEMPTS = 100;
			int attempts = 0;
			int c, s;
			if (!curGrid->isPlaced(id)) {
				do {
					attempts++;
					rotation = (rand() % 2 != 0) ? glm::radians(90.0f) : 0.0f;
					startIdx = { rand() % columns,rand() % rows };

					c = (int)glm::cos(rotation), s = (int)glm::sin(rotation);
					endIdx = { startIdx.x + (item->length() - 1) * c,startIdx.y + (item->length() - 1) * s };

					if (endIdx.x > -1 && endIdx.y > -1 && endIdx.x < (int)columns && endIdx.y < (int)rows) {

						glm::i32vec2 areaB = { startIdx.x - 1,startIdx.y - 1 }, areaE = { endIdx.x + 1, endIdx.y + 1 };
						areaB.x = std::clamp(areaB.x, 0, (int)columns - 1);
						areaB.y = std::clamp(areaB.y, 0, (int)rows - 1);
						areaE.x = std::clamp(areaE.x, 0, (int)columns - 1);
						areaE.y = std::clamp(areaE.y, 0, (int)rows - 1);

						isPlaced = !curGrid->isOccupied(areaB, areaE);
					}
				} while (!isPlaced && attempts < MAX_ATTEMPTS);
				if (isPlaced) {
					item->setRotation(rotation);
					item->setPos({ gridPos.x + startIdx.x * size.x, gridPos.y - startIdx.y * size.y, gridPos.z + 0.1f });
					curGrid->addPlacement(id, startIdx, endIdx);

					auto& bindVec = _ui->getBindings(id);
					auto count = (_ui->get<counter>(bindVec));
					if (!count.empty())
						++(*count.front());
				}
				else
					_ui->deactivate(id);
			}
			_ui->unBind(id, gridId);
		}
	}
}

bool gridManager::attack(uint32_t gridId, uint32_t maskId, uint32_t x, uint32_t y)
{
	auto* curGrid = _ui->get<grid>(gridId);
	if (curGrid == nullptr)
		throw std::runtime_error("grid doesn't provided for attack!");

	maskLayer* curMask = nullptr;
	if (maskId != UINT32_MAX) {
		curMask = _ui->get<maskLayer>(maskId);
		curMask->setCellVisibility(y * curMask->getColumnCount() + x, false);
	}

	curGrid->changeState({ x,y }, gridCellState::shot);
	uint32_t shipId = curGrid->getItemAt({ x, y });
	if (shipId != UINT32_MAX) {
		auto& attackedShip = static_cast<ship&>(*_ui->get(shipId));

		auto& [shipStart, shipEnd] = *curGrid->getPlacement(shipId);
		attackedShip.changeState(x - shipStart.x + y - shipStart.y, shipCellState::shot);
		
		if (attackedShip.getShipState() == shipState::destroyed && curMask != nullptr) {
			uint32_t rows = curGrid->getRowCount();
			uint32_t columns = curGrid->getColumnCount();
			glm::i32vec2 areaB = { shipStart.x - 1,shipStart.y - 1 }, areaE = { shipEnd.x + 1, shipEnd.y + 1 };
			areaB.x = std::clamp(areaB.x, 0, (int)columns - 1);
			areaB.y = std::clamp(areaB.y, 0, (int)rows - 1);
			areaE.x = std::clamp(areaE.x, 0, (int)columns - 1);
			areaE.y = std::clamp(areaE.y, 0, (int)rows - 1);

			curMask->setCellVisibility(areaB, areaE, false);
		}

		return true;
	}

	return false;
}

bool gridManager::autoAttack(uint32_t gridId, uint32_t maskId)
{
	auto* curGrid = _ui->get<grid>(gridId);
	uint32_t rows = curGrid->getRowCount(), columns = curGrid->getColumnCount();

	glm::i32vec2 pos;
	do {
		pos.x = rand() % columns; pos.y = rand() % rows;

	} while (curGrid->getState(pos) == gridCellState::shot);

	return attack(gridId, maskId, pos.x, pos.y);
}

void gridManager::resetGrid(uint32_t gridId)
{
	auto* curGrid = _ui->get<grid>(gridId);
	if (curGrid == nullptr)
		throw std::runtime_error("grid doesn't provided for reseting!");
	curGrid->reset();
	_ui->deactivate(gridId);
}

void gridManager::resetShips(std::vector<uint32_t>& ships)
{
	for (auto id : ships) {
		auto& curShip = static_cast<ship&>(*_ui->get(id));
		curShip.reset();

		auto countVec = _ui->get<counter>(_ui->getBindings(id));
		if (!countVec.empty()) 
			countVec.front()->reset();

		_ui->deactivate(id);
	}
}