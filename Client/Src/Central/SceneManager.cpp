#include "SceneManager.h"

SceneManager::~SceneManager(){
	for(std::pair<const SceneID, Scene*>& element: im_Scenes){
		Scene*& scene = element.second;
		if(scene != nullptr){
			delete scene;
			scene = nullptr;
		}
	}
}

void SceneManager::AddScene(const SceneID ID, Scene* const scene){
	im_Scenes.insert(std::make_pair(ID, scene));
}

void SceneManager::EarlyInit(){
}

void SceneManager::Init(){
}

void SceneManager::FixedUpdate(){
}

void SceneManager::Update(){
}

void SceneManager::LateUpdate(){
}

void SceneManager::PreRender() const{
}

void SceneManager::Render(){
}

void SceneManager::PostRender() const{
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
	Scene* const startScene = new GameScene();
	AddScene(SceneID::Game, startScene);
	im_CurrScene = im_NextScene = startScene;
}