#pragma once

#include "../StateShared.hpp"

class StateEnemyHeal final{ //Static class
	_6_DELETED(StateEnemyHeal)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};