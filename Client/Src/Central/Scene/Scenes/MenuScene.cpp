#include "MenuScene.h"

#include "../../App.h"
#include "../SceneManager.h"

#include <glm/gtx/color_space.hpp>

extern bool endLoop;
extern float windowMouseX;
extern float windowMouseY;
extern int windowWidth;
extern int windowHeight;
extern float leftRightMB;

MenuScene::MenuScene():
	SceneSupport(),
	hue(0.0f),
	textOffsetX(0.0f),
	textOffsetY(0.0f)
{
}

MenuScene::~MenuScene(){
}

void MenuScene::Enter(){
	SceneSupport::Enter();

	glfwSetInputMode(App::RetrieveWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	cam.SetPos(glm::vec3(0.f, 0.f, 5.f));
	cam.SetTarget(glm::vec3(0.f));
	cam.SetUp(glm::vec3(0.f, 1.f, 0.f));
}

void MenuScene::Exit(){
	SceneSupport::Exit();
}

void MenuScene::EarlyInit(){
	SceneSupport::EarlyInit();
}

void MenuScene::Init(){
	SceneSupport::Init();

	Meshes::meshes[(int)MeshType::SpaceSpriteAni]->AddTexMap({"Imgs/SpaceSpriteSheet.png", Mesh::TexType::Diffuse, 0});
	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::SpaceSpriteAni])->AddAni("SpaceSpriteAni", 0, 152);
	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::SpaceSpriteAni])->Play("SpaceSpriteAni", -1, 2.0f);
}

void MenuScene::FixedUpdate(){
	SceneSupport::FixedUpdate();
}

void MenuScene::Update(){
	SceneSupport::Update();

	view = cam.LookAt();
	projection = glm::ortho(-float(windowWidth) / 2.f, float(windowWidth) / 2.f, -float(windowHeight) / 2.f, float(windowHeight) / 2.f, .1f, 99999.0f);
	static float buttonBT = 0.0f;

	const bool results[] = {
		windowMouseX >= (float)windowWidth * 0.47f
		&& windowMouseX <= (float)windowWidth * 0.53f
		&& windowMouseY >= (float)windowHeight * 0.56f
		&& windowMouseY <= (float)windowHeight * 0.6f,
		windowMouseX >= (float)windowWidth * 0.47f
		&& windowMouseX <= (float)windowWidth * 0.53f
		&& windowMouseY >= (float)windowHeight * 0.63f
		&& windowMouseY <= (float)windowHeight * 0.67f,
		windowMouseX >= (float)windowWidth * 0.47f
		&& windowMouseX <= (float)windowWidth * 0.53f
		&& windowMouseY >= (float)windowHeight * 0.7f
		&& windowMouseY <= (float)windowHeight * 0.74f,
		windowMouseX >= (float)windowWidth * 0.47f
		&& windowMouseX <= (float)windowWidth * 0.53f
		&& windowMouseY >= (float)windowHeight * 0.77f
		&& windowMouseY <= (float)windowHeight * 0.81f,
		windowMouseX >= (float)windowWidth * 0.47f
		&& windowMouseX <= (float)windowWidth * 0.53f
		&& windowMouseY >= (float)windowHeight * 0.84f
		&& windowMouseY <= (float)windowHeight * 0.88f,
		windowMouseX >= (float)windowWidth * 0.47f
		&& windowMouseX <= (float)windowWidth * 0.53f
		&& windowMouseY >= (float)windowHeight * 0.91f
		&& windowMouseY <= (float)windowHeight * 0.95f,
	};

	for(int i = 0; i < (int)MenuButtons::Amt; ++i){
		if(results[i]){
			if(textScaleFactors[i] != 1.1f){
				ISound* const sound = soundEngine->play2D("Audio/Sounds/Pop.flac", false, true);
				sound->setVolume(luaManager->Read<float>("Scripts/Audio.lua", "popVol", true));
				sound->setIsPaused(false);

				textScaleFactors[i] = 1.1f;
				textColours[i] = glm::vec4(1.f, 1.f, 0.f, 1.f);
			}

			if(leftRightMB > 0.f && buttonBT <= elapsedTime){
				ISound* const sound = soundEngine->play2D("Audio/Sounds/Select.wav", false, true);
				sound->setVolume(luaManager->Read<float>("Scripts/Audio.lua", "selectVol", true));
				sound->setIsPaused(false);

				switch((MenuButtons)i){
					case MenuButtons::Play:
						SceneManager::GetObjPtr()->SetNextScene(SceneID::Game);
						break;
					case MenuButtons::Shop:
					case MenuButtons::Scores:
					case MenuButtons::Settings:
					case MenuButtons::Credits:
						SceneManager::GetObjPtr()->SetNextScene(SceneID::Credits);
						break;
					case MenuButtons::Quit:
						endLoop = true;
						break;
				}

				buttonBT = elapsedTime + .3f;
				return;
			}
		} else{
			textScaleFactors[i] = 1.f;
			textColours[i] = glm::vec4(1.f);
		}
	}

	textOffsetX = sinf(elapsedTime * 4.0f) * 4.0f;
	textOffsetY = -cosf(elapsedTime * 4.0f) * 4.0f;

	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::SpaceSpriteAni])->Update();

	static float hueChangeBT = 0.0f;
	if(hueChangeBT <= elapsedTime){
		hue = PseudorandMinMax(0.0f, 360.0f);

		hueChangeBT = elapsedTime + 0.4f;
	}
}

void MenuScene::LateUpdate(){
	SceneSupport::LateUpdate();
}

void MenuScene::PreRender(){
	SceneSupport::PreRender();

	forwardSP.Use();

	forwardSP.Set3fv("camPos", cam.GetPos());
	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MenuScene::Render(){
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

	for(size_t i = 0; i < (int)MenuButtons::Amt; ++i){
		textChief.RenderText(textSP, {
			menuButtonTexts[i],
			(float)windowWidth * 0.5f,
			(float)windowHeight * 0.07f * float((int)MenuButtons::Amt - 1 - (float)i) + (float)windowHeight * 0.05f,
			textScaleFactors[i],
			textColours[i],
			0,
			TextChief::TextAlignment::Center
		});
	}

	textChief.RenderText(textSP, {
		"ANOTHER WORLD",
		(float)windowWidth * 0.5f + textOffsetX, 
		(float)windowHeight * 0.7f + textOffsetY,
		4.0f,
		glm::vec4(glm::rgbColor(glm::vec3(hue, 1.0f, 1.0f)), 1.f),
		0,
		TextChief::TextAlignment::Center
	});
}

void MenuScene::PostRender(){
	SceneSupport::PostRender();

	glDepthFunc(GL_LESS);
}