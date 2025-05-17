#pragma once
#ifndef INTERACT_ELEMENT_H
#define INTERACT_ELEMENT_H
#include <overtime.h>

#define ELEMENT_CLASS_TYPE(type) static elementType getStaticType() { return elementType::type; }\
								virtual inline elementType getType() const override { return getStaticType(); }
enum elementType {
	buttonElement = 0, counterElement, gridElement, ship1Element, ship2Element, ship3Element, ship4Element, unknown
};

class objectPool;
class interactElement {
	friend objectPool;
public:
	interactElement(const std::string& name);
	virtual ~interactElement() = default;

	virtual void onRender() {}
	virtual void onEvent(overtime::event& event) {}

	virtual const glm::vec3& getPos() const = 0;
	virtual const glm::vec2& getSize() const = 0;
	virtual void setPos(const glm::vec3& newPos) = 0;
	virtual void setSize(const glm::vec2& newSize) = 0;

	inline const std::string& getName() const { return _name; };
	virtual inline uint32_t getId() const { return _id; }
	virtual inline elementType getType() const { return elementType::unknown; }

	virtual inline bool isActive() const { return _isActive; }
	virtual void activate() { _isVisible = _isActive = true; }
	virtual void deactivate() { _isVisible = _isActive = false; }

	virtual inline bool isVisible() const { return _isVisible; }
	virtual inline void setVisibility(bool newVisible) { _isVisible = newVisible; }
protected:
	std::string _name;
	uint32_t _id = UINT32_MAX;
	bool _isActive = true;
	bool _isVisible = true;
};
#endif