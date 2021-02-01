#include <Core.h>
#include <Engine.h>
#include "Admin/App.h"

#include "Lua/Units/LuaTest4th.hpp"

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

	LuaTest4th();

	while(!endLoop){
		if(Key(VK_ESCAPE)){
			endLoop = true;
			break;
		}
	}

	worker.join();
}