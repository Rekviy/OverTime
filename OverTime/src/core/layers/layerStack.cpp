#include "layerStack.h"

namespace overtime {
	layerStack::layerStack()
	{
		m_Layers.reserve(4);
	}
	layerStack::~layerStack()
	{
		for (layer* layer : m_Layers) {
			layer->onDetach();
			delete layer;
		}
	}

	void layerStack::pushLayer(layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->onAttach();
	}
	void layerStack::pushOverlay(layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->onAttach();
	}
	void layerStack::popLayer(layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin()+ m_LayerInsertIndex, layer);

		if (it != m_Layers.begin() + m_LayerInsertIndex) {
			layer->onDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}
	void layerStack::popOverlay(layer* overlay)
	{
		auto it = std::find(m_Layers.begin()+ m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.end()) {
			overlay->onDetach();
			m_Layers.erase(it);
		}

	}
}