#pragma once
#ifndef IMGUILAYER_H
#define IMGUILAYER_H
#include "core/layer.h"

namespace overtime {
	class imGuiLayer : public layer {
	public:
		imGuiLayer();
		~imGuiLayer() override;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;
		void begin();
		void end();
	};
}
#endif