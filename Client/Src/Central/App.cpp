#include "App.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Vendor/stb_image.h"

#include <atlstr.h>

extern float angularFOV;
extern bool LMB;
extern bool RMB;
extern float leftRightMB;
extern float dt;
extern bool endLoop;
extern bool firstCall;
extern float pitch;
extern float yaw;
extern float lastX;
extern float lastY;
extern float SENS;
extern int optimalWinXPos;
extern int optimalWinYPos;
extern int optimalWinWidth;
extern int optimalWinHeight;
extern int windowWidth;
extern int windowHeight;

const GLFWvidmode* App::mode = nullptr;
GLFWwindow* App::window = nullptr;

static void FramebufferSizeCallback(GLFWwindow*, int width, int height){ //Resize callback
	windowWidth = width;
	windowHeight = height;
}

static void CursorPosCallback(GLFWwindow*, double xPos, double yPos){
	if(firstCall){
		firstCall = 0;
	} else{ //Add mouse movement offset between last frame and curr frame
		yaw -= (float(xPos) - lastX) * SENS;
		pitch -= (float(yPos) - lastY) * SENS;
	}
	lastX = float(xPos);
	lastY = float(yPos);
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){ //For mouse buttons
	LMB = (bool)glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	RMB = (bool)glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	leftRightMB = float(LMB - RMB);
}

static void ScrollCallback(GLFWwindow*, double xOffset, double yOffset){
	angularFOV -= float(xOffset) + float(yOffset);
	angularFOV = std::max(1.f, std::min(75.f, angularFOV));
}

App::~App(){
	if(luaManager != nullptr){
		luaManager->Destroy();
		luaManager = nullptr;
	}

	if(dataAppWindow != nullptr){
		delete dataAppWindow;
		dataAppWindow = nullptr;
	}

	if(dataConsoleWindow != nullptr){
		delete dataConsoleWindow;
		dataConsoleWindow = nullptr;
	}

	if(dataOptions != nullptr){
		delete dataOptions;
		dataOptions = nullptr;
	}

	if(sceneManager != nullptr){
		sceneManager->Destroy();
		sceneManager = nullptr;
	}

	glfwTerminate(); //Clean/Del all GLFW's resources that were allocated
}

void App::EarlyInit(){
	luaManager->Init();
	(void)TuneConsoleWindow("Scripts/ConsoleWindow.lua");

	glfwInit();
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For Mac OS X
	#endif
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	window = glfwCreateWindow(mode->width / 2, mode->height / 2, "", nullptr, nullptr);
	glfwSetWindowPos(window, mode->width / 4, mode->height / 4);
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glfwMaximizeWindow(window);

	if(window == nullptr){
		return (void)puts("Failed to create GLFW window\n");
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		return (void)puts("Failed to init GLAD\n");
	}

	sceneManager->EarlyInit();
}

void App::Init(){
	GetConsoleCursorInfo(StdHandle, &cursorInfo);
	sceneManager->Init();
}

void App::FixedUpdate(){
	sceneManager->FixedUpdate();
}

void App::Update(){
	if(glfwWindowShouldClose(App::window)){
		endLoop = true;
		return;
	}

	(void)TuneConsoleWindow("Scripts/ConsoleWindow.lua");
	(void)TuneAppWindow("Scripts/AppWindow.lua");
	(void)TuneOptions("Scripts/OptionsOpenGL.lua");

	float currFrameTime = (float)glfwGetTime();
	dt = currFrameTime - lastFrameTime;
	lastFrameTime = currFrameTime;

	elapsedTime += dt;
	static float toggleFullscreenBT = 0.f;
	if(Key(VK_F1) && toggleFullscreenBT <= elapsedTime){
		if(fullscreen){
			glfwSetWindowMonitor(window, 0, optimalWinXPos, optimalWinYPos, optimalWinWidth, optimalWinHeight, GLFW_DONT_CARE);
			fullscreen = false;
		} else{
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
			fullscreen = true;
		}
		toggleFullscreenBT = elapsedTime + .5f;
	}

	sceneManager->Update();
}

void App::LateUpdate(){
	sceneManager->LateUpdate();
}

void App::PreRender(){
	glViewport(0, 0, 2048, 2048);

	glViewport(0, 0, windowWidth, windowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.f, 0.82f, 0.86f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneManager->PreRender();
}

void App::Render(){
	sceneManager->Render();
}

void App::PostRender(){
	sceneManager->PostRender();

	glfwSwapBuffers(window); //Swap the large 2D colour buffer containing colour values for each pixel in GLFW's window
	glfwPollEvents(); //Check for triggered events and call corresponding functions registered via callback methods
}

App::App():
	fullscreen(false),
	elapsedTime(0.f),
	lastFrameTime(0.f),
	luaManager(LuaManager::GetObjPtr()),
	dataAppWindow(new WIN32_FIND_DATA()),
	dataConsoleWindow(new WIN32_FIND_DATA()),
	dataOptions(new WIN32_FIND_DATA()),
	StdHandle(GetStdHandle(DWORD(-11))),
	cursorInfo({}),
	sceneManager(SceneManager::GetObjPtr())
{
}

bool App::TuneAppWindow(cstr const fPath) const{
	static FILETIME lastWriteTime = {};
	const HANDLE handle = FindFirstFile((CStringW)(CString)fPath, dataAppWindow);

	if(dataAppWindow->ftLastWriteTime.dwLowDateTime != lastWriteTime.dwLowDateTime){
		std::cout << "App Window tuned.\n\n";

		const std::vector<bool> results = luaManager->ReadFromTable<bool>(fPath, "callbackFlags", {
			"shldSetFramebufferSizeCallback",
			"shldSetCursorPosCallback",
			"shldSetMouseButtonCallback",
			"shldSetScrollCallback",
		}, true);

		const int resultsSize = (int)results.size();
		for(int i = 0; i < resultsSize; ++i){
			if(results[i]){
				switch(i){
					case 0:
						glfwSetFramebufferSizeCallback(window, &FramebufferSizeCallback);
						break;
					case 1:
						glfwSetCursorPosCallback(window, CursorPosCallback);
						break;
					case 2:
						glfwSetMouseButtonCallback(window, MouseButtonCallback);
						break;
					case 3:
						glfwSetScrollCallback(window, ScrollCallback);
						break;
				}
			} else{
				switch(i){
					case 0:
						glfwSetFramebufferSizeCallback(window, nullptr);
						break;
					case 1:
						glfwSetCursorPosCallback(window, nullptr);
						break;
					case 2:
						glfwSetMouseButtonCallback(window, nullptr);
						break;
					case 3:
						glfwSetScrollCallback(window, nullptr);
						break;
				}
			}
		}

		glfwSwapInterval(luaManager->Read<int>(fPath, "swapInterval", true));

		if(luaManager->Read<bool>(fPath, "showWindow", true)){
			glfwShowWindow(window);
		} else{
			glfwHideWindow(window);
		}

		glfwSetWindowTitle(window, luaManager->Read<cstr>(fPath, "windowTitle", true));

		lastWriteTime.dwLowDateTime = dataAppWindow->ftLastWriteTime.dwLowDateTime;
	}

	return false;
}

bool App::TuneConsoleWindow(cstr const fPath){
	static FILETIME lastWriteTime = {};
	const HANDLE handle = FindFirstFile((CStringW)(CString)fPath, dataConsoleWindow);

	if(dataConsoleWindow->ftLastWriteTime.dwLowDateTime != lastWriteTime.dwLowDateTime){
		std::cout << "Console Window tuned.\n\n";

		SetConsoleTitle((CStringW)(CString)luaManager->Read<cstr>(fPath, "consoleTitle", true));
		system(luaManager->Read<cstr>(fPath, "color", true));

		cursorInfo.bVisible = luaManager->Read<bool>(fPath, "isCursorVisible", true);
		SetConsoleCursorInfo(StdHandle, &cursorInfo);

		::ShowWindow(::GetConsoleWindow(), luaManager->Read<int>(fPath, "showWindowInt", true));

		lastWriteTime.dwLowDateTime = dataConsoleWindow->ftLastWriteTime.dwLowDateTime;
	}

	return true;
}

bool App::TuneOptions(cstr const fPath) const{
	static FILETIME lastWriteTime = {};
	const HANDLE handle = FindFirstFile((CStringW)(CString)fPath, dataOptions);

	if(dataOptions->ftLastWriteTime.dwLowDateTime != lastWriteTime.dwLowDateTime){
		std::cout << "OpenGL Options tuned.\n\n";

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

		lastWriteTime.dwLowDateTime = dataOptions->ftLastWriteTime.dwLowDateTime;
	}

	return true;
}