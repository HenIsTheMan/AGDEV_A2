template <class T>
T LuaManager::Read(cstr const fPath, cstr const varName, const bool printErrMsg){
	assert(false);
	return T();
}

template <>
bool LuaManager::Read(cstr const fPath, cstr const varName, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, varName);
		if(lua_isboolean(im_ReadL, -1)){
			return lua_toboolean(im_ReadL, -1);
		}
	}

	return false;
}

template <>
float LuaManager::Read(cstr const fPath, cstr const varName, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, varName);
		if(lua_isnumber(im_ReadL, -1)){
			return (float)lua_tonumber(im_ReadL, -1);
		}
	}

	return 0.0f;
}

template <>
int LuaManager::Read(cstr const fPath, cstr const varName, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, varName);
		if(lua_isinteger(im_ReadL, -1)){
			return (int)lua_tointeger(im_ReadL, -1);
		}
	}

	return 0;
}

template <>
cstr LuaManager::Read(cstr const fPath, cstr const varName, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, varName);
		if(lua_isstring(im_ReadL, -1)){
			return lua_tostring(im_ReadL, -1);
		}
	}

	return cstr();
}

template <class T>
T LuaManager::ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg){
	assert(false);
	return T();
}

template <>
bool LuaManager::ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, tableName);

		if(lua_istable(im_ReadL, -1)){
			lua_pushstring(im_ReadL, keyName);
			lua_gettable(im_ReadL, -2);

			const bool result = lua_toboolean(im_ReadL, -1);
			lua_pop(im_ReadL, 1);

			return result;
		}
	}

	return false;
}

template <>
float LuaManager::ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, tableName);

		if(lua_istable(im_ReadL, -1)){
			lua_pushstring(im_ReadL, keyName);
			lua_gettable(im_ReadL, -2);

			const float result = (float)lua_tonumber(im_ReadL, -1);
			lua_pop(im_ReadL, 1);

			return result;
		}
	}

	return 0.0f;
}

template <>
int LuaManager::ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, tableName);

		if(lua_istable(im_ReadL, -1)){
			lua_pushstring(im_ReadL, keyName);
			lua_gettable(im_ReadL, -2);

			const int result = (int)lua_tointeger(im_ReadL, -1);
			lua_pop(im_ReadL, 1);

			return result;
		}
	}

	return 0;
}

template <class T>
std::vector<T> LuaManager::ReadFromTable(cstr const fPath, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg){
	assert(false);
	return std::vector<T>();
}

template <>
std::vector<bool> LuaManager::ReadFromTable(cstr const fPath, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, tableName);

		if(lua_istable(im_ReadL, -1)){
			std::vector<bool> results;
			const size_t keyNamesSize = keyNames.size();
			results.reserve(keyNamesSize);

			for(cstr const keyName: keyNames){
				lua_pushstring(im_ReadL, keyName);
				lua_gettable(im_ReadL, -2);

				results.emplace_back(lua_toboolean(im_ReadL, -1));
				lua_pop(im_ReadL, 1);
			}

			return results;
		}
	}

	return std::vector<bool>();
}