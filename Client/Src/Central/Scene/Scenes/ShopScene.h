#pragma once

#include "../SceneSupport.h"

class SceneManager;

class ShopScene final: public SceneSupport{
public:
	ShopScene();
	~ShopScene() = default;

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