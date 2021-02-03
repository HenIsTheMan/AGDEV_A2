#pragma once

#include "../StateShared.hpp"

class StateAttack final{ //Static class
	_6_DELETED(StateAttack)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateAttack.inl"