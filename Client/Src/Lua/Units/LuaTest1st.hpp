#pragma once

#include "../LuaHelpers.h"

namespace{
	void LuaTest1st(){
		struct{
			str name0 = str();
			str name1 = str();
			int val0 = 0;
			double val1 = 0.0;
		} foobar;

		lua_State* const L = luaL_newstate();
		luaL_openlibs(L);

		if(!LuaErrCheck(L, luaL_dofile(L, "Scripts/Test1st.lua"), true)){
			lua_getglobal(L, "a");
			if(lua_isnumber(L, -1)){
				std::cout << (float)lua_tonumber(L, -1) << "\n\n";
			}
		}

		lua_close(L);
	}
}