#include "SceneManager.h"

#include "Scenes/MenuScene.h"
#include "Scenes/GameScene.h"

SceneManager::~SceneManager(){
	for(std::pair<const SceneID, IScene*>& element: im_Scenes){
		IScene*& scene = element.second;
		if(scene != nullptr){
			delete scene;
			scene = nullptr;
		}
	}
}

void SceneManager::AddScene(const SceneID ID, IScene* const scene){
	im_Scenes.insert(std::make_pair(ID, scene));
}

void SceneManager::EarlyInit(){
	im_CurrScene->EarlyInit();
}

void SceneManager::Init(){
	im_CurrScene->Init();
}

void SceneManager::FixedUpdate(){
	im_CurrScene->FixedUpdate();
}

void SceneManager::Update(){
	im_CurrScene->Update();
}

void SceneManager::LateUpdate(){
	im_CurrScene->LateUpdate();
}

void SceneManager::PreRender(){
	im_CurrScene->PreRender();
}

void SceneManager::Render(){
	im_CurrScene->Render();
}

void SceneManager::PostRender(){
	im_CurrScene->PostRender();
}

void SceneManager::SetCurrScene(const SceneID ID){
	im_CurrScene = im_Scenes.at(ID);
}

void SceneManager::SetNextScene(const SceneID ID){
	im_NextScene = im_Scenes.at(ID);
}

SceneManager::SceneManager():
	im_CurrScene(nullptr),
	im_NextScene(nullptr),
	im_Scenes()
{
	IScene* const startScene = new GameScene();
	AddScene(SceneID::Game, startScene);
	im_CurrScene = im_NextScene = startScene;
}