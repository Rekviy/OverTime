#pragma once
#ifndef LAYERSTACK_H
#define LAYERSTACK_H
#include "core.h"
#include "layer.h"

namespace overtime {
	class OVERTIME_API layerStack {
	public:
		layerStack();
		~layerStack();

		void pushLayer(layer* layer);
		void pushOverlay(layer* overlay);
		void popLayer(layer* layer);
		void popOverlay(layer* overlay);

		std::vector<layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<layer*> m_Layers;
		std::vector<layer*>::iterator m_LayerInsert;
	};
}


#endif 