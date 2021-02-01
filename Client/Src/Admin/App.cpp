#include "App.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Vendor/stb_image.h"

#include <atlstr.h>

extern float dt;
extern bool endLoop;
extern int optimalWinXPos;
extern int optimalWinYPos;
extern int optimalWinWidth;
extern int optimalWinHeight;
extern int winWidth;
extern int winHeight;

const GLFWvidmode* App::mode = nullptr;
GLFWwindow* App::win = nullptr;

App::App():
	fullscreen(false),
	elapsedTime(0.f),
	lastFrameTime(0.f),
	scene(),
	luaManager(LuaManager::GetObjPtr()),
	data(new WIN32_FIND_DATA())
{
	if(!InitAPI(win)){
		(void)puts("Failed to init API\n");
		endLoop = true;
	}
	(void)Init();
}

App::~App(){
	if(luaManager != nullptr){
		luaManager->Destroy();
		luaManager = nullptr;
	}

	if(data != nullptr){
		delete data;
		data = nullptr;
	}

	glfwTerminate(); //Clean/Del all GLFW's resources that were allocated
}

bool App::Init(){
	luaManager->Init();
	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	(void)scene.Init();

	return true;
}

bool App::TuneOptions(cstr const fPath) const{
	static FILETIME lastWriteTime = {};
	const HANDLE handle = FindFirstFile((CStringW)(CString)fPath, data);

	if(data->ftLastWriteTime.dwLowDateTime != lastWriteTime.dwLowDateTime){
		std::cout << "OpenGL options tuned.\n\n";

		glPointSize(luaManager->Read<float>(fPath, "ptSize", true));
		glLineWidth(luaManager->Read<float>(fPath, "lineWidth", true));

		const std::vector<bool> results = luaManager->ReadFromTable<bool>(fPath, "enableOptionsOpenGL", {
			"enableStencilTest",
			"enableDepthTest",
			"enableBlend",
			"enableCullFace",
			"enableProgPtSize",
			"enableMultisample",
			"enableFramebufferSRGB",
			"enableTexCubemapSeamless",
		}, true);

		const int resultsSize = (int)results.size();
		for(int i = 0; i < resultsSize; ++i){
			if(results[i]){
				switch(i){
					case 0:
						glEnable(GL_STENCIL_TEST);
						break;
					case 1:
						glEnable(GL_DEPTH_TEST);
						break;
					case 2:
						glEnable(GL_BLEND);
						break;
					case 3:
						glEnable(GL_CULL_FACE);
						break;
					case 4:
						glEnable(GL_PROGRAM_POINT_SIZE);
						break;
					case 5:
						glEnable(GL_MULTISAMPLE);
						break;
					case 6:
						glEnable(GL_FRAMEBUFFER_SRGB);
						break;
					case 7:
						glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
						break;
				}
			}
		}

		lastWriteTime.dwLowDateTime = data->ftLastWriteTime.dwLowDateTime;
	}

	return true;
}

void App::Update(){
	if(glfwWindowShouldClose(App::win)){
		endLoop = true;
		return;
	}

	(void)TuneOptions("Scripts/OptionsOpenGL.lua");

	float currFrameTime = (float)glfwGetTime();
	dt = currFrameTime - lastFrameTime;
	lastFrameTime = currFrameTime;

	elapsedTime += dt;
	static float toggleFullscreenBT = 0.f;
	if(Key(VK_F1) && toggleFullscreenBT <= elapsedTime){
		if(fullscreen){
			glfwSetWindowMonitor(win, 0, optimalWinXPos, optimalWinYPos, optimalWinWidth, optimalWinHeight, GLFW_DONT_CARE);
			fullscreen = false;
		} else{
			glfwSetWindowMonitor(win, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
			fullscreen = true;
		}
		toggleFullscreenBT = elapsedTime + .5f;
	}

	scene.Update(win);
}

void App::PreRender() const{
}

void App::Render(){
	glViewport(0, 0, 2048, 2048);

	glViewport(0, 0, winWidth, winHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.f, 0.82f, 0.86f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.ForwardRender();
}

void App::PostRender() const{
	glfwSwapBuffers(win); //Swap the large 2D colour buffer containing colour values for each pixel in GLFW's window
	glfwPollEvents(); //Check for triggered events and call corresponding functions registered via callback methods
}