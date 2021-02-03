#pragma once

#include "../LuaHelpers.h"

namespace{
	int HostFunc(lua_State* const L){
		const float a = (float)lua_tonumber(L, 1);
		const float b = (float)lua_tonumber(L, 2);

		std::cout << "[C++] HostFunc(" << a << ", " << b << ")\n";
		const float c = a * b;
		lua_pushnumber(L, c);

		return 1;
	}

	void LuaTest5th(){
		lua_State* const L = luaL_newstate();
		luaL_openlibs(L);
		lua_register(L, "HostFunc", HostFunc);

		if(!LuaErrCheck(L, luaL_dofile(L, "Scripts/Test5th.lua"), true)){
			lua_getglobal(L, "Perform");

			if(lua_isfunction(L, -1)){
				lua_pushnumber(L, 3.97f);
				lua_pushnumber(L, 3.95f);

				if(!LuaErrCheck(L, lua_pcall(L, 2, 1, 0), true)){
					std::cout << "[C++] " << (float)lua_tonumber(L, -1) << '\n';
				}
			}
		}

		std::cout << '\n';
		lua_close(L);
	}
}