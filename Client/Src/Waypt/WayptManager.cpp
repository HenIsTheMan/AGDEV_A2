#include "WayptManager.h"

WayptManager::~WayptManager(){
	for(Waypt*& waypt: waypts){
		if(waypt != nullptr){
			delete waypt;
			waypt = nullptr;
		}
	}
}

void WayptManager::AddWaypt(const glm::vec3& pos){
	Waypt* waypt = new Waypt();
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
					waypts[wayptsSize - 1]->nextWaypt = waypts[1];
				} else{
					waypts[1]->nextWaypt = nullptr;
				}
			} else if(i == wayptsSize - 1){
				if(wayptsSize > 2){
					waypts[wayptsSize - 2]->nextWaypt = waypts[0];
				} else{
					waypts[0]->nextWaypt = nullptr;
				}
			} else{
				waypts[static_cast<std::vector<Waypt*, std::allocator<Waypt >>::size_type>(i) - 1]->nextWaypt =
					waypts[static_cast<std::vector<Waypt*, std::allocator<Waypt*>>::size_type>(i) + 1];
			}

			waypts.erase(waypts.begin() + i);
		}
	}
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
	waypts()
{
}