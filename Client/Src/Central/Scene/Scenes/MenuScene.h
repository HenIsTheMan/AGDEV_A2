#pragma once

#include "../IScene.hpp"

class MenuScene final: public IScene{
public:
	MenuScene();
	~MenuScene();

	void EarlyInit() override;
	void Init() override;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;

	void PreRender() override;
	void Render() override;
	void PostRender() override;
private:
};