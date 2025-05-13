#pragma once
#ifndef SHIP_H
#define SHIP_H
#include "ui/interactElement.h"
#include "ui/themeManager.h"
#include <initializer_list>

class ship :public interactElement {
public:
	class shipCell {
	public:
		enum state {
			normal = 0, placingAllowed, placingDenied, shot
		};
		shipCell(glm::vec3 position, const std::vector<std::string>& keys);
		~shipCell();

		void changeState(state newState);
		glm::vec3 _pos;
		overtime::ref<themeManager::style> _style;
		std::vector<std::string> _keys;
	private:
		state _currentState = state::normal;
	};
	ship(const std::string& name, const glm::vec3& position, uint32_t length, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
		const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving = [](ship* btn) {return true; });
	virtual ~ship() override;
	virtual void onRender()override;
	virtual void onEvent(overtime::event& event) override;
	virtual inline bool isActive() const override { return _isActive; }
	virtual void activate() override;
	virtual void deactivate() override;
	virtual inline bool isVisible() const override { return _isVisible; }
	virtual inline void setVisibility(bool newVisible) override { _isVisible = newVisible; }
	inline bool isPlaced() const { return _isPlaced; }
	inline void setPlaced(bool newPlaced) { _isPlaced = newPlaced; }
	inline void switchPlaced() { _isPlaced = !_isPlaced; }
	virtual const glm::vec3& getPos() const override;
	virtual inline const glm::vec2& getSize() const override { return _size; }
	virtual void setPos(const glm::vec3& newPos) override;
	virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; }
	std::vector<shipCell>::iterator begin() { return _cells.begin(); }
	std::vector<shipCell>::iterator end() { return _cells.end(); }
	virtual inline elementType getType() override { return _type; }
	void changeState(shipCell::state newState);
	void changeState(uint32_t cell, shipCell::state newState);
private:
	
	bool onWindowResize(overtime::windowResizeEvent& event);
	bool onMouseMoved(overtime::mouseMovedEvent& event);
	bool onMouseButtonPressed(overtime::mouseButtonPressedEvent& event);
	bool onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event);
	bool onKeyPressed(overtime::keyPressedEvent& event);
	void updateBounds();
	bool _isActive = true;
	bool _isVisible = true;
	bool _isPlaced = false;
	bool _isDragging = false;
	uint32_t _cellClicked = 0;
	elementType _type;
	//glm::vec2 _clickOffset;
	glm::vec2 _size;
	glm::vec4 _bounds;
	float _rotation = 0.0f;
	std::vector<shipCell> _cells;
	std::function<bool(ship*)> _funcOnMoving;
	std::function<bool(ship*)> _funcOnRelease;
};

#endif