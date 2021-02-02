#pragma once

#include "SDPs/Singleton.h"

#include "SceneID.hpp"

#include "GameScene.h"

class SceneManager final: public Singleton<SceneManager>{
	friend Singleton<SceneManager>;
public:
	~SceneManager();

	void AddScene(const SceneID ID, Scene* const scene);

	void EarlyInit();
	void Init();

	void FixedUpdate();
	void Update();
	void LateUpdate();

	void PreRender() const;
	void Render();
	void PostRender() const;

	void SetCurrScene(const SceneID ID);
	void SetNextScene(const SceneID ID);
private:
	Scene* im_CurrScene;
	Scene* im_NextScene;

	std::map<SceneID, Scene*> im_Scenes;

	SceneManager();
};