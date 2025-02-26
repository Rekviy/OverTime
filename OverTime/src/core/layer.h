#pragma once
#ifndef LAYER_H
#define LAYER_H

#include "core.h"
#include "events/event.h"

namespace overtime {
	class OVERTIME_API layer {
	public:
		layer(const std::string& name = "Layer");
		virtual ~layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(event& event) {}
		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
#endif // !1
