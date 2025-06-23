#pragma once
#pragma once
#ifndef GRID_H
#define GRID_H

#include "ui/interactElement.h"
#include "ui/themeManager.h"

#include <array>
#include <unordered_map>
namespace breeze {
	enum class gridCellState {
		normal = 0, shot, stateCount
	};
	class grid : public interactElement {
	public:
		class gridCell {
		public:
			gridCell(const std::array<std::string, (uint32_t)gridCellState::stateCount>& keys);
			~gridCell() = default;
			void changeState(gridCellState newState);

			gridCellState _currentState = gridCellState::normal;
			overtime::ref<style> _style;
			bool _isOccupied = false;
			//todo move keys to grid
			std::array<std::string, (uint32_t)gridCellState::stateCount> _keys;
		private:

		};


		grid(const std::string& name, uint32_t rowCount, uint32_t columnCount, const glm::vec3& startPosition, const glm::vec2& gridCellSize,
			const std::vector<std::array<std::string, (uint32_t)gridCellState::stateCount>>& keys, bool isActive = false);
		~grid() = default;

		virtual void reset() override;

		virtual void onRender() noexcept override;
		virtual void onImGui() noexcept override;

		virtual inline const glm::vec3& getPos() const override { return _pos; }
		virtual inline const glm::vec2& getSize() const override { return _size; }

		virtual inline void setPos(const glm::vec3& newPos) override { _pos = newPos; }
		virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; }

		std::vector<gridCell>::iterator begin() { return _storage.begin(); }
		std::vector<gridCell>::iterator end() { return _storage.end(); }

		auto placingsBegin() { return _placings.begin(); }
		auto placingsEnd() { return _placings.end(); }

		auto tempPlacingsBegin() { return _tempPlacings.begin(); }
		auto tempPlacingsEnd() { return _tempPlacings.end(); }

		void setOccupation(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end, bool newOccupation);
		void setOccupation(const glm::i32vec2& begin, const glm::i32vec2& end, bool newOccupation);

		bool isOccupied(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end) noexcept;
		bool isOccupied(const glm::i32vec2& begin, const glm::i32vec2& end) noexcept;

		bool addPlacement(uint32_t itemId, const glm::i32vec2& begin, const glm::i32vec2& end);
		bool addPlacement(uint32_t itemId, const std::pair<glm::i32vec2, glm::i32vec2>& position);
		void removePlacement(uint32_t itemId);

		const std::pair<glm::i32vec2, glm::i32vec2>* getPlacement(uint32_t itemId) const noexcept;
		uint32_t getItemAt(const glm::i32vec2& position) const noexcept;
		uint32_t getItemAt(uint32_t index) const noexcept;
		std::vector<uint32_t> getAllItems() const noexcept;

		inline bool isPlaced(uint32_t id) const { return (_placings.find(id) != _placings.end()) || (_tempPlacings.find(id) != _tempPlacings.end()); }

		inline uint32_t placementCount() const { return (uint32_t)_placings.size(); }

		bool addTempPlacement(uint32_t itemId, const glm::i32vec2& begin, const glm::i32vec2& end);
		bool addTempPlacement(uint32_t itemId, const std::pair<glm::i32vec2, glm::i32vec2>& position);
		bool acceptPlacing(uint32_t itemId);
		void rejectPlacing(uint32_t itemId) noexcept;
		uint32_t getTempItemAt(const glm::i32vec2& position) const noexcept;
		uint32_t getTempItemAt(uint32_t index) const noexcept;

		inline uint32_t tempPlacementCount() const { return (uint32_t)_tempPlacings.size(); }

		void changeState(std::vector<gridCell>::iterator begin, std::vector<gridCell>::iterator end, gridCellState newState);
		void changeState(const glm::i32vec2& begin, const glm::i32vec2& end, gridCellState newState);
		void changeState(const glm::i32vec2& cellPos, gridCellState newState);
		void changeState(uint32_t cellIndex, gridCellState newState);

		//gridCellState getState(std::vector<gridCell>::iterator& begin, std::vector<gridCell>::iterator& end) const;
		//gridCellState getState(const glm::i32vec2& begin, const glm::i32vec2& end) const;
		gridCellState getState(const glm::i32vec2& cellPos) const { return _storage[cellPos.y * _columnCount + cellPos.x]._currentState; }
		gridCellState getState(uint32_t cellIndex) const { return _storage[cellIndex]._currentState; }

		inline uint32_t getColumnCount() const { return _columnCount; }
		inline uint32_t getRowCount() const { return _rowCount; }

		ELEMENT_CLASS_TYPE(gridElement)
	private:
		uint32_t _columnCount;
		uint32_t _rowCount;
		std::vector<gridCell> _storage;
		std::unordered_map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>> _placings;
		std::unordered_map<uint32_t, std::pair<glm::i32vec2, glm::i32vec2>> _tempPlacings;
		glm::vec3 _pos;
		glm::vec2 _size;
	};

	class gridException : public std::exception {
	public:
		gridException(const char* const message)
			: exception(message)
		{}
		virtual ~gridException() = default;
	};

	class gridOutOfRange : public gridException {//, public std::out_of_range???
	public:
		gridOutOfRange(const char* const message, const glm::i32vec2& begin, const glm::i32vec2& end)
			: gridException(message), _rangeBegin(begin), _rangeEnd(end)
		{}
		virtual ~gridOutOfRange() = default;
		std::pair< glm::i32vec2, glm::i32vec2> getRange()
		{
			return { _rangeBegin , _rangeEnd };
		}
	private:
		glm::i32vec2 _rangeBegin;
		glm::i32vec2 _rangeEnd;
	};
}
#endif