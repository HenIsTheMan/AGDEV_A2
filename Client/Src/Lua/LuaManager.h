#pragma once

#include "Engine.h"

#include "LuaHelpers.h"

#include "../Experimental/BoolType.hpp"
#include "../Experimental/DoubleType.hpp"
#include "../Experimental/StrType.hpp"

class LuaManager final: public Singleton<LuaManager>{
	friend Singleton<LuaManager>;
public:
	enum struct WriteType: int{
		Append,
		Overwrite,
		Wipe,
		Amt
	};

	~LuaManager();

	void Init();

	void PrintFileContent(cstr const fPath);

	template <class T>
	T CallCppFunc(cstr const fPath, cstr const luaFuncName, cstr const hostFuncNameInLua, int (* const hostFunc)(lua_State* const L), const std::vector<T>& params, const bool printErrMsg);

	template <>
	inline float CallCppFunc(cstr const fPath, cstr const luaFuncName, cstr const hostFuncNameInLua, int (* const hostFunc)(lua_State* const L), const std::vector<float>& params, const bool printErrMsg);

	template <class T>
	T CallLuaFunc(cstr const fPath, cstr const funcName, const std::vector<T>& params, const bool printErrMsg);

	template <>
	inline float CallLuaFunc(cstr const fPath, cstr const funcName, const std::vector<float>& params, const bool printErrMsg);

	template <class T>
	T Read(cstr const fPath, cstr const varName, const bool printErrMsg);

	template <>
	inline bool Read(cstr const fPath, cstr const varName, const bool printErrMsg);

	template <>
	inline float Read(cstr const fPath, cstr const varName, const bool printErrMsg);

	template <>
	inline int Read(cstr const fPath, cstr const varName, const bool printErrMsg);

	template <>
	inline cstr Read(cstr const fPath, cstr const varName, const bool printErrMsg);

	template <class T>
	std::vector<T> ReadFromArr(cstr const fPath, cstr const arrName, const int startIndex, const int endIndex, const bool printErrMsg);

	template <>
	inline std::vector<cstr> ReadFromArr(cstr const fPath, cstr const arrName, const int startIndex, const int endIndex, const bool printErrMsg);

	template <class T>
	std::vector<T> ReadFromJaggedArr(cstr const fPath, cstr const arrName, const int startIndex, const int endIndex, const int subStartIndex, const int subEndIndex, const bool printErrMsg);

	template <>
	inline std::vector<glm::vec3> ReadFromJaggedArr(cstr const fPath, cstr const arrName, const int startIndex, const int endIndex, const int subStartIndex, const int subEndIndex, const bool printErrMsg);

	template <class T>
	T ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg);

	template <>
	inline bool ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg);

	template <>
	inline float ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg);

	template <>
	inline int ReadFromTable(cstr const fPath, cstr const tableName, cstr const keyName, const bool printErrMsg);

	template <class T>
	std::vector<T> ReadFromTable(cstr const fPath, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg);

	template <>
	inline std::vector<bool> ReadFromTable(cstr const fPath, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg);

	template <>
	inline std::vector<float> ReadFromTable(cstr const fPath, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg);

	inline void ReadCustomFromTable(cstr const fPath, const std::vector<ObjType*>& data, cstr const tableName, const std::vector<cstr>& keyNames, const bool printErrMsg);

	template <class T>
	T Write(cstr const fPath, cstr const newLHS, const T newVal, const T oldKey, const WriteType type, const bool printErrMsg);
private:
	lua_State* im_ReadL;
	lua_State* im_WriteL;

	LuaManager();
};

#include "LuaManager.inl"