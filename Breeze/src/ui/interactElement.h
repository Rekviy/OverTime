#pragma once
#ifndef INTERACT_ELEMENT_H
#define INTERACT_ELEMENT_H
#include <overtime.h>
namespace breeze {
	#define ELEMENT_CLASS_TYPE(type) static elementType getStaticType() { return elementType::type; }\
								virtual inline elementType getType() const override { return getStaticType(); }
	enum elementType {
		buttonElement = 0, counterElement, gridElement, ship1Element, ship2Element, ship3Element, ship4Element, maskOverlay, unknown
	};
	enum elementFlags : uint32_t { active = BIT(0), visible = BIT(1), blocked = BIT(2) };
	class objectPool;
	class interactElement {
		friend objectPool;
	public:
		interactElement(const std::string& name);
		virtual ~interactElement() = default;

		virtual void reset() = 0;

		virtual void onRender() {}
		virtual void onImGui() {}
		virtual void onEvent(overtime::event& event) {}

		virtual const glm::vec3& getPos() const = 0;
		virtual const glm::vec2& getSize() const = 0;
		virtual void setPos(const glm::vec3& newPos) = 0;
		virtual void setSize(const glm::vec2& newSize) = 0;

		inline const std::string& getName() const { return _name; };
		virtual inline uint32_t getId() const { return _id; }
		virtual inline elementType getType() const { return elementType::unknown; }

		virtual void activate() { setFlag(active | visible); }
		virtual void deactivate() { dropFlag(active | visible); }

		virtual inline uint32_t checkFlag(uint32_t flag) const { return _status & flag; }
		virtual inline void setFlag(uint32_t flag) { _status |= flag; }
		virtual inline void dropFlag(uint32_t flag) { _status &= ~flag; }
	protected:
		std::string _name;
		uint32_t _id = UINT32_MAX;
		uint32_t _status = 0;
	};
}
#endif