#pragma once

#include "ObjType.hpp"

struct DoubleType final: public ObjType{
	DoubleType() = default;
	~DoubleType() = default;

	double data;
};