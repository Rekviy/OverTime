#pragma once
#ifndef UILAYER_H
#define UILAYER_H

#include "interactElement.h"
#include "objectPool.h"
#include <overtime.h>
#include <unordered_map>

class gameUI {
public:
	gameUI();
	~gameUI() = default;
	template<typename T, typename... Args>
	uint32_t push(Args&&... args)
	{
		OT_ASSERT((std::is_base_of<interactElement, T>::value), "class is not derived from interactElement!");
		return _pool.push(std::make_unique<T>(std::forward<Args>(args)...));
	}
	uint32_t push(overtime::scope<interactElement> element)
	{
		return _pool.push(std::move(element));
	}

	overtime::scope<interactElement> pop(uint32_t id);

	template<typename T>
	T* get(uint32_t id)
	{
		auto* item = _pool.get(id);
		if (item->getType() == T::getStaticType())
			return static_cast<T*>(item);
		return nullptr;
	}

	template<typename T>
	std::vector<T*> get(const std::vector<uint32_t>& ids)
	{
		std::vector<T*> items;
		for (auto id : ids) {
			auto* item = _pool.get(id);
			if (item->getType() == T::getStaticType())
				items.push_back(static_cast<T*>(item));
		}
		return items;
	}

	interactElement* get(uint32_t id);

	void activate(const std::vector<uint32_t>& ids);
	bool activate(uint32_t id);
	uint32_t activateFirst(elementType type);
	void activateAll();
	void deactivate(const std::vector<uint32_t>& ids);
	void deactivate(uint32_t id);
	void deactivateAll();
	bool isExist(uint32_t id);
	std::vector<overtime::scope<interactElement>> setTypeCap(elementType type, uint32_t newCap);
	std::vector<uint32_t>  setTypeActiveCap(elementType type, uint32_t newCap);
	uint32_t checkTypeCap(elementType type) const;
	uint32_t checkTypeActiveCap(elementType type) const;
	inline bool isTypeCapReached(elementType type) const { return _pool.isTypeCapReached(type); }
	inline bool isTypeActiveCapReached(elementType type) const { return _pool.isTypeActiveCapReached(type); }
	inline uint32_t getTypeCap(elementType type) const { return _pool.getTypeCap(type); }
	inline uint32_t getTypeActiveCap(elementType type) const { return  _pool.getTypeActiveCap(type); }

	bool bind(uint32_t childId, uint32_t ParentId);
	bool unBind(uint32_t childId, uint32_t ParentId);
	bool unBindAll(uint32_t unBindFrom);
	const std::vector<uint32_t> getBindings(uint32_t ParentId);
	const std::vector<uint32_t> getParents(uint32_t childId);
	inline const objectPool& getPool() const { return _pool; }
	void onRender();
	void onImGuiRender();
	void onEvent(overtime::event& event);
private:
	objectPool _pool;
	std::unordered_map<uint32_t, std::vector<uint32_t>> _bindings;
	std::unordered_map<uint32_t, std::vector<uint32_t>> _parents;

};
#endif