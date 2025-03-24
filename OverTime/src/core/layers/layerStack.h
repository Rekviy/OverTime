#pragma once
#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "layer.h"

#include <vector>

namespace overtime {
	class layerStack {
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
		unsigned int m_LayerInsertIndex = 0;
	};
}

#endif 