#pragma once

#include "../StateShared.hpp"

class StateEnemyChase final{ //Static class
	_6_DELETED(StateEnemyChase)
public:
	static float distSquaredThreshold;
	static Entity* targetEntity;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};