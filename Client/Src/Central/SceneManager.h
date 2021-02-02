#pragma once

#include "SDPs/Singleton.h"

#include "GameScene.h"

class SceneManager final: public Singleton<SceneManager>{
	friend Singleton<SceneManager>;
public:
	~SceneManager() = default;

	void EarlyInit();
	void Init();

	void FixedUpdate();
	void Update();
	void LateUpdate();

	void PreRender() const;
	void Render();
	void PostRender() const;
private:
	SceneManager();
};