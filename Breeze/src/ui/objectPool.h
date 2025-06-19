#pragma once
#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include "interactElement.h"

#include<overtime.h>
#include <map>

class objectPool {
public:
	objectPool();
	~objectPool() = default;
	std::map<uint32_t, overtime::scope<interactElement>>::const_iterator cbegin() { return _storage.cbegin(); }
	std::map<uint32_t, overtime::scope<interactElement>>::const_iterator cend() { return _storage.cend(); }
	std::map<uint32_t, overtime::scope<interactElement>>::iterator begin() { return _storage.begin(); }
	std::map<uint32_t, overtime::scope<interactElement>>::iterator end() { return _storage.end(); }

	std::map<elementType, std::vector<uint32_t>>::iterator activeBegin() { return _typeActiveKeys.begin(); }
	std::map<elementType, std::vector<uint32_t>>::iterator activeEnd() { return _typeActiveKeys.end(); }

	std::map<elementType, std::vector<uint32_t>>::iterator typeBegin() { return _typeKeys.begin(); }
	std::map<elementType, std::vector<uint32_t>>::iterator typeEnd() { return _typeKeys.end(); }
	uint32_t push(overtime::scope<interactElement> element);
	overtime::scope<interactElement> pop(uint32_t id);
	interactElement* get(uint32_t id);
	std::map<uint32_t, overtime::scope<interactElement>>::iterator find(uint32_t id);
	void activateAll();
	bool activate(uint32_t id);
	uint32_t activateFirst(elementType type);
	void deactivateAll();
	void deactivate(uint32_t id);
	bool isExist(uint32_t id);
	std::vector<overtime::scope<interactElement>> setTypeCap(elementType type, uint32_t newCap);
	std::vector<uint32_t> setTypeActiveCap(elementType type, uint32_t newCap);
	uint32_t inline getTypeCap(elementType type) const { return _typeCaps.at(type); }
	uint32_t inline getTypeActiveCap(elementType type) const { return _typeActiveCaps.at(type); }
	inline bool isTypeActiveCapReached(elementType type) const { return  _typeActiveCaps[type] == _typeActiveKeys.at(type).size(); }
	inline bool isTypeCapReached(elementType type) const { return  _typeCaps[type] == _typeKeys.at(type).size(); }
	uint32_t checkTypeCap(elementType type) const;
	uint32_t checkTypeActiveCap(elementType type) const;
	inline uint32_t size() const { return (uint32_t)_storage.size(); }
private:
	void zSort(std::vector<uint32_t>::iterator begin, std::vector<uint32_t>::iterator end);
	std::map<uint32_t, overtime::scope<interactElement>> _storage;
	std::map<elementType, std::vector<uint32_t>> _typeKeys;
	std::map<elementType, std::vector<uint32_t>> _typeActiveKeys;
	std::vector<uint32_t> _typeCaps;
	std::vector<uint32_t> _typeActiveCaps;
	uint32_t _idCounter = 0;
};
#endif 