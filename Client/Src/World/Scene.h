#pragma once

class App;

class Scene final{ //Static class
	Scene() = delete;
	~Scene() = delete;
	Scene(const Scene&) = delete;
	Scene(Scene&&) noexcept = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) noexcept = delete;

	friend App;
public:
	///Setters
	static void SetInCtor(void (*inCtor)());
	static void SetInDtor(void (*inDtor)());
	static void SetInit(void (*init)());
	static void SetFixedUpdate(void (*fixedUpdate)());
	static void SetUpdate(void (*update)());
	static void SetLateUpdate(void (*lateUpdate)());
	static void SetPreRender(void (*preRender)());
	static void SetRender(void (*render)());
	static void SetPostRender(void (*postRender)());
private:
	static void (*sm_InCtor)();
	static void (*sm_InDtor)();
	static void (*sm_Init)();
	static void (*sm_FixedUpdate)();
	static void (*sm_Update)();
	static void (*sm_LateUpdate)();
	static void (*sm_PreRender)();
	static void (*sm_Render)();
	static void (*sm_PostRender)();

	static void InCtor();
	static void InDtor();
	static void Init();
	static void FixedUpdate();
	static void Update();
	static void LateUpdate();
	static void PreRender();
	static void Render();
	static void PostRender();
};