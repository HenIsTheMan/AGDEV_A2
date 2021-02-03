#pragma once

#include "../StateShared.hpp"

class StateEscape final{ //Static class
	_6_DELETED(StateEscape)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateEscape.inl"