#pragma once

#include "Core.h"

namespace{
	bool LuaErrCheck(lua_State* const L, const int r, const bool printErrMsg);
};