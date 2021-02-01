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
extern int winWidth;
extern int winHeight;

const GLFWvidmode* App::mode = nullptr;
GLFWwindow* App::win = nullptr;

static void FramebufferSizeCallback(GLFWwindow*, int width, int height){ //Resize callback
	winWidth = width;
	winHeight = height;
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

static void MouseButtonCallback(GLFWwindow* win, int button, int action, int mods){ //For mouse buttons
	LMB = (bool)glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);
	RMB = (bool)glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT);
	leftRightMB = float(LMB - RMB);
}

static void ScrollCallback(GLFWwindow*, double xOffset, double yOffset){
	angularFOV -= float(xOffset) + float(yOffset);
	angularFOV = std::max(1.f, std::min(75.f, angularFOV));
}

App::App():
	fullscreen(false),
	elapsedTime(0.f),
	lastFrameTime(0.f),
	scene(),
	luaManager(LuaManager::GetObjPtr()),
	data(new WIN32_FIND_DATA())
{
	(void)Init1st();
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

bool App::Init1st() const{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For Mac OS X
	#endif

	const GLFWvidmode* const& mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SetWindowPos(
		GetConsoleWindow(),
		0,
		int((GetSystemMetrics(SM_CXFULLSCREEN) - float(mode->width - GetSystemMetrics(SM_CXFULLSCREEN)) / 2.f) * 5.f / 6.f),
		0,
		int((GetSystemMetrics(SM_CXFULLSCREEN) - float(mode->width - GetSystemMetrics(SM_CXFULLSCREEN)) / 2.f) / 6.f),
		GetSystemMetrics(SM_CYFULLSCREEN),
		0
	);
	optimalWinWidth = winWidth = int((GetSystemMetrics(SM_CXFULLSCREEN) - float(mode->width - GetSystemMetrics(SM_CXFULLSCREEN)) / 2.f) * 5.f / 6.f);
	optimalWinHeight = winHeight = GetSystemMetrics(SM_CYFULLSCREEN) - int(float(mode->height - GetSystemMetrics(SM_CYFULLSCREEN)) / 2.f);
	win = glfwCreateWindow(winWidth, winHeight, "Nameless Engine", nullptr, nullptr);
	optimalWinXPos = int(float(mode->width - GetSystemMetrics(SM_CXFULLSCREEN)) / 2.f);
	optimalWinYPos = int(float(mode->height - GetSystemMetrics(SM_CYFULLSCREEN)) / 2.f);
	glfwSetWindowPos(win, optimalWinXPos, optimalWinYPos);

	if(win == 0){ //Get a handle to the created window obj
		(void)puts("Failed to create GLFW win\n");
		return false;
	}
	glfwMakeContextCurrent(win); //Make context of the window the main context on the curr thread
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		(void)puts("Failed to init GLAD\n");
		return false;
	}
	glfwSetFramebufferSizeCallback(win, &FramebufferSizeCallback);
	glfwSetCursorPosCallback(win, CursorPosCallback);
	glfwSetMouseButtonCallback(win, MouseButtonCallback);
	//glfwSetScrollCallback(win, ScrollCallback);

	glfwSwapInterval(0);

	return true;
}

bool App::TuneAppWindow() const{
	return false;
}

bool App::TuneConsoleWindow() const{
	SetConsoleTitle(L"Nameless Console");
	system("Color 0A");

	HANDLE StdHandle = GetStdHandle(DWORD(-11));
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(StdHandle, &cursorInfo);
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(StdHandle, &cursorInfo);

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

	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleEventHandler, TRUE)){
		(void)puts("Failed to install console event handler!\n");
		return false;
	}
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

	static bool yes = true;
	if(yes){
		(void)TuneAppWindow();
		(void)TuneConsoleWindow();
		yes = false;
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