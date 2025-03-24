#pragma once
#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "core/layers/layerStack.h"

namespace overtime {
	class imGuiLayer : public layer {
	public:
		imGuiLayer();
		~imGuiLayer() override;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(timeStep ts) override;
		void pushLayer(layer* layer);
		void begin();
		void end();
	private:
		layerStack m_LayerStack;
	};
}
#endif