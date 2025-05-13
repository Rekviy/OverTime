#pragma once
#ifndef GRID_H
#define GRID_H

#include "ui/themeManager.h"
#include <initializer_list>

class grid {
public:
	class gridCell {
	public:
		enum state {
			normal = 0, shot
		};
		gridCell(const std::vector<std::string>& keys);
		~gridCell();

		std::vector<std::string> _keys;
		overtime::ref<themeManager::style> _style;
		bool _isOccupied = false;
		void changeState(state newState);
		state _currentState = state::normal;
	};


	grid(uint32_t rowCount, uint32_t columnCount, const glm::vec3& startPosition, const glm::vec2& gridCellSize, const std::vector<std::vector<std::string>>& keys);
	~grid();
	void onRender();
	inline const glm::vec3& getPos() const { return _pos; }
	inline const glm::vec2& getSize() const { return _size; }
	inline void setPos(const glm::vec3& newPos) { _pos = newPos; }
	inline void setSize(const glm::vec2& newSize) { _size = newSize; }

	std::vector<gridCell>::iterator begin() { return _storage.begin(); }
	std::vector<gridCell>::iterator end() { return _storage.end(); }

	bool isOccupied(const std::vector<gridCell>::iterator& begin, const std::vector<gridCell>::iterator& end);
	bool isOccupied(const glm::i32vec2& begin, const glm::i32vec2& end);
	void changeState(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end, gridCell::state newState);
	void changeState(const glm::i32vec2& begin, const glm::i32vec2& end, gridCell::state newState);

	void onEvent(overtime::event& event);
	inline uint32_t getColumnCount() const noexcept { return _columnCount; }
	inline uint32_t getRowCount() const noexcept { return _rowCount; }
private:
	bool onWindowResize(overtime::windowResizeEvent& event);
	bool onMouseMoved(overtime::mouseMovedEvent& event);
	bool onMouseButtonPressed(overtime::mouseButtonPressedEvent& event);
	bool onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event);
	uint32_t _columnCount;
	uint32_t _rowCount;
	std::vector<gridCell> _storage;

	glm::vec3 _pos;
	glm::vec2 _size;
};

#endif