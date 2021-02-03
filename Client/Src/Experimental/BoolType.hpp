#pragma once

#include "ObjType.hpp"

struct BoolType final: public ObjType{ //Wrapper class
	BoolType() = default;
	~BoolType() = default;

	bool data;
};