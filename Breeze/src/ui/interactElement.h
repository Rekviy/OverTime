#pragma once
#ifndef INTERACT_ELEMENT_H
#define INTERACT_ELEMENT_H
#include <overtime.h>
class objectPool;
class interactElement {
friend objectPool;
public:
	enum elementType {
		button = 0, counter, ship1, ship2, ship3, ship4, unknown
	};

	interactElement(const std::string& name);
	virtual ~interactElement() = default;
	inline const std::string& getName() const { return _name; };
	virtual inline uint32_t getId() const { return _id; }
	virtual void onRender() {}
	virtual void onEvent(overtime::event& event) {}
	virtual bool isActive() const = 0;
	virtual void activate() {}
	virtual void deactivate() {}
	virtual bool isVisible() const = 0;
	virtual void setVisibility(bool newVisible) {}
	virtual const glm::vec3& getPos() const = 0;
	virtual const glm::vec2& getSize() const = 0;
	virtual void setPos(const glm::vec3& newPos) {}
	virtual void setSize(const glm::vec2& newSize) {}
	virtual inline elementType getType() = 0;
protected:
	std::string _name;
	uint32_t _id = INT32_MAX;
};
#endif