#include "SceneManager.h"

#include "Scenes/StartScene.h"
#include "Scenes/MenuScene.h"
#include "Scenes/ShopScene.h"
#include "Scenes/ScoreboardScene.h"
#include "Scenes/SettingsScene.h"
#include "Scenes/CreditsScene.h"
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
	for(std::pair<const SceneID, IScene*>& element: im_Scenes){
		element.second->EarlyInit();
	}
}

void SceneManager::Init(){
	for(std::pair<const SceneID, IScene*>& element: im_Scenes){
		element.second->Init();
	}
}

void SceneManager::FixedUpdate(){
	im_CurrScene->FixedUpdate();
}

void SceneManager::Update(){
	if(im_NextScene != im_CurrScene){
		if(im_CurrScene != nullptr){
			im_CurrScene->Exit();
		}
		im_CurrScene = im_NextScene;
		im_CurrScene->Enter();
	}

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
	AddScene(SceneID::Start, new StartScene());
	AddScene(SceneID::Menu, new MenuScene());
	AddScene(SceneID::Shop, new ShopScene());
	AddScene(SceneID::Scoreboard, new ScoreboardScene());
	AddScene(SceneID::Settings, new SettingsScene());
	AddScene(SceneID::Credits, new CreditsScene());
	AddScene(SceneID::Game, new GameScene());

	SetNextScene(SceneID::Start);
}