#pragma once

#include "SDPs/Singleton.h"

#include "GameScene.h"

class SceneManager final: public Singleton<SceneManager>{
	friend Singleton<SceneManager>;
public:
	~SceneManager() = default;
private:
	GameScene* im_GameScene;

	SceneManager();
};