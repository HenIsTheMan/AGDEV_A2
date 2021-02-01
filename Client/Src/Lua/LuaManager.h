#pragma once

#include "Engine.h"

class LuaManager final: public Singleton<LuaManager>{
	friend Singleton<LuaManager>;
public:
	~LuaManager();

	void Init();

	void PrintFileContent(cstr const fPath);
private:
	lua_State* im_ReadL;
	lua_State* im_WriteL;

	LuaManager();
};