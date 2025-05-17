#include "objectPool.h"

using namespace overtime;

objectPool::objectPool()
{
	_typeCaps.resize(elementType::unknown + 1, UINT32_MAX);
	_typeActiveCaps.resize(elementType::unknown + 1, UINT32_MAX);
}

objectPool::~objectPool()
{}

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

		if (it->second->isActive() && _typeActiveKeys[type].size() < _typeActiveCaps[type])
			_typeActiveKeys[type].push_back(it->first);
		return it->first;
	}
	return -1;
}


overtime::scope<interactElement> objectPool::pop(uint32_t id)
{
	auto& it = _storage.extract(id);
	auto& vec = _typeKeys.at(it.mapped()->getType());
	auto& vecIt = std::find(vec.begin(), vec.end(), id);
	//std::swap(vecIt, vec.end() - 1);
	vec.erase(vecIt);
	if (it.mapped()->isActive()) {
		vec = _typeActiveKeys.at(it.mapped()->getType());
		vecIt = std::find(vec.begin(), vec.end(), id);
		//std::swap(vecIt, vec.end() - 1);
		vec.erase(vecIt);
	}

	return std::move(it.mapped());
}

interactElement& objectPool::get(uint32_t id)
{
	//todo add checking for is element == _storage.end before return
	return *_storage.at(id);
}

std::map<uint32_t, overtime::scope<interactElement>>::iterator objectPool::find(uint32_t id)
{
	return _storage.find(id);
}

void objectPool::activate(uint32_t id)
{
	if (!_storage.at(id)->isActive()) {
		if (_typeActiveKeys[_storage.at(id)->getType()].size() + 1 < _typeActiveCaps[_storage.at(id)->getType()]) {
			_storage.at(id)->activate();

			_typeActiveKeys[_storage.at(id)->getType()].push_back(id);
		}
	}

}

uint32_t objectPool::activateFirst(elementType type)
{
	if (_typeActiveKeys[type].size() < _typeActiveCaps[type]) {
		for (auto id : _typeKeys[type]) {
			if (!_storage.at(id)->isActive()) {
				_storage.at(id)->activate();
				_typeActiveKeys[_storage.at(id)->getType()].push_back(id);
				return id;
			}
		}
	}
	return -1;
}

void objectPool::deactivate(uint32_t id)
{
	_storage.at(id)->deactivate();
	auto& activeVec = _typeActiveKeys[_storage.at(id)->getType()];
	auto& vecIt = std::find(activeVec.begin(), activeVec.end(), id);
	//std::swap(vecIt, vec.end() - 1);
	activeVec.erase(vecIt);

}

bool objectPool::isExist(uint32_t id)
{
	if (_storage.find(id) != _storage.end()) return true;
	return false;
}

std::vector<overtime::scope<interactElement>> objectPool::setTypeCap(elementType type, uint32_t newCap)
{
	std::vector<overtime::scope<interactElement>> removed;
	if (type > _typeCaps.size()) {
		_typeCaps.reserve(type);
	}

	if (_typeKeys[type].size() > newCap) {
		if (_typeActiveCaps.at(type) > newCap) setTypeActiveCap(type, newCap);

		uint32_t diff = (uint32_t)_typeKeys[type].size() - newCap;
		removed.reserve(diff);
		auto& vec = _typeKeys[type];

		for (uint32_t i = diff; i > 0;) {
			auto& it = vec.begin() + --i;
			if (_storage.at(*it)->isActive()) deactivate(*it);

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
		auto& itStart = activeVec.begin(), itEnd = activeVec.begin() + (_typeActiveKeys[type].size() - newCap);

		for (; itStart != itEnd; ++itStart) {
			deactivated.push_back(*itStart);
			_storage.at(*itStart)->deactivate();
		}
		activeVec.erase(itStart, itEnd);
	}
	_typeActiveCaps[type] = newCap;
	return deactivated;
}

uint32_t objectPool::checkTypeCap(elementType type) const
{
	if (type > _typeCaps.size()) {
		OT_ASSERT(false, "Type doesn't exist!");
	}
	return (uint32_t)_typeKeys.at(type).size();
}

uint32_t objectPool::checkTypeActiveCap(elementType type) const
{
	if (type > _typeActiveCaps.size()) {
		OT_ASSERT(false, "Type doesn't exist!");
	}
	return (uint32_t)_typeActiveKeys.at(type).size();
}


