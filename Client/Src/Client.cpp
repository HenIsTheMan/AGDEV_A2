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

	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	(void)SetLayeredWindowAttributes(GetConsoleWindow(), NULL, 230, LWA_ALPHA);

	(void)SetWindowPos(
		GetConsoleWindow(),
		0,
		0,
		0,
		GetSystemMetrics(SM_CXFULLSCREEN),
		GetSystemMetrics(SM_CYFULLSCREEN),
		0
	);

	auto ConsoleEventHandler = [](const DWORD event){
		LPCWSTR msg;
		switch(event){
			case CTRL_C_EVENT: msg = L"Ctrl + C"; break;
			case CTRL_BREAK_EVENT: msg = L"Ctrl + BREAK"; break;
			case CTRL_CLOSE_EVENT: msg = L"Closing prog..."; break;
			case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT: msg = L"User is logging off..."; break;
			default: msg = L"???";
		}
		MessageBox(NULL, msg, L"Nameless", MB_OK);
		return TRUE;
	};

	if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleEventHandler, TRUE)){
		(void)puts("Failed to install console event handler!\n");
		return false;
	}

	std::thread worker(&MainProcess);

	while(!endLoop){
		if(Key(VK_ESCAPE)){
			endLoop = true;
			break;
		}
	}

	worker.join();
}