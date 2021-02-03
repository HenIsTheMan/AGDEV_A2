#pragma once

#include "Engine.h"

#include "Waypt.h"

#include "../ObjPool/ObjPool.h"

class WayptManager final: public Singleton<WayptManager>{
	friend Singleton<WayptManager>;
public:
	~WayptManager();

	void InitWayptPool(const size_t& inactiveSize, const size_t& activeSize);

	void AddWaypt(const glm::vec3& pos);
	void RemoveWaypt(const glm::vec3& pos);

	void ReserveWaypts(const size_t& size);

	Waypt* RetrieveRandWaypt();

	const Waypt* GetWaypt(const glm::vec3& pos) const;
	const Waypt* GetNextWaypt(const glm::vec3& pos) const;
	const std::vector<Waypt*>& GetWaypts() const;
private:
	ObjPool<Waypt>* wayptPool;
	std::vector<Waypt*> waypts;

	WayptManager();
};