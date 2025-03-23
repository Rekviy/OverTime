#pragma once
#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "core/layerStack.h"

namespace overtime {
	class imGuiLayer : public layer {
	public:
		imGuiLayer();
		~imGuiLayer() override;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		void pushLayer(layer* layer);
		void begin();
		void end();
	private:
		layerStack m_LayerStack;
	};
}
#endif