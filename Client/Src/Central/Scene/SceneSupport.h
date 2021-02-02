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

#include "../../Lua/LuaManager.h"

class SceneSupport: public IScene{ //Abstract class
public:
	~SceneSupport();

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
protected:
	static float elapsedTime;

	static Cam cam;
	static ISoundEngine* soundEngine;
	static TextChief textChief;

	ShaderProg forwardSP;
	ShaderProg viewingFrustumSP;
	ShaderProg textSP;

	static glm::mat4 view;
	static glm::mat4 projection;

	static ModelStack modelStack;

	static int polyModes[2];

	static float textScaleFactors[2];
	static glm::vec4 textColours[2];

	LuaManager* luaManager;
	LPWIN32_FIND_DATA dataAudio;
	ISound* BGM;

	SceneSupport();

	void TuneAudio(cstr const fPath);
};