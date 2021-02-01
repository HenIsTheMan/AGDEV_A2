#include "LuaHelpers.h"

namespace{
	bool LuaErrCheck(lua_State* const L, const int r, const bool printErrMsg){
		if(r != LUA_OK){
			if(printErrMsg){
				puts(lua_tostring(L, -1));
			}
			return true;
		}
		return false;
	}
};