#include <Core.h>
#include <Engine.h>
#include "Admin/App.h"

#include "Lua/Units/LuaTest1st.hpp"
#include "Lua/Units/LuaTest2nd.hpp"
#include "Lua/Units/LuaTest3rd.hpp"
#include "Lua/Units/LuaTest4th.hpp"
#include "Lua/Units/LuaTest5th.hpp"

extern bool endLoop;

void MainProcess(){
	App* app = App::GetObjPtr();
	while(!endLoop){
		app->Update();
		app->PreRender();
		app->Render();
		app->PostRender();
	}
	app->Destroy();
}

int main(const int&, const char* const* const&){
	if(!InitConsole()){
		return -1;
	}

	std::thread worker(&MainProcess);

	LuaTest5th();

	while(!endLoop){
		if(Key(VK_ESCAPE)){
			endLoop = true;
			break;
		}
	}

	worker.join();
}