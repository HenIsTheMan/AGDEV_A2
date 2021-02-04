#include "ShopScene.h"

#include "../../App.h"
#include "../SceneManager.h"

extern float leftRightMB;
extern float windowMouseX;
extern float windowMouseY;
extern int windowWidth;
extern int windowHeight;

ShopScene::ShopScene():
	SceneSupport(),
	backColor(glm::vec4(1.0f))
{
}

void ShopScene::Enter(){
	SceneSupport::Enter();

	glfwSetInputMode(App::RetrieveWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	cam.SetPos(glm::vec3(0.f, 0.f, 5.f));
	cam.SetTarget(glm::vec3(0.f));
	cam.SetUp(glm::vec3(0.f, 1.f, 0.f));
}

void ShopScene::Exit(){
	SceneSupport::Exit();
}

void ShopScene::EarlyInit(){
	SceneSupport::EarlyInit();
}

void ShopScene::Init(){
	SceneSupport::Init();
}

void ShopScene::FixedUpdate(){
	SceneSupport::FixedUpdate();
}

void ShopScene::Update(){
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
}

void ShopScene::LateUpdate(){
	SceneSupport::LateUpdate();
}

void ShopScene::PreRender(){
	SceneSupport::PreRender();

	forwardSP.Use();

	forwardSP.Set3fv("camPos", cam.GetPos());
	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ShopScene::Render(){
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
		"Shop",
		(float)windowWidth * 0.5f, 
		(float)windowHeight * 0.85f,
		2.0f,
		glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
		0,
		TextChief::TextAlignment::Center
	});
}

void ShopScene::PostRender(){
	SceneSupport::PostRender();

	glDepthFunc(GL_LESS);
}