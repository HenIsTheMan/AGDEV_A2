#include "CreditsScene.h"

#include "../../App.h"
#include "../SceneManager.h"

extern int windowWidth;
extern int windowHeight;

CreditsScene::CreditsScene():
	SceneSupport()
{
}

void CreditsScene::Enter(){
	SceneSupport::Enter();

	glfwSetInputMode(App::RetrieveWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	cam.SetPos(glm::vec3(0.f, 0.f, 5.f));
	cam.SetTarget(glm::vec3(0.f));
	cam.SetUp(glm::vec3(0.f, 1.f, 0.f));
}

void CreditsScene::Exit(){
	SceneSupport::Exit();
}

void CreditsScene::EarlyInit(){
	SceneSupport::EarlyInit();
}

void CreditsScene::Init(){
	SceneSupport::Init();
}

void CreditsScene::FixedUpdate(){
	SceneSupport::FixedUpdate();
}

void CreditsScene::Update(){
	SceneSupport::Update();

	view = cam.LookAt();
	projection = glm::ortho(-float(windowWidth) / 2.f, float(windowWidth) / 2.f, -float(windowHeight) / 2.f, float(windowHeight) / 2.f, .1f, 99999.0f);
}

void CreditsScene::LateUpdate(){
	SceneSupport::LateUpdate();
}

void CreditsScene::PreRender(){
	SceneSupport::PreRender();

	forwardSP.Use();

	forwardSP.Set3fv("camPos", cam.GetPos());
	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CreditsScene::Render(){
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
}

void CreditsScene::PostRender(){
	SceneSupport::PostRender();

	glDepthFunc(GL_LESS);
}