#pragma once

#include "ObjType.hpp"

struct BoolType final: public ObjType{
	BoolType() = default;
	~BoolType() = default;

	bool data;
};