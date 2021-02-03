#pragma once

#include "../StateShared.hpp"

#include "../../Waypt/WayptManager.h"

class StateEnemyPatrol final{ //Static class
	_6_DELETED(StateEnemyPatrol)
public:
	static WayptManager* const wayptManager;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};