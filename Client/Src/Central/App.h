#pragma once

#include "../Lua/LuaManager.h"

#include "SceneManager.h"

class App final: public Singleton<App>{
	explicit App(const App&) = delete;
	explicit App(App&&) noexcept = delete;
	App& operator=(const App&) = delete;
	App& operator=(App&&) noexcept = delete;

	friend Singleton<App>;
public:
	~App();

	void EarlyInit();
	void Init();

	void FixedUpdate();
	void Update();
	void LateUpdate();

	void PreRender() const;
	void Render();
	void PostRender() const;
private:
	App();

	bool TuneAppWindow(cstr const fPath) const;
	bool TuneConsoleWindow(cstr const fPath);
	bool TuneOptions(cstr const fPath) const;

	bool fullscreen;
	float elapsedTime;
	float lastFrameTime;

	static const GLFWvidmode* mode;
	static GLFWwindow* win;

	LuaManager* luaManager;
	LPWIN32_FIND_DATA dataAppWindow;
	LPWIN32_FIND_DATA dataConsoleWindow;
	LPWIN32_FIND_DATA dataOptions;

	HANDLE StdHandle;
	CONSOLE_CURSOR_INFO cursorInfo;

	SceneManager* sceneManager;
};

template <class T>
inline T& operator++(T& myEnum){
	myEnum = T((int)myEnum + 1);
	return myEnum;
}