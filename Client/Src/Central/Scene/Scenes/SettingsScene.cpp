#include "SettingsScene.h"

#include "../../App.h"
#include "../SceneManager.h"

extern float leftRightMB;
extern float windowMouseX;
extern float windowMouseY;
extern int windowWidth;
extern int windowHeight;

SettingsScene::SettingsScene():
	SceneSupport(),
	backColor(glm::vec4(1.0f)),
	currSettingsType((SettingsType)0)
{
}

void SettingsScene::Enter(){
	SceneSupport::Enter();

	glfwSetInputMode(App::RetrieveWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	cam.SetPos(glm::vec3(0.f, 0.f, 5.f));
	cam.SetTarget(glm::vec3(0.f));
	cam.SetUp(glm::vec3(0.f, 1.f, 0.f));
}

void SettingsScene::Exit(){
	SceneSupport::Exit();
}

void SettingsScene::EarlyInit(){
	SceneSupport::EarlyInit();
}

void SettingsScene::Init(){
	SceneSupport::Init();
}

void SettingsScene::FixedUpdate(){
	SceneSupport::FixedUpdate();
}

void SettingsScene::Update(){
	SceneSupport::Update();

	view = cam.LookAt();
	projection = glm::ortho(-float(windowWidth) / 2.f, float(windowWidth) / 2.f, -float(windowHeight) / 2.f, float(windowHeight) / 2.f, .1f, 99999.0f);
	static float buttonBT = 0.0f;

	if(windowMouseX >= (float)windowWidth * 0.02f
		&& windowMouseX <= (float)windowWidth * 0.08f
		&& windowMouseY >= (float)windowHeight * 0.91f
		&& windowMouseY <= (float)windowHeight * 0.95f
	){
		if(backScaleFactor != 1.1f){
			ISound* const sound = soundEngine->play2D("Audio/Sounds/Pop.flac", false, true);
			sound->setVolume(luaManager->Read<float>("Scripts/Audio.lua", "popVol", true));
			sound->setIsPaused(false);

			backScaleFactor = 1.1f;
			backColor = glm::vec4(1.f, 1.f, 0.f, 1.f);
		}

		if(leftRightMB > 0.f && buttonBT <= elapsedTime){
			ISound* const sound = soundEngine->play2D("Audio/Sounds/Select.wav", false, true);
			sound->setVolume(luaManager->Read<float>("Scripts/Audio.lua", "selectVol", true));
			sound->setIsPaused(false);

			SceneManager::GetObjPtr()->SetNextScene(SceneID::Menu);

			buttonBT = elapsedTime + .3f;
			return;
		}
	} else{
		backScaleFactor = 1.f;
		backColor = glm::vec4(1.f);
	}

	static bool isUp = false;
	if(!isUp && Key(VK_UP)){
		currSettingsType = (int)currSettingsType == 0 ? SettingsType((int)SettingsType::Amt - 1) : SettingsType((int)currSettingsType - 1);

		isUp = true;
	} else if(isUp && !Key(VK_UP)){
		isUp = false;
	}

	static bool isDown = false;
	if(!isDown && Key(VK_DOWN)){
		currSettingsType = (int)currSettingsType == (int)SettingsType::Amt - 1 ? (SettingsType)0 : SettingsType((int)currSettingsType + 1);

		isDown = true;
	} else if(isDown && !Key(VK_DOWN)){
		isDown = false;
	}

	static cstr fPath = "Scripts/Audio.lua";

	static bool isLeft = false;
	if(!isLeft && Key(VK_LEFT)){
		switch(currSettingsType){
			case SettingsType::IsPausedBGM:
				luaManager->WriteOverwrite<bool>(fPath, keyNames[(int)currSettingsType], !luaManager->Read<bool>(fPath, keyNames[(int)currSettingsType], true), true);
				break;
			case SettingsType::RolloffFactor:
			case SettingsType::DopplerFactorDopplerEffect:
				luaManager->WriteOverwrite<float>(fPath, keyNames[(int)currSettingsType], glm::clamp(luaManager->Read<float>(fPath, keyNames[(int)currSettingsType], true) - 0.1f, 0.0f, 10.0f), true);
				break;
			case SettingsType::DistFactor:
				luaManager->WriteOverwrite<float>(fPath, keyNames[(int)currSettingsType], luaManager->Read<float>(fPath, keyNames[(int)currSettingsType], true) - 0.1f, true);
				break;
			default:
				luaManager->WriteOverwrite<float>(fPath, keyNames[(int)currSettingsType], glm::clamp(luaManager->Read<float>(fPath, keyNames[(int)currSettingsType], true) - 0.1f, 0.0f, 1.0f), true);
				break;
		}

		isLeft = true;
	} else if(isLeft && !Key(VK_LEFT)){
		isLeft = false;
	}

	static bool isRight = false;
	if(!isRight && Key(VK_RIGHT)){
		switch(currSettingsType){
			case SettingsType::IsPausedBGM:
				luaManager->WriteOverwrite<bool>(fPath, keyNames[(int)currSettingsType], !luaManager->Read<bool>(fPath, keyNames[(int)currSettingsType], true), true);
				break;
			case SettingsType::RolloffFactor:
			case SettingsType::DopplerFactorDopplerEffect:
				luaManager->WriteOverwrite<float>(fPath, keyNames[(int)currSettingsType], glm::clamp(luaManager->Read<float>(fPath, keyNames[(int)currSettingsType], true) + 0.1f, 0.0f, 10.0f), true);
				break;
			case SettingsType::DistFactor:
				luaManager->WriteOverwrite<float>(fPath, keyNames[(int)currSettingsType], luaManager->Read<float>(fPath, keyNames[(int)currSettingsType], true) + 0.1f, true);
				break;
			default:
				luaManager->WriteOverwrite<float>(fPath, keyNames[(int)currSettingsType], glm::clamp(luaManager->Read<float>(fPath, keyNames[(int)currSettingsType], true) + 0.1f, 0.0f, 1.0f), true);
				break;
		}

		isRight = true;
	} else if(isRight && !Key(VK_RIGHT)){
		isRight = false;
	}
}

void SettingsScene::LateUpdate(){
	SceneSupport::LateUpdate();
}

void SettingsScene::PreRender(){
	SceneSupport::PreRender();

	forwardSP.Use();

	forwardSP.Set3fv("camPos", cam.GetPos());
	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SettingsScene::Render(){
	SceneSupport::Render();

	modelStack.PushModel({
		modelStack.Scale(glm::vec3(float(windowWidth) / 2.f, float(windowHeight) / 2.f, 1.f)),
	});
		forwardSP.Set1i("noNormals", 1);
		Meshes::meshes[(int)MeshType::SpaceSpriteAni]->SetModel(modelStack.GetTopModel());
		Meshes::meshes[(int)MeshType::SpaceSpriteAni]->Render(forwardSP);
		forwardSP.Set1i("noNormals", 0);
	modelStack.PopModel();

	glDepthFunc(GL_GREATER);

	for(size_t i = 0; i < (int)SettingsType::Amt; ++i){
		textChief.RenderText(textSP, {
			txts[i],
			(float)windowWidth * 0.4f,
			(float)windowHeight * (0.7f / (float)SettingsType::Amt) * float((int)SettingsType::Amt - 1 - (float)i) + (float)windowHeight * 0.1f,
			1.5f - 0.05f * (float)SettingsType::Amt,
			i == (int)currSettingsType ? glm::vec4(1.0f) : glm::vec4(glm::vec3(0.67f), 1.0f),
			0,
			TextChief::TextAlignment::Center
		});
	}

	static cstr fPath = "Scripts/Audio.lua";
	const str valTxts[(int)SettingsType::Amt]{
		luaManager->Read<bool>(fPath, "isPausedBGM", true) ? "true" : "false",
		std::to_string(luaManager->Read<float>(fPath, "soundVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "rolloffFactor", true)),
		std::to_string(luaManager->Read<float>(fPath, "dopplerFactorDopplerEffect", true)),
		std::to_string(luaManager->Read<float>(fPath, "distFactorDopplerEffect", true)),
		std::to_string(luaManager->Read<float>(fPath, "coinVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "scopeVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "popVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "selectVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "BGMVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "reloadVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "shotgunVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "scarVol", true)),
		std::to_string(luaManager->Read<float>(fPath, "sniperVol", true)),
	};
	for(size_t i = 0; i < (int)SettingsType::Amt; ++i){
		textChief.RenderText(textSP, {
			valTxts[i],
			(float)windowWidth * 0.6f,
			(float)windowHeight * (0.7f / (float)SettingsType::Amt) * float((int)SettingsType::Amt - 1 - (float)i) + (float)windowHeight * 0.1f,
			1.5f - 0.05f * (float)SettingsType::Amt,
			i == (int)currSettingsType ? glm::vec4(1.0f) : glm::vec4(glm::vec3(0.67f), 1.0f),
			0,
			TextChief::TextAlignment::Center
		});
	}

	textChief.RenderText(textSP, {
		"Back",
		(float)windowWidth * 0.05f,
		(float)windowHeight * 0.05f,
		backScaleFactor,
		backColor,
		0,
		TextChief::TextAlignment::Center
	});

	textChief.RenderText(textSP, {
		"Settings",
		(float)windowWidth * 0.5f, 
		(float)windowHeight * 0.85f,
		2.0f,
		glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
		0,
		TextChief::TextAlignment::Center
	});
}

void SettingsScene::PostRender(){
	SceneSupport::PostRender();

	glDepthFunc(GL_LESS);
}