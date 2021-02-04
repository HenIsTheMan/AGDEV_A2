#pragma once

#include "../StateShared.hpp"

class StateEnemyEscape final{ //Static class
	_6_DELETED(StateEnemyEscape)
public:
	static glm::vec3 terrainPos;
	static float distSquaredThreshold;
	static Entity* targetEntity;

	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};