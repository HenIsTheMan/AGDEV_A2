#include "WayptManager.h"

#include "Engine.h"

WayptManager::~WayptManager(){
	if(wayptPool != nullptr){
		wayptPool->Destroy();
		wayptPool = nullptr;
	}
}

void WayptManager::InitWayptPool(const size_t& inactiveSize, const size_t& activeSize){
	wayptPool->Init(inactiveSize, activeSize);
}

void WayptManager::AddWaypt(const glm::vec3& pos){
	Waypt* waypt = wayptPool->ActivateObj();
	waypt->pos = pos;
	
	const int wayptsSize = (int)waypts.size();
	if(wayptsSize > 0){
		waypts[static_cast<std::vector<Waypt*, std::allocator<Waypt*>>::size_type>(wayptsSize) - 1]->nextWaypt = waypt;
		waypt->nextWaypt = waypts[0];
	}

	waypts.emplace_back(waypt);
}

void WayptManager::RemoveWaypt(const glm::vec3& pos){
	const int wayptsSize = (int)waypts.size();
	for(int i = 0; i < wayptsSize; ++i){
		const Waypt* const waypt = waypts[i];

		if(waypt != nullptr && waypt->pos == pos){
			if(i == 0){
				if(wayptsSize > 2){
					waypts[static_cast<std::vector<Waypt*, std::allocator<Waypt*>>::size_type>(wayptsSize) - 1]->nextWaypt = waypts[1];
				} else{
					waypts[1]->nextWaypt = nullptr;
				}
			} else if(i == wayptsSize - 1){
				if(wayptsSize > 2){
					waypts[static_cast<std::vector<Waypt*, std::allocator<Waypt*>>::size_type>(wayptsSize) - 2]->nextWaypt = waypts[0];
				} else{
					waypts[0]->nextWaypt = nullptr;
				}
			} else{
				waypts[static_cast<std::vector<Waypt*, std::allocator<Waypt >>::size_type>(i) - 1]->nextWaypt =
					waypts[static_cast<std::vector<Waypt*, std::allocator<Waypt*>>::size_type>(i) + 1];
			}

			wayptPool->DeactivateObj(waypt);
			waypts.erase(waypts.begin() + i);
		}
	}
}

Waypt* WayptManager::RetrieveRandWaypt(){
	return waypts[PseudorandMinMax(0, (int)waypts.size() - 1)];
}

const Waypt* WayptManager::GetWaypt(const glm::vec3& pos) const{
	const int wayptsSize = (int)waypts.size();

	for(int i = 0; i < wayptsSize; ++i){
		Waypt* const waypt = waypts[i];
		if(waypt != nullptr && waypt->pos == pos){
			return waypt;
		}
	}

	return nullptr;
}

const Waypt* WayptManager::GetNextWaypt(const glm::vec3& pos) const{
	const int wayptsSize = (int)waypts.size();

	for(int i = 0; i < wayptsSize; ++i){
		Waypt* const waypt = waypts[i];
		if(waypt != nullptr && waypt->pos == pos){
			return waypt->nextWaypt;
		}
	}

	return nullptr;
}

void WayptManager::ReserveWaypts(const size_t& size){
	waypts.reserve(size);
}

const std::vector<Waypt*>& WayptManager::GetWaypts() const{
	return waypts;
}

WayptManager::WayptManager():
	wayptPool(ObjPool<Waypt>::GetObjPtr()),
	waypts()
{
}