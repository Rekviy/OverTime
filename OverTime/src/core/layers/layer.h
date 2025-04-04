#pragma once
#ifndef LAYER_H
#define LAYER_H
#include "events/event.h"
#include "core/timeStep.h"

#include <string>

namespace overtime {
	class  layer {
	public:
		layer(const std::string& name = "Layer");
		virtual ~layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(timeStep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(event& event) {}
		inline const std::string& getName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
#endif
