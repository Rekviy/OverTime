#pragma once
#ifndef INTERACT_ELEMENT_H
#define INTERACT_ELEMENT_H
#include <overtime.h>
class interactElement {
public:
	virtual ~interactElement() = default;
	virtual void onRender() = 0;
	virtual void onEvent(overtime::event& e) = 0;
	virtual const glm::vec3& getPos() const = 0;
	virtual const glm::vec2& getSize() const  = 0;
	virtual void setPos(const glm::vec3& newPos) = 0;
	virtual void setSize(const glm::vec2& newSize) = 0;
};
#endif