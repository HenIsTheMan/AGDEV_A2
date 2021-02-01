#pragma once

#include "../LuaHelpers.h"

namespace{
	void LuaTest3rd(){
		lua_State* const L = luaL_newstate();
		luaL_openlibs(L);

		if(!LuaErrCheck(L, luaL_dofile(L, "Scripts/Test3rd.lua"), true)){
			lua_getglobal(L, "DoSth");

			if(lua_isfunction(L, -1)){
				lua_pushnumber(L, 3.98f);
				lua_pushnumber(L, 3.92f);

				if(!LuaErrCheck(L, lua_pcall(L, 2, 1, 0), true)){
					std::cout << "[C++] " << (float)lua_tonumber(L, -1) << '\n';
				}
			}
		}

		std::cout << '\n';
		lua_close(L);
	}
}