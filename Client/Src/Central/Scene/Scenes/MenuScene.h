#pragma once

#include "../SceneSupport.h"

class MenuScene final: public SceneSupport{
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