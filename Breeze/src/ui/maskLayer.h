#pragma once
#ifndef MASKLAYER_H
#define MASKLAYER_H
#include "themeManager.h"
#include "interactElement.h"

#include <array>

enum class maskCellState {
	idle = 0, hover, clicked, stateCount
};
class maskCell {
public:
	maskCell(const overtime::ref<style> style);
	~maskCell() = default;

	maskCellState _currentState = maskCellState::idle;
	overtime::ref<style> _style;
	bool _isVisible = true;
};
class maskLayer :public interactElement {
public:
	maskLayer(const std::string& name, uint32_t rowCount, uint32_t columnCount, const glm::vec3& startPosition, const glm::vec2& cellSize,
		std::array<std::string, (uint32_t)maskCellState::stateCount>&& keys,
		std::function<bool(maskLayer*)>&& funcOnRelease = [](maskLayer* btn) {return true; }, bool isActive = false);
	~maskLayer() = default;
	virtual void onRender() noexcept override;
	virtual void onImGui() noexcept override;
	virtual void onEvent(overtime::event& event) override;

	virtual inline const glm::vec3& getPos() const override { return _pos; }
	virtual inline const glm::vec2& getSize() const override { return _size; }
	virtual inline void setPos(const glm::vec3& newPos) override { _pos = newPos; updateBounds();}
	virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; updateBounds();}
	
	virtual void activate() override { interactElement::activate(); updateBounds(); }

	void reset();

	inline void setCellVisibility(uint32_t cell, bool newVisibility) { _storage.at(cell)._isVisible = newVisibility; }
	void setCellVisibility(const glm::i32vec2& begin, const glm::i32vec2& end, bool newVisibility);
	bool isCellVisible(uint32_t cell) const { return _storage.at(cell)._isVisible; }

	inline uint32_t getColumnCount() const { return _columnCount; }
	inline uint32_t getRowCount() const { return _rowCount; }
	inline uint32_t getCellClicked() const { return _cellClicked; }
	ELEMENT_CLASS_TYPE(maskOverlay)
private:
	void updateBounds();
	bool onWindowResize(overtime::windowResizeEvent& event);
	bool onMouseMoved(overtime::mouseMovedEvent& event);
	bool onMouseButtonPressed(overtime::mouseButtonPressedEvent& event);
	bool onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event);

	uint32_t _columnCount;
	uint32_t _rowCount;
	glm::vec3 _pos;
	glm::vec2 _size;
	glm::vec4 _bounds;
	uint32_t _cellClicked = 0;
	std::vector<maskCell> _storage;
	std::array<std::string, (uint32_t)maskCellState::stateCount> _keys;
	std::function<bool(maskLayer*)> _funcOnRelease;
};
#endif