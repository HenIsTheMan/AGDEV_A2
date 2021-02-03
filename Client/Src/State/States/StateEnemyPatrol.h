#pragma once

#include "../StateShared.hpp"

class StateEnemyPatrol final{ //Static class
	_6_DELETED(StateEnemyPatrol)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateEnemyPatrol.inl"