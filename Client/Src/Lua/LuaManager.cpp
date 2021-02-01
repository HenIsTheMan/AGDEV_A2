#include "LuaManager.h"

LuaManager::~LuaManager(){
}

void LuaManager::Init(){
	im_ReadL = luaL_newstate();
	luaL_openlibs(im_ReadL);

	im_WriteL = luaL_newstate();
	luaL_openlibs(im_WriteL);
}

void LuaManager::PrintFileContent(cstr const fPath){
	str myStr = str();
	std::ifstream stream;

	stream.open(fPath);
	while(!stream.eof()){
		std::getline(stream, myStr);
		std::cout << myStr + '\n';
	}
	stream.close();
}

LuaManager::LuaManager():
	im_ReadL(nullptr),
	im_WriteL(nullptr)
{
}