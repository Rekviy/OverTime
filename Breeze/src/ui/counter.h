#pragma once
#ifndef COUNTER_H
#define COUNTER_H
#include "interactElement.h"
#include "themeManager.h"
class counter : public interactElement {
public:
	counter(const std::string& name, const glm::vec3& position, const glm::vec2& size, uint32_t startNumber, uint32_t endNumber, const std::vector<std::string>& keys);
	virtual ~counter() override;
	virtual void onRender() override;
	virtual void onEvent(overtime::event& e) override;
	virtual inline const glm::vec3& getPos() const override { return _pos; }
	virtual inline const glm::vec2& getSize() const override { return _size; }
	virtual inline void setPos(const glm::vec3& newPos) override { _pos = newPos; }
	virtual inline void setSize(const glm::vec2& newSize) override { _size = newSize; }
	inline uint32_t operator++() { if (_count < _end)++_count; updateStyle(); return _count; }
	inline uint32_t operator--() { if (_count > _start)--_count; updateStyle(); return _count; }
	inline uint32_t operator++(int) { uint32_t temp; (_count < _end) ? temp = _count++ : temp = _count; updateStyle(); return temp; }
	inline uint32_t operator--(int) { uint32_t temp; (_count > _start) ? temp = _count-- : temp = _count; updateStyle(); return temp; }
	uint32_t count() const { return _count; }
	void reset();
	ELEMENT_CLASS_TYPE(counterElement)
private:
	
	void updateStyle();
	uint32_t _count = 0;
	uint32_t _start = 0;
	uint32_t _end = 0;
	glm::vec3 _pos;
	glm::vec2 _size;
	std::vector<std::string> _keys;
	overtime::ref<style> _style;
};

#endif