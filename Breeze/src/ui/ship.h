#pragma once
#ifndef SHIP_H
#define SHIP_H
#include "interactElement.h"
#include "themeManager.h"
enum class shipState { normal = 0, dragging, destroyed  };
enum shipCellState { normal = 0, placingAllowed, placingDenied, shot };
class ship :public interactElement {
public:
	class shipCell {
	public:

		shipCell(glm::vec3 position, const std::vector<std::string>& keys);
		~shipCell();

		void changeState(shipCellState newState);
		inline shipCellState getState() const { return _currentState; }
		glm::vec3 _pos;
		overtime::ref<themeManager::style> _style;
		std::vector<std::string> _keys;
	private:
		shipCellState _currentState = shipCellState::normal;
	};
	virtual ~ship() override;
	virtual void onRender() override;
	virtual void onEvent(overtime::event& event) override;
	virtual inline void activate() override;
	virtual inline void deactivate() override;

	virtual const glm::vec3& getPos() const override;
	virtual inline const glm::vec2& getSize() const override { return _size; }
	virtual void setPos(const glm::vec3& newPos) override;
	virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; }
	inline uint32_t length() const { return (uint32_t)_cells.size(); }
	std::vector<shipCell>::iterator begin() { return _cells.begin(); }
	std::vector<shipCell>::iterator end() { return _cells.end(); }

	shipCellState getState() const;
	shipCellState getState(uint32_t cell) const;
	void changeState(shipCellState newState);
	void changeState(uint32_t cell, shipCellState newState);

	inline void setRotation(float radians) { _angle = radians; setPos(_cells.begin()->_pos); }
	inline float getRotation() const { return _angle; }


	void changeShipState(shipState newState);
	inline shipState getShipState() const { return _currentState; }
protected:
	ship(const std::string& name, const glm::vec3& position, uint32_t length, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
		const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving);

	bool onWindowResize(overtime::windowResizeEvent& event);
	bool onMouseMoved(overtime::mouseMovedEvent& event);
	bool onMouseButtonPressed(overtime::mouseButtonPressedEvent& event);
	bool onMouseButtonReleased(overtime::mouseButtonReleasedEvent& event);
	bool onKeyPressed(overtime::keyPressedEvent& event);
	void updateBounds();
	void calculateNewPos(uint32_t cellClicked, const glm::vec3& newPos);
	shipState _currentState = shipState::normal;
	uint32_t _cellClicked = 0;
	//glm::vec2 _clickOffset;
	glm::vec2 _size;
	glm::vec4 _bounds;
	float _angle = 0.0f;
	std::vector<shipCell> _cells;
	std::function<bool(ship*)> _funcOnMoving, _funcOnRelease, _funcOnPress;
};


class ship1 : public ship {
public:
	ship1(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
		const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving = [](ship* btn) {return true; });
	ELEMENT_CLASS_TYPE(ship1Element)
};
class ship2 : public ship {
public:
	ship2(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
		const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving = [](ship* btn) {return true; });
	ELEMENT_CLASS_TYPE(ship2Element)
};
class ship3 : public ship {
public:
	ship3(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
		const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving = [](ship* btn) {return true; });
	ELEMENT_CLASS_TYPE(ship3Element)
};
class ship4 : public ship {
public:
	ship4(const std::string& name, const glm::vec3& position, const glm::vec2& cellSize, const std::vector<std::vector<std::string>>& keys,
		const std::function<bool(ship*)>& funcOnPress, const std::function<bool(ship*)>& funcOnRelease, const std::function<bool(ship*)>& funcOnMoving = [](ship* btn) {return true; });
	ELEMENT_CLASS_TYPE(ship4Element)
};

#endif