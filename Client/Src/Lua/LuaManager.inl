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
std::vector<T> LuaManager::ReadFromArr(cstr const fPath, cstr const arrName, const int startIndex, const int endIndex, const bool printErrMsg){
	assert(false);
	return std::vector<T>();
}

template <>
std::vector<cstr> LuaManager::ReadFromArr(cstr const fPath, cstr const arrName, const int startIndex, const int endIndex, const bool printErrMsg){
	std::vector<cstr> vec;

	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, arrName);

		if(lua_istable(im_ReadL, -1)){
			for(int i = startIndex; i <= endIndex; ++i){
				lua_pushinteger(im_ReadL, i);
				lua_gettable(im_ReadL, -2);

				vec.emplace_back(lua_tostring(im_ReadL, -1));
				lua_pop(im_ReadL, 1);
			}
		}
	}

	return vec;
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

template <class T>
void LuaManager::ReadCustomFromTable(cstr const fPath, const std::vector<ObjType*>& data, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, tableName);

		if(lua_istable(im_ReadL, -1)){
			const int keyNamesSize = (int)keyNames.size();

			for(int i = 0; i < keyNamesSize; ++i){
				lua_pushstring(im_ReadL, keyNames[i]);
				lua_gettable(im_ReadL, -2);

				switch(lua_type(im_ReadL, -1)){
					case LUA_TSTRING:
						static_cast<StrType*>(data[i])->data = lua_tostring(im_ReadL, -1));
						break;
					case LUA_TBOOLEAN:
						static_cast<BoolType*>(data[i])->data = lua_toboolean(im_ReadL, -1));
						break;
					//case LUA_TNUMBER:
					//	printf("%d: %g\n", i, lua_tonumber(pStackDumpThisState, i));
					//	break;
					//default: 
					//	printf("%d: %s\n", i, lua_typename(pStackDumpThisState, t));
					//	break;
				}

				lua_pop(im_ReadL, 1);
			}
		}
	}
}