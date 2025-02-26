#include "layerStack.h"

namespace overtime {
	layerStack::layerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}
	layerStack::~layerStack()
	{
		for (layer* layer : m_Layers)
			delete layer;
	}

	void layerStack::pushLayer(layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}
	void layerStack::pushOverlay(layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}
	void layerStack::popLayer(layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			--m_LayerInsert;
		}
	}
	void layerStack::popOverlay(layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);

	}
}