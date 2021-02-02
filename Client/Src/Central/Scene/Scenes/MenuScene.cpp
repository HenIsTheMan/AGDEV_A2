#include "MenuScene.h"

extern bool endLoop;
extern int windowWidth;
extern int windowHeight;
extern float leftRightMB;

MenuScene::MenuScene(){
}

MenuScene::~MenuScene(){
}

void MenuScene::EarlyInit(){
	SceneSupport::EarlyInit();
}

void MenuScene::Init(){
}

void MenuScene::FixedUpdate(){
}

void MenuScene::Update(){
	POINT mousePos;
	if(GetCursorPos(&mousePos)){
		HWND hwnd = ::GetActiveWindow();
		(void)ScreenToClient(hwnd, &mousePos);
	} else{
		(void)puts("Failed to get mouse pos relative to screen!");
	}
	static float buttonBT = 0.f;

	cam.SetPos(glm::vec3(0.f, 0.f, 5.f));
	cam.SetTarget(glm::vec3(0.f));
	cam.SetUp(glm::vec3(0.f, 1.f, 0.f));
	view = cam.LookAt();
	projection = glm::ortho(-float(windowWidth) / 2.f, float(windowWidth) / 2.f, -float(windowHeight) / 2.f, float(windowHeight) / 2.f, .1f, 99999.0f);

	if(mousePos.x >= (float)windowWidth * 0.47f
		&& mousePos.x <= (float)windowWidth * 0.53f
		&& mousePos.y >= (float)windowHeight * 0.77f
		&& mousePos.y <= (float)windowHeight * 0.83f){
		if(textScaleFactors[0] != 1.1f){
			soundEngine->play2D("Audio/Sounds/Pop.flac", false);
			textScaleFactors[0] = 1.1f;
			textColours[0] = glm::vec4(1.f, 1.f, 0.f, 1.f);
		}
		if(leftRightMB > 0.f && buttonBT <= elapsedTime){
			soundEngine->play2D("Audio/Sounds/Select.wav", false);

			/*if(guns[0]){
				delete guns[0];
				guns[0] = nullptr;
			}
			guns[0] = new Shotgun();
			if(guns[1]){
				delete guns[1];
				guns[1] = nullptr;
			}
			guns[1] = new Scar();
			if(guns[2]){
				delete guns[2];
				guns[2] = nullptr;
			}
			guns[2] = new Sniper();*/

			//screen = Screen::Game;

			cam.SetPos(glm::vec3(0.0f, 1500.0f, 2400.0f));
			cam.SetTarget(glm::vec3(0.0f, 1500.0f, 0.0f));
			cam.SetUp(glm::vec3(0.f, 1.f, 0.f));

			buttonBT = elapsedTime + .3f;
		}
	} else{
		textScaleFactors[0] = 1.f;
		textColours[0] = glm::vec4(1.f);
	}

	if(mousePos.x >= (float)windowWidth * 0.47f
		&& mousePos.x <= (float)windowWidth * 0.53f
		&& mousePos.y >= (float)windowHeight * 0.87f
		&& mousePos.y <= (float)windowHeight * 0.93f){
		if(textScaleFactors[1] != 1.1f){
			soundEngine->play2D("Audio/Sounds/Pop.flac", false);
			textScaleFactors[1] = 1.1f;
			textColours[1] = glm::vec4(1.f, 1.f, 0.f, 1.f);
		}
		if(leftRightMB > 0.f && buttonBT <= elapsedTime){
			soundEngine->play2D("Audio/Sounds/Select.wav", false);
			endLoop = true;
			buttonBT = elapsedTime + .3f;
		}
	} else{
		textScaleFactors[1] = 1.f;
		textColours[1] = glm::vec4(1.f);
	}
}

void MenuScene::LateUpdate(){
}

void MenuScene::PreRender(){
	forwardSP.Use();

	forwardSP.Set1f("shininess", 32.f); //More light scattering if lower
	forwardSP.Set3fv("globalAmbient", Light::globalAmbient);
	forwardSP.Set3fv("camPos", cam.GetPos());
	forwardSP.Set1i("dAmt", 2);

	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	forwardSP.Set1i("nightVision", 0);
}

void MenuScene::Render(){
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
		"Play",
		(float)windowWidth * 0.5f,
		(float)windowHeight * 0.2f,
		textScaleFactors[0],
		textColours[0],
		0,
		TextChief::TextAlignment::Center
	});
	textChief.RenderText(textSP, {
		"Quit",
		(float)windowWidth * 0.5f, 
		(float)windowHeight * 0.1f,
		textScaleFactors[1],
		textColours[1],
		0,
		TextChief::TextAlignment::Center
	});

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

void MenuScene::PostRender(){
	glDepthFunc(GL_LESS);
}