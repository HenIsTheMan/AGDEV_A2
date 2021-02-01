#pragma once

#include "../LuaHelpers.h"

namespace{
	void LuaTest4th(){
		lua_State* const L = luaL_newstate();
		luaL_openlibs(L);

		if(!LuaErrCheck(L, luaL_dofile(L, "Scripts/Test4th.lua"), true)){
			lua_getglobal(L, "GetBook");

			if(lua_isfunction(L, -1)){
				lua_pushinteger(L, 1);

				if(!LuaErrCheck(L, lua_pcall(L, 1, 1, 0), true)){
					lua_pushstring(L, "myDouble");
					lua_gettable(L, -2);

					std::cout << "[C++] " << (double)lua_tonumber(L, -1) << '\n';

					lua_pop(L, 1);
				}
			}
		}

		std::cout << '\n';
		lua_close(L);
	}
}