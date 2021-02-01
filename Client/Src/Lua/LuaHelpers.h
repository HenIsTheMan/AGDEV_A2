#pragma once

#include "Core.h"

bool LuaErrCheck(lua_State* const L, const int r, const bool printErrMsg);