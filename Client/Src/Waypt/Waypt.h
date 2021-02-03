#pragma once

#include "Engine.h"

struct Waypt final{
public:
	Waypt();
	~Waypt() = default;

	glm::vec3 pos;
	Waypt* nextWaypt;
};