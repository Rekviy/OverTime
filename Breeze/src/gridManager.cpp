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
	uint32_t shipId = ship->getId();
	auto& playerGrid = _ui->get<grid>(_playerGridId);
	uint32_t rows = playerGrid.getRowCount(), columns = playerGrid.getColumnCount();
	const glm::vec2& size = playerGrid.getSize();
	const glm::vec3& gridPos = playerGrid.getPos();
	bool swapped = false;
	glm::vec2 sBegin = ship->getPos(), sEnd = (ship->end() - 1)->_pos;
	if (sEnd.x < sBegin.x) { std::swap(sBegin.x, sEnd.x); swapped = true; }
	if (sEnd.y > sBegin.y) { std::swap(sBegin.y, sEnd.y); swapped = true; }

	glm::i32vec2 startIdx = round(glm::vec2((sBegin.x - gridPos.x) / size.x, (gridPos.y - sBegin.y) / size.y));
	glm::i32vec2 endIdx = round(glm::vec2((sEnd.x - gridPos.x) / size.x, (gridPos.y - sEnd.y) / size.y));

	if (startIdx.x > -1 && startIdx.y > -1 && startIdx.x < (int)columns && startIdx.y < (int)rows &&
		endIdx.x > -1 && endIdx.y > -1 && endIdx.x < (int)columns && endIdx.y < (int)rows) {

		if (swapped)
			ship->setPos({ gridPos.x + endIdx.x * size.x, gridPos.y - endIdx.y * size.y, gridPos.z + 0.2f });
		else
			ship->setPos({ gridPos.x + startIdx.x * size.x, gridPos.y - startIdx.y * size.y, gridPos.z + 0.2f });

		glm::i32vec2 areaB = startIdx - 1, areaE = endIdx + 1;

		areaB.x = std::clamp(areaB.x, 0, (int)columns - 1);
		areaB.y = std::clamp(areaB.y, 0, (int)rows - 1);
		areaE.x = std::clamp(areaE.x, 0, (int)columns - 1);
		areaE.y = std::clamp(areaE.y, 0, (int)rows - 1);

		ship::shipCell::state state = ship::shipCell::state::placingAllowed;
		if (playerGrid.isOccupied(areaB, areaE))
			state = ship::shipCell::state::placingDenied;

		ship->changeState(state);

		playerGrid.addTempPlacement(shipId, startIdx, endIdx);
		return true;
	}
	else if (playerGrid.isPlaced(shipId))
		playerGrid.removeTempPlacement(shipId);
	ship->changeState(ship::shipCell::state::normal);
	return false;
}

bool gridManager::placeShip(ship* ship)
{
	auto& playerGrid = _ui->get<grid>(_playerGridId);
	uint32_t shipId = ship->getId();
	if (!playerGrid.isPlaced(shipId) || ship->getState() == ship::shipCell::placingDenied) {
		auto& bind = _ui->getBindings(shipId).begin();
		auto& count = _ui->get<counter>(*_ui->getBindings(shipId).begin());
		--count;
		_ui->deactivate(shipId);
	}
	else {
		const auto& shipPos = ship->getPos();
		ship->setPos({ shipPos.x, shipPos.y, playerGrid.getPos().z + 0.1f });
		playerGrid.acceptPlacing(shipId);
		_ui->bind(shipId, _playerGridId);
	}
	ship->changeState(ship::shipCell::state::normal);
	return true;
}

bool gridManager::removeShip(ship* ship)
{
	uint32_t shipId = ship->getId();
	auto& playerGrid = _ui->get<grid>(_playerGridId);
	if (playerGrid.isPlaced(shipId)) {
		playerGrid.removePlacement(shipId);
		_ui->unBind(shipId, _playerGridId);
	}
	return true;
}

uint32_t gridManager::createShip(uint32_t gridId, elementType shipType)
{
	auto& curGrid = _ui->get<grid>(gridId);
	const glm::vec2& size = curGrid.getSize();
	const glm::vec3& gridPos = curGrid.getPos();


	uint32_t shipId = -1;
	std::string name = (curGrid.getName());
	switch (shipType) {
		case ship1Element:
			name.append("ship1_");
			name.append(1, '1' + _ui->checkTypeCap(shipType));
			shipId = _ui->push<ship1>(ship1(name, { 0.0f, 0.0f, gridPos.z + 0.1f },
				size, { { "cherry","cherryApproved","cherryDenied","frame" } },
				[this](ship* ship) {return false; },
				[this](ship* ship) {return false; },
				[this](ship* ship) {return false; }
			));
			break;
		case ship2Element:
			name.append("ship2_");
			name.append(1, '1' + _ui->checkTypeCap(shipType));
			shipId = _ui->push<ship2>(ship2(name, { 0.0f, 0.0f, gridPos.z + 0.1f },
				size, { { "cherry","cherryApproved","cherryDenied","frame" } },
				[this](ship* ship) {return false; },
				[this](ship* ship) {return false; },
				[this](ship* ship) {return false; }
			));
			break;
		case ship3Element:
			name.append("ship3_");
			name.append(1, '1' + _ui->checkTypeCap(shipType));
			shipId = _ui->push<ship3>(ship3(name, { 0.0f, 0.0f, gridPos.z + 0.1f },
				size, { { "cherry","cherryApproved","cherryDenied","frame" } },
				[this](ship* ship) {return false; },
				[this](ship* ship) {return false; },
				[this](ship* ship) {return false; }
			));
			break;
		case ship4Element:
			name.append("ship4_");
			name.append(1, '1' + _ui->checkTypeCap(shipType));
			shipId = _ui->push<ship4>(ship4(name, { 0.0f, 0.0f, gridPos.z + 0.1f },
				size, { { "cherry","cherryApproved","cherryDenied","frame" } },
				[this](ship* ship) {return false; },
				[this](ship* ship) {return false; },
				[this](ship* ship) {return false; }
			));
			break;
	}
	return shipId;
}

void gridManager::autoPlace(uint32_t gridId)
{
	auto& curGrid = _ui->get<grid>(gridId);
	uint32_t rows = curGrid.getRowCount(), columns = curGrid.getColumnCount();
	const glm::vec2& size = curGrid.getSize();
	const glm::vec3& gridPos = curGrid.getPos();

	for (int type = elementType::ship4Element; type >= elementType::ship1Element; type--)
		while (!_ui->isTypeActiveCapReached((elementType)type))
			_ui->bind(_ui->activateFirst((elementType)type), gridId);

	for (auto& id : _ui->getBindings(gridId)) {
		ship& item = static_cast<ship&>(_ui->get(id));
		item.setDragging(false);
		glm::i32vec2 startIdx(0), endIdx(0);
		float rotation = 0.0f;
		bool isPlaced = false;
		const int MAX_ATTEMPTS = 100;
		int attempts = 0;
		int c, s;
		if (!curGrid.isPlaced(id)) {
			do {
				attempts++;
				rotation = (rand() % 2 != 0) ? glm::radians(90.0f) : 0.0f;
				startIdx = { rand() % columns,rand() % rows };

				c = glm::cos(rotation), s = glm::sin(rotation);
				endIdx = { startIdx.x + (item.length() - 1) * c,startIdx.y + (item.length() - 1) * s };

				if (endIdx.x > -1 && endIdx.y > -1 && endIdx.x < (int)columns && endIdx.y < (int)rows) {

					glm::i32vec2 areaB = { startIdx.x - 1,startIdx.y - 1 }, areaE = { endIdx.x + 1, endIdx.y + 1 };
					areaB.x = std::clamp(areaB.x, 0, (int)columns - 1);
					areaB.y = std::clamp(areaB.y, 0, (int)rows - 1);
					areaE.x = std::clamp(areaE.x, 0, (int)columns - 1);
					areaE.y = std::clamp(areaE.y, 0, (int)rows - 1);

					isPlaced = !curGrid.isOccupied(areaB, areaE);
				}
			} while (!isPlaced && attempts < MAX_ATTEMPTS);
			if (isPlaced) {
				item.setRotation(rotation);
				item.setPos({ gridPos.x + startIdx.x * size.x, gridPos.y - startIdx.y * size.y, gridPos.z + 0.1f });
				curGrid.addPlacement(id, startIdx, endIdx);
			}
		}
	}
	_ui->unBindAll(gridId);
}
