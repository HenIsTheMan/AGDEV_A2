#pragma once

#include "../Cam.h"

#include "../../Entity/Entities/Shotgun.h"
#include "../../Entity/Entities/Scar.h"
#include "../../Entity/Entities/Sniper.h"

#include "../../Entity/EntityManager.h"

#include "../../Shared/Meshes.h"

#include "../ModelStack.h"

#include "../../LOD/LOD.h"

#include "../../Shared/RotateVecIn2D.hpp"

#include "IScene.hpp"

class SceneSupport: public IScene{ //Abstract class
public:
	~SceneSupport();

	void EarlyInit() override;
	void Init() override;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;

	void PreRender() override;
	void Render() override;
	void PostRender() override;
protected:
	Cam cam;
	ISoundEngine* soundEngine;
	TextChief textChief;

	ShaderProg forwardSP;
	ShaderProg viewingFrustumSP;
	ShaderProg textSP;

	glm::mat4 view;
	glm::mat4 projection;

	ModelStack modelStack;

	float elapsedTime;

	int polyModes[2];

	SceneSupport();
};