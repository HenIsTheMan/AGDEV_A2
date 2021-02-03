template <class T>
T LuaManager::CallCppFunc(cstr const fPath, cstr const luaFuncName, cstr const hostFuncNameInLua, int (* const hostFunc)(lua_State* const L), const std::vector<T>& params, const bool printErrMsg){
	assert(false);
	return T();
}

template <>
float LuaManager::CallCppFunc(cstr const fPath, cstr const luaFuncName, cstr const hostFuncNameInLua, int (* const hostFunc)(lua_State* const L), const std::vector<float>& params, const bool printErrMsg){
	lua_register(im_ReadL, hostFuncNameInLua, hostFunc);

	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), true)){
		lua_getglobal(im_ReadL, luaFuncName);

		if(lua_isfunction(im_ReadL, -1)){
			for(const float param: params){
				lua_pushnumber(im_ReadL, param);
			}

			if(!LuaErrCheck(im_ReadL, lua_pcall(im_ReadL, (int)params.size(), 1, 0), true)){
				return (float)lua_tonumber(im_ReadL, -1);
			}
		}
	}

	return 0.0f;
}

template <class T>
void LuaManager::CallLuaFuncReturnVoid(cstr const fPath, cstr const funcName, const std::vector<T>& params, const bool printErrMsg){
	assert(false);
}

template <>
void LuaManager::CallLuaFuncReturnVoid(cstr const fPath, cstr const funcName, const std::vector<int>& params, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), true)){
		lua_getglobal(im_ReadL, funcName);

		if(lua_isfunction(im_ReadL, -1)){
			for(const int param: params){
				lua_pushinteger(im_ReadL, param);
			}

			if(LuaErrCheck(im_ReadL, lua_pcall(im_ReadL, (int)params.size(), 0, 0), true)){
				(void)puts("Ayo, err here yo");
			}
		}
	}
}

template <class T>
T LuaManager::CallLuaFunc(cstr const fPath, cstr const funcName, const std::vector<T>& params, const bool printErrMsg){
	assert(false);
	return T();
}

template <>
float LuaManager::CallLuaFunc(cstr const fPath, cstr const funcName, const std::vector<float>& params, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), true)){
		lua_getglobal(im_ReadL, funcName);

		if(lua_isfunction(im_ReadL, -1)){
			for(const float param: params){
				lua_pushnumber(im_ReadL, param);
			}

			if(!LuaErrCheck(im_ReadL, lua_pcall(im_ReadL, (int)params.size(), 1, 0), true)){
				return (float)lua_tonumber(im_ReadL, -1);
			}
		}
	}

	return 0.0f;
}

template <>
int LuaManager::CallLuaFunc(cstr const fPath, cstr const funcName, const std::vector<int>& params, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), true)){
		lua_getglobal(im_ReadL, funcName);

		if(lua_isfunction(im_ReadL, -1)){
			for(const int param: params){
				lua_pushinteger(im_ReadL, param);
			}

			if(!LuaErrCheck(im_ReadL, lua_pcall(im_ReadL, (int)params.size(), 1, 0), true)){
				return (int)lua_tointeger(im_ReadL, -1);
			}
		}
	}

	return 0;
}

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
std::vector<T> LuaManager::ReadFromJaggedArr(cstr const fPath, cstr const arrName, const int startIndex, const int endIndex, const int subStartIndex, const int subEndIndex, const bool printErrMsg){
	assert(false);
	return std::vector<T>();
}

template <>
std::vector<glm::vec3> LuaManager::ReadFromJaggedArr(cstr const fPath, cstr const arrName, const int startIndex, const int endIndex, const int subStartIndex, const int subEndIndex, const bool printErrMsg){
	std::vector<glm::vec3> vec;

	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, arrName);

		if(lua_istable(im_ReadL, -1)){
			for(int i = startIndex; i <= endIndex; ++i){
				lua_pushinteger(im_ReadL, i);
				lua_gettable(im_ReadL, -2);
				
				if(lua_istable(im_ReadL, -1)){
					glm::vec3 myVec3;

					for(int j = subStartIndex; j <= subEndIndex; ++j){
						lua_pushinteger(im_ReadL, j);
						lua_gettable(im_ReadL, -2);

						myVec3[j - 1] = (float)lua_tonumber(im_ReadL, -1);
						lua_pop(im_ReadL, 1);
					}

					vec.emplace_back(myVec3);
				}

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

std::vector<cstr> LuaManager::ReadFromTableCstr(cstr const fPath, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, tableName);

		if(lua_istable(im_ReadL, -1)){
			std::vector<cstr> results;
			const size_t keyNamesSize = keyNames.size();
			results.reserve(keyNamesSize);

			for(cstr const keyName: keyNames){
				lua_pushstring(im_ReadL, keyName);
				lua_gettable(im_ReadL, -2);

				results.emplace_back(lua_tostring(im_ReadL, -1));
				lua_pop(im_ReadL, 1);
			}

			return results;
		}
	}

	return std::vector<cstr>();
}

template <>
std::vector<float> LuaManager::ReadFromTable(cstr const fPath, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg){
	if(!LuaErrCheck(im_ReadL, luaL_dofile(im_ReadL, fPath), printErrMsg)){
		lua_getglobal(im_ReadL, tableName);

		if(lua_istable(im_ReadL, -1)){
			std::vector<float> results;
			const size_t keyNamesSize = keyNames.size();
			results.reserve(keyNamesSize);

			for(cstr const keyName: keyNames){
				lua_pushstring(im_ReadL, keyName);
				lua_gettable(im_ReadL, -2);

				results.emplace_back((float)lua_tonumber(im_ReadL, -1));
				lua_pop(im_ReadL, 1);
			}

			return results;
		}
	}

	return std::vector<float>();
}

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
						static_cast<StrType*>(data[i])->data = lua_tostring(im_ReadL, -1);
						break;
					case LUA_TBOOLEAN:
						static_cast<BoolType*>(data[i])->data = lua_toboolean(im_ReadL, -1);
						break;
					case LUA_TNUMBER:
						static_cast<DoubleType*>(data[i])->data = lua_tonumber(im_ReadL, -1);
						break;
					default: 
						(void)printf("%s\n", lua_typename(im_ReadL, -1));
						assert(false);
						break;
				}

				lua_pop(im_ReadL, 1);
			}
		}
	}
}

template <class T>
void LuaManager::WriteOverwrite(cstr const fPath, cstr const tableName, cstr const keyName, const T newVal, const bool printErrMsg){
	assert(false);
}

template <>
void LuaManager::WriteOverwrite(cstr const fPath, cstr const tableName, cstr const keyName, const float newVal, const bool printErrMsg){
	if(!LuaErrCheck(im_WriteL, luaL_dofile(im_WriteL, "Scripts/WriteToLuaFile.lua"), printErrMsg)){
		lua_getglobal(im_WriteL, "WriteOverwrite");

		if(lua_isfunction(im_WriteL, -1)){
			lua_pushstring(im_WriteL, fPath);

			std::stringstream ss;
			ss << newVal;
			const str newStr = keyName + (str)" = " + ss.str();
			lua_pushstring(im_WriteL, newStr.c_str());

			const str oldStr = keyName + (str)" = " + ReadFromTableCstr(fPath, tableName, {keyName}, true)[0];
			lua_pushstring(im_WriteL, oldStr.c_str());

			if(LuaErrCheck(im_WriteL, lua_pcall(im_WriteL, 3, 0, 0), true)){
				(void)puts("Ayo, err here yo");
			}
		}
	}
}