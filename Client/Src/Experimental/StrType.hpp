#pragma once

#include "ObjType.hpp"

#include <string>

struct StrType final: public ObjType{
	StrType() = default;
	~StrType() = default;

	std::string data;
};