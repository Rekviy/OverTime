#pragma once
#ifndef GRID_H
#define GRID_H

#include "ui/interactElement.h"
#include "ui/themeManager.h"
#include <initializer_list>
#include <map>
class grid : public interactElement {
public:
	class gridCell {
	public:
		enum state {
			normal = 0, shot
		};
		gridCell(const std::vector<std::string>& keys);
		~gridCell();
		//todo move keys to grid
		std::vector<std::string> _keys;
		overtime::ref<style> _style;
		bool _isOccupied = false;
		void changeState(state newState);
		state _currentState = state::normal;
	};


	grid(const std::string& name, uint32_t rowCount, uint32_t columnCount, const glm::vec3& startPosition, const glm::vec2& gridCellSize, const std::vector<std::vector<std::string>>& keys);
	~grid();
	void reset();
	virtual inline const glm::vec3& getPos() const override { return _pos; }
	virtual inline const glm::vec2& getSize() const override { return _size; }
	virtual inline void setPos(const glm::vec3& newPos) override { _pos = newPos; }
	virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; }

	std::vector<gridCell>::iterator begin() { return _storage.begin(); }
	std::vector<gridCell>::iterator end() { return _storage.end(); }
	std::map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>>::iterator placingsBegin() { return _placings.begin(); }
	std::map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>>::iterator placingsEnd() { return _placings.end(); }
	std::map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>>::iterator tempPlacingsBegin() { return _tempPlacings.begin(); }
	std::map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>>::iterator tempPlacingsEnd() { return _tempPlacings.end(); }

	void setOccupation(const std::vector<gridCell>::iterator& begin, const std::vector<gridCell>::iterator& end, bool newOccupation);
	void setOccupation(const glm::i32vec2& begin, const glm::i32vec2& end, bool newOccupation);
	bool isOccupied(const std::vector<gridCell>::iterator& begin, const std::vector<gridCell>::iterator& end);
	bool isOccupied(const glm::i32vec2& begin, const glm::i32vec2& end);
	bool addPlacement(uint32_t itemId, const glm::i32vec2& begin, const glm::i32vec2& end);
	void removePlacement(uint32_t itemId);
	const std::pair<glm::i32vec2, glm::i32vec2>& getPlacement(uint32_t itemId) const;
	uint32_t getItemAt(const glm::i32vec2& position) const;
	std::vector<uint32_t> getAllItems() const;
	inline bool isPlaced(uint32_t id) const { return (_placings.find(id) != _placings.end()) || (_tempPlacings.find(id) != _tempPlacings.end()); }
	inline uint32_t placementCount() const { return (uint32_t)_placings.size(); }

	bool addTempPlacement(uint32_t itemId, const glm::i32vec2& begin, const glm::i32vec2& end);
	bool acceptPlacing(uint32_t itemId);
	void rejectPlacing(uint32_t itemId);
	inline uint32_t tempPlacementCount() const { return (uint32_t)_tempPlacings.size(); }
	void changeState(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end, gridCell::state newState);
	void changeState(const glm::i32vec2& begin, const glm::i32vec2& end, gridCell::state newState);
	void changeState(const glm::i32vec2& cellPos, gridCell::state newState);
	void changeState(uint32_t cellIndex, gridCell::state newState);

	//gridCell::state getState(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end) const;
	//gridCell::state getState(const glm::i32vec2& begin, const glm::i32vec2& end) const;
	gridCell::state getState(const glm::i32vec2& cellPos) const { return _storage[cellPos.y * _rowCount + cellPos.x]._currentState; }
	gridCell::state getState(uint32_t cellIndex) const { return _storage[cellIndex]._currentState; }

	virtual void onRender() override;
	inline uint32_t getColumnCount() const noexcept { return _columnCount; }
	inline uint32_t getRowCount() const noexcept { return _rowCount; }
	ELEMENT_CLASS_TYPE(gridElement)
private:
	uint32_t _columnCount;
	uint32_t _rowCount;
	std::vector<gridCell> _storage;
	std::map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>> _placings;
	std::map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>> _tempPlacings;
	glm::vec3 _pos;
	glm::vec2 _size;
};

#endif