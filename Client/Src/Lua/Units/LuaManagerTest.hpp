#pragma once

#include "../LuaManager.h"

namespace{
	void LuaManagerTest(){
		LuaManager* luaManager = LuaManager::GetObjPtr();
		luaManager->Init();

		luaManager->PrintFileContent("Scripts/Test4th.lua");

		if(luaManager != nullptr){
			luaManager->Destroy();
			luaManager = nullptr;
		}
	}
}