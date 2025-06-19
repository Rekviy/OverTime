#include "objectPool.h"

using namespace overtime;

objectPool::objectPool()
{
	_typeCaps.resize(elementType::unknown + 1, UINT32_MAX);
	_typeActiveCaps.resize(elementType::unknown + 1, UINT32_MAX);
}

uint32_t objectPool::push(overtime::scope<interactElement> element)
{
	if (_storage.find(element->_id) != _storage.end())
		return element->_id;
	if (_typeKeys[element->getType()].size() < _typeCaps[element->getType()]) {
		auto& it = _storage.emplace(_idCounter++, std::move(element)).first;
		it->second->_id = it->first;
		auto type = it->second->getType();
		auto& itByType = _typeKeys.find(type);

		_typeKeys[type].push_back(it->first);

		if (it->second->checkFlag(elementFlags::active) && _typeActiveKeys[type].size() < _typeActiveCaps[type]) {
			_typeActiveKeys[type].push_back(it->first);
			zSort(_typeActiveKeys[type].begin(), _typeActiveKeys[type].end());
		}
		return it->first;
	}
	return -1;
}


overtime::scope<interactElement> objectPool::pop(uint32_t id)
{
	auto itToDelete = _storage.find(id);
	if (itToDelete != _storage.end()) {
		throw std::out_of_range("objectPool::pop: element with id: " + std::to_string(id) + "not found");
	}
	deactivate(id);
	auto& node = _storage.extract(id);
	auto& vec = _typeKeys[node.mapped()->getType()];
	auto& vecIt = std::find(vec.begin(), vec.end(), id);
	vec.erase(vecIt);

	return std::move(node.mapped());
}

interactElement* objectPool::get(uint32_t id)
{
	auto it = _storage.find(id);
	if (it != _storage.end())
		return it->second.get();
	return nullptr;
}

std::map<uint32_t, overtime::scope<interactElement>>::iterator objectPool::find(uint32_t id)
{
	return _storage.find(id);
}

void objectPool::activateAll()
{
	for (auto& key : _typeKeys) {
		auto type = key.first;
		for (auto id : key.second) {
			if (_typeActiveKeys[type].size() < _typeActiveCaps[type])
				activate(id);
		}
	}
}
//todo rework activation/deactivation to work within op
bool objectPool::activate(uint32_t id)
{
	auto itemIt = _storage.find(id);
	if (itemIt != _storage.end() && !itemIt->second->checkFlag(elementFlags::active)) {
		auto type = itemIt->second->getType();
		if (_typeActiveKeys[type].size() < _typeActiveCaps[type]) {
			itemIt->second->activate();

			_typeActiveKeys[type].push_back(id);
			zSort(_typeActiveKeys[type].begin(), _typeActiveKeys[type].end());
			return true;
		}
	}
	return false;
}

uint32_t objectPool::activateFirst(elementType type)
{
	if (_typeActiveKeys[type].size() < _typeActiveCaps[type]) {
		for (auto id : _typeKeys[type]) {
			if (!_storage.at(id)->checkFlag(elementFlags::active)) {
				activate(id);
				return id;
			}
		}
	}
	return -1;
}

void objectPool::deactivateAll()
{
	for (auto& key : _typeActiveKeys) {
		for (auto id : key.second) {
			_storage.at(id)->deactivate();
		}
		key.second.clear();
	}

}

void objectPool::deactivate(uint32_t id)
{
	if (_storage.at(id)->checkFlag(elementFlags::active)) {
		_storage.at(id)->deactivate();
		auto& activeVec = _typeActiveKeys[_storage.at(id)->getType()];
		auto& vecIt = std::find(activeVec.begin(), activeVec.end(), id);
		//std::swap(vecIt, vec.end() - 1);
		activeVec.erase(vecIt);
	}
}

bool objectPool::isExist(uint32_t id)
{
	if (_storage.find(id) != _storage.end()) return true;
	return false;
}

std::vector<overtime::scope<interactElement>> objectPool::setTypeCap(elementType type, uint32_t newCap)
{
	std::vector<overtime::scope<interactElement>> removed;
	if (type >= _typeCaps.size())
		throw std::out_of_range("objectPool::setTypeCap type not in enum range!");

	if (_typeActiveCaps[type] > newCap) setTypeActiveCap(type, newCap);

	if (_typeKeys[type].size() > newCap) {
		uint32_t diff = (uint32_t)_typeKeys[type].size() - newCap;
		removed.reserve(diff);
		auto& vec = _typeKeys[type];

		for (uint32_t i = diff; i > 0;) {
			auto& it = vec.begin() + --i;
			if (_storage[*it]->checkFlag(elementFlags::active)) deactivate(*it);

			removed.push_back(std::move(_storage.extract(*it).mapped()));
			vec.erase(it);
		}

	}
	_typeCaps[type] = newCap;
	return removed;
}

std::vector<uint32_t> objectPool::setTypeActiveCap(elementType type, uint32_t newCap)
{
	std::vector<uint32_t> deactivated;

	if (_typeActiveKeys[type].size() > newCap) {
		auto& activeVec = _typeActiveKeys[type];
		uint32_t diff = _typeActiveKeys[type].size() - newCap;

		for (uint32_t i = 0; i < diff; i++) {
			uint32_t id = activeVec[i];
			_storage[id]->deactivate();
			deactivated.push_back(id);
		}
		activeVec.erase(activeVec.begin(), activeVec.begin() + diff);
	}
	else if (_typeCaps[type] < newCap)
		setTypeCap(type, newCap);
	_typeActiveCaps[type] = newCap;
	return deactivated;
}

uint32_t objectPool::checkTypeCap(elementType type) const
{
	if (type >= _typeCaps.size()) {
		throw std::out_of_range("objectPool::checkTypeCap: elementType out of range");
	}
	return (uint32_t)_typeKeys.at(type).size();
}

uint32_t objectPool::checkTypeActiveCap(elementType type) const
{
	if (type >= _typeActiveCaps.size()) {
		throw std::out_of_range("objectPool::checkTypeActiveCap: elementType out of range");
	}
	return (uint32_t)_typeActiveKeys.at(type).size();
}

void objectPool::zSort(std::vector<uint32_t>::iterator begin, std::vector<uint32_t>::iterator end)
{
	std::sort(begin, end, [this](const uint32_t& idA, const uint32_t& idB)->bool {
		if (_storage[idA]->getPos().z == _storage[idB]->getPos().z)
			return (idA > idB) ? true : false;
		return (_storage[idA]->getPos().z < _storage[idB]->getPos().z) ? true : false;
	});
}