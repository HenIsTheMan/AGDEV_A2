#include "SceneSupport.h"

SceneSupport::~SceneSupport(){
}

void SceneSupport::EarlyInit(){
}

void SceneSupport::Init(){
}

void SceneSupport::FixedUpdate(){
}

void SceneSupport::Update(){
}

void SceneSupport::LateUpdate(){
}

void SceneSupport::PreRender(){
}

void SceneSupport::Render(){
}

void SceneSupport::PostRender(){
}

SceneSupport::SceneSupport():
	cam(glm::vec3(0.0f, 0.f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.f, 1.f, 0.0f), 0.0f, 1400.0f),
	soundEngine(nullptr)
{
}