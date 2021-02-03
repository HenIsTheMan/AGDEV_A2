#pragma once

#include "ObjType.hpp"

struct BoolType final: public ObjType{
	BoolType() = default;
	virtual ~BoolType() = default;

	bool data;
};