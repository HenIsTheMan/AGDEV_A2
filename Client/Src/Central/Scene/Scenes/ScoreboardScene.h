#pragma once

#include "../SceneSupport.h"

class SceneManager;

class ScoreboardScene final: public SceneSupport{
public:
	ScoreboardScene();
	~ScoreboardScene() = default;

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
private:
	float backScaleFactor;
	glm::vec4 backColor;
};