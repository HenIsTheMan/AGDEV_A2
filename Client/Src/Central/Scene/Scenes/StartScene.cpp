#include "StartScene.h"

#include "../../App.h"
#include "../SceneManager.h"

extern bool endLoop;
extern float windowMouseX;
extern float windowMouseY;
extern int windowWidth;
extern int windowHeight;
extern float leftRightMB;

StartScene::StartScene():
	SceneSupport()
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
	static float buttonBT = 0.f;

	if(leftRightMB > 0.f && buttonBT <= elapsedTime){
		soundEngine->play2D("Audio/Sounds/Select.wav", false);

		SceneManager::GetObjPtr()->SetNextScene(SceneID::Menu);

		buttonBT = elapsedTime + .3f;
	}
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
		(float)windowWidth * 0.5f, 
		(float)windowHeight * 0.8f,
		4.0f,
		glm::vec4(glm::vec3(1.f, 0.f, 1.f), 1.f),
		0,
		TextChief::TextAlignment::Center
	});
	textChief.RenderText(textSP, {
		"WORLD",
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.6f,
		4.0f,
		glm::vec4(glm::vec3(1.f, 0.f, 1.f), 1.f),
		0,
		TextChief::TextAlignment::Center
	});
}

void StartScene::PostRender(){
	SceneSupport::PostRender();

	glDepthFunc(GL_LESS);
}