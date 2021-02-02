#pragma once

#include "SDPs/Singleton.h"

#include "GameScene.h"

class SceneManager final: public Singleton<SceneManager>{
	friend Singleton<SceneManager>;
public:
	~SceneManager() = default;

	void Init();
private:
	GameScene* im_GameScene;

	void (*im_GameInCtor)();
	void (*im_GameInDtor)();
	void (*im_GameInit)();
	void (*im_GameFixedUpdate)();
	void (*im_GameUpdate)();
	void (*im_GameLateUpdate)();
	void (*im_GamePreRender)();
	void (*im_GameRender)();
	void (*im_GamePostRender)();

	SceneManager();
};