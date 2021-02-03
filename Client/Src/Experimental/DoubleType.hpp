#pragma once

#include "ObjType.hpp"

struct DoubleType final: public ObjType{ //Wrapper class
	DoubleType() = default;
	~DoubleType() = default;

	double data;
};