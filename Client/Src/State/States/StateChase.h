#pragma once

#include "../StateShared.hpp"

class StateChase final{ //Static class
	_6_DELETED(StateChase)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateChase.inl"