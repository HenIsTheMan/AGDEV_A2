#pragma once

#include "../SceneSupport.h"

class SceneManager;

class StartScene final: public SceneSupport{
public:
	StartScene();
	~StartScene() = default;

	void Enter() override;
	void Exit() override;

	void EarlyInit() override;
	void Init() override;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;

	void PreRender() override;
	void Render() override;
	void PostRender() override;
};