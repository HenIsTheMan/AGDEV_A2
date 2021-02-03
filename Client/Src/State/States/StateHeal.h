#pragma once

#include "../StateShared.hpp"

class StateHeal final{ //Static class
	_6_DELETED(StateHeal)
public:
	static void Enter(Entity* const entity);
	static void Update(Entity* const entity, const double dt);
	static void Exit(Entity* const entity);
};

#include "StateHeal.inl"