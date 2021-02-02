#include "MenuScene.h"

MenuScene::MenuScene(){
}

MenuScene::~MenuScene(){
}

void MenuScene::EarlyInit(){
}

void MenuScene::Init(){
}

void MenuScene::FixedUpdate(){
}

void MenuScene::Update(){
}

void MenuScene::LateUpdate(){
}

void MenuScene::PreRender(){
	forwardSP.Use();

	forwardSP.Set1f("shininess", 32.f); //More light scattering if lower
	forwardSP.Set3fv("globalAmbient", Light::globalAmbient);
	forwardSP.Set3fv("camPos", cam.GetPos());
	forwardSP.Set1i("dAmt", 2);

	DirectionalLight* directionalLightFromTop = (DirectionalLight*)dLightFromTop;
	forwardSP.Set3fv("directionalLights[0].ambient", directionalLightFromTop->ambient);
	forwardSP.Set3fv("directionalLights[0].diffuse", directionalLightFromTop->diffuse);
	forwardSP.Set3fv("directionalLights[0].spec", directionalLightFromTop->spec);
	forwardSP.Set3fv("directionalLights[0].dir", directionalLightFromTop->dir);

	DirectionalLight* directionalLightFromBottom = (DirectionalLight*)dLightFromBottom;
	forwardSP.Set3fv("directionalLights[1].ambient", directionalLightFromBottom->ambient);
	forwardSP.Set3fv("directionalLights[1].diffuse", directionalLightFromBottom->diffuse);
	forwardSP.Set3fv("directionalLights[1].spec", directionalLightFromBottom->spec);
	forwardSP.Set3fv("directionalLights[1].dir", directionalLightFromBottom->dir);

	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	forwardSP.Set1i("nightVision", 0);
}

void MenuScene::Render(){
	modelStack.PushModel({
		modelStack.Scale(glm::vec3(float(winWidth) / 2.f, float(winHeight) / 2.f, 1.f)),
	});
		forwardSP.Set1i("noNormals", 1);
		Meshes::meshes[(int)MeshType::Quad]->SetModel(modelStack.GetTopModel());
		Meshes::meshes[(int)MeshType::Quad]->Render(forwardSP);
		forwardSP.Set1i("noNormals", 0);
	modelStack.PopModel();

	glDepthFunc(GL_GREATER);

	textChief.RenderText(textSP, {
		"Play",
		(float)winWidth * 0.5f,
		(float)winHeight * 0.2f,
		textScaleFactors[0],
		textColours[0],
		0,
		TextChief::TextAlignment::Center
	});
	textChief.RenderText(textSP, {
		"Quit",
		(float)winWidth * 0.5f, 
		(float)winHeight * 0.1f,
		textScaleFactors[1],
		textColours[1],
		0,
		TextChief::TextAlignment::Center
	});

	textChief.RenderText(textSP, {
		"ANOTHER",
		(float)winWidth * 0.5f, 
		(float)winHeight * 0.8f,
		4.0f,
		glm::vec4(glm::vec3(1.f, 0.f, 1.f), 1.f),
		0,
		TextChief::TextAlignment::Center
	});
	textChief.RenderText(textSP, {
		"WORLD",
		(float)winWidth * 0.5f,
		(float)winHeight * 0.6f,
		4.0f,
		glm::vec4(glm::vec3(1.f, 0.f, 1.f), 1.f),
		0,
		TextChief::TextAlignment::Center
	});
}

void MenuScene::PostRender(){
	glDepthFunc(GL_LESS);
}