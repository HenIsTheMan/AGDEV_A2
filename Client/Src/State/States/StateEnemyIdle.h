#pragma once

#include "../StateShared.hpp"

class StateEnemyIdle final{ //Static class
	_6_DELETED(StateEnemyIdle)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateEnemyIdle.inl"