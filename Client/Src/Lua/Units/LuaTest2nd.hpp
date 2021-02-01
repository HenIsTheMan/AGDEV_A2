#pragma once

#include "../LuaHelpers.h"

namespace{
	void LuaTest2nd(){
		struct{
			str myStr = str();
			double myDouble = 0.0;
		} foobar;

		lua_State* const L = luaL_newstate();
		luaL_openlibs(L);

		if(!LuaErrCheck(L, luaL_dofile(L, "Scripts/Test.lua"), true)){
			lua_getglobal(L, "foobar");

			if(lua_istable(L, -1)){
				lua_pushstring(L, "myStr");
				lua_gettable(L, -2);

				foobar.myStr = lua_tostring(L, -1);
				std::cout << foobar.myStr << '\n';

				lua_pop(L, 1);

				lua_pushstring(L, "myDouble");
				lua_gettable(L, -2);

				foobar.myDouble = (double)lua_tonumber(L, -1);
				std::cout << foobar.myDouble << '\n';

				lua_pop(L, 1);
			}
		}

		std::cout << '\n';
		lua_close(L);
	}
}