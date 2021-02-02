#include "StartScene.h"

#include "../../App.h"
#include "../SceneManager.h"

#include "../../../Shared/Easing.hpp"

#include <glm/gtx/color_space.hpp>

extern bool endLoop;
extern float dt;
extern float windowMouseX;
extern float windowMouseY;
extern int windowWidth;
extern int windowHeight;
extern float leftRightMB;

StartScene::StartScene():
	SceneSupport(),
	customHue(0.0f),
	textScale(1.0f),
	textStartScale(1.5f),
	textEndScale(0.9f),
	textOffsetX(0.0f),
	textOffsetY(0.0f)
{
}

void StartScene::Enter(){
	SceneSupport::Enter();

	glfwSetInputMode(App::RetrieveWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	cam.SetPos(glm::vec3(0.f, 0.f, 5.f));
	cam.SetTarget(glm::vec3(0.f));
	cam.SetUp(glm::vec3(0.f, 1.f, 0.f));
}

void StartScene::Exit(){
	SceneSupport::Exit();
}

void StartScene::EarlyInit(){
	SceneSupport::EarlyInit();
}

void StartScene::Init(){
	SceneSupport::Init();
}

void StartScene::FixedUpdate(){
	SceneSupport::FixedUpdate();
}

void StartScene::Update(){
	SceneSupport::Update();

	view = cam.LookAt();
	projection = glm::ortho(-float(windowWidth) / 2.f, float(windowWidth) / 2.f, -float(windowHeight) / 2.f, float(windowHeight) / 2.f, .1f, 99999.0f);

	if(leftRightMB > 0.f){
		soundEngine->play2D("Audio/Sounds/Select.wav", false);

		SceneManager::GetObjPtr()->SetNextScene(SceneID::Menu);
		return;
	}

	customHue += (float)dt * 40.0f;
	if(customHue >= 360.0f){
		customHue = 0.0f;
	}

	textOffsetX = sinf(elapsedTime * 4.0f) * 4.0f;
	textOffsetY = cosf(elapsedTime * 4.0f) * 4.0f;

	const float lerpFactor = EaseInQuint(cosf(elapsedTime * 4.0f) * 0.5f + 0.5f);
	textScale = (1.0f - lerpFactor) * textStartScale + lerpFactor * textEndScale;
}

void StartScene::LateUpdate(){
	SceneSupport::LateUpdate();
}

void StartScene::PreRender(){
	SceneSupport::PreRender();

	forwardSP.Use();

	forwardSP.Set3fv("camPos", cam.GetPos());
	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void StartScene::Render(){
	SceneSupport::Render();

	modelStack.PushModel({
		modelStack.Scale(glm::vec3(float(windowWidth) / 2.f, float(windowHeight) / 2.f, 1.f)),
	});
		forwardSP.Set1i("noNormals", 1);
		Meshes::meshes[(int)MeshType::Quad]->SetModel(modelStack.GetTopModel());
		Meshes::meshes[(int)MeshType::Quad]->Render(forwardSP);
		forwardSP.Set1i("noNormals", 0);
	modelStack.PopModel();

	glDepthFunc(GL_GREATER);

	textChief.RenderText(textSP, {
		"ANOTHER",
		(float)windowWidth * 0.5f + textOffsetX, 
		(float)windowHeight * 0.8f + textOffsetY,
		4.0f,
		glm::vec4(glm::rgbColor(glm::vec3(360.0f - customHue, 1.0f, 1.0f)), 1.f),
		0,
		TextChief::TextAlignment::Center
	});
	textChief.RenderText(textSP, {
		"WORLD",
		(float)windowWidth * 0.5f + textOffsetX,
		(float)windowHeight * 0.6f + textOffsetY,
		4.0f,
		glm::vec4(glm::rgbColor(glm::vec3(customHue, 1.0f, 1.0f)), 1.f),
		0,
		TextChief::TextAlignment::Center
	});

	textChief.RenderText(textSP, {
		"Click anywhere to continue...",
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.1f,
		textScale,
		glm::vec4(1.0f),
		0,
		TextChief::TextAlignment::Center
	});
}

void StartScene::PostRender(){
	SceneSupport::PostRender();

	glDepthFunc(GL_LESS);
}