#pragma once

#include "Engine.h"

#include "LuaHelpers.h"

class LuaManager final: public Singleton<LuaManager>{
	friend Singleton<LuaManager>;
public:
	~LuaManager();

	void Init();

	void PrintFileContent(cstr const fPath);

	template <class T>
	T ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg);

	template <>
	inline bool ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg);
private:
	lua_State* im_ReadL;
	lua_State* im_WriteL;

	LuaManager();
};

#include "LuaManager.inl"