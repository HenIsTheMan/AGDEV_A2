#pragma once

#include "SDPs/Singleton.h"

#include "SceneID.hpp"

#include "GameScene.h"

class SceneManager final: public Singleton<SceneManager>{
	friend Singleton<SceneManager>;
public:
	~SceneManager();

	void AddScene(const SceneID ID, IScene* const scene);

	void EarlyInit();
	void Init();

	void FixedUpdate();
	void Update();
	void LateUpdate();

	void PreRender();
	void Render();
	void PostRender();

	void SetCurrScene(const SceneID ID);
	void SetNextScene(const SceneID ID);
private:
	IScene* im_CurrScene;
	IScene* im_NextScene;

	std::map<SceneID, IScene*> im_Scenes;

	SceneManager();
};