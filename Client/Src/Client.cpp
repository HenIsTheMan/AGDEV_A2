#include <Core.h>
#include <Engine.h>
#include "Admin/App.h"

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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::thread worker(&MainProcess);

	while(!endLoop){
		if(Key(VK_ESCAPE)){
			endLoop = true;
			break;
		}
	}

	worker.join();
}