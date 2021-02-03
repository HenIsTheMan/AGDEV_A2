#include "GameScene.h"
#include "Vendor/stb_image.h"

#include "../../App.h"

#include "../../../Shared/Easing.hpp"

#include <glm/gtc/epsilon.hpp>

#include <atlstr.h>

#include "../../../Experimental/BoolType.hpp"
#include "../../../Experimental/StrType.hpp"

#include "../../../State/States/StateEnemyIdle.h"

#ifndef DEBUGGING
	#define DEBUGGING
#endif

float terrainXScale = 1.0f;
float terrainYScale = 1.0f;
float terrainZScale = 1.0f;

extern bool LMB;
extern bool RMB;
extern float dt;
extern float leftRightMB;
extern float angularFOV;
extern int windowWidth;
extern int windowHeight;

void SetUpCubemap(uint& cubemapRefID, const std::vector<cstr>& faces);

glm::vec3 Light::globalAmbient = glm::vec3(.2f);

GameScene::GameScene():
	SceneSupport(),
	models{},
	cubemapRefID(0),
	currSlot(0),
	inv{
		ItemType::Shotgun,
		ItemType::Scar,
		ItemType::Sniper,
		ItemType::None,
		ItemType::None,
	},
	currGun(nullptr),
	guns{
		nullptr,
		nullptr,
		nullptr,
	},
	reticleColour(glm::vec4(1.f)),
	shldUpdateEntityManager(true),
	shldRenderEntityManager(true),
	isCamDetached(true),
	shldRenderViewingFrustum(false),
	frustumColor(glm::vec3(1.0f)),
	dLightFromTop(nullptr),
	dLightFromBottom(nullptr),
	entityManager(EntityManager::GetObjPtr()),
	nodeManager(NodeManager::GetObjPtr()),
	regionManager(RegionManager::GetObjPtr()),
	myPlayer(nullptr),
	luaManager(LuaManager::GetObjPtr()),
	dataCoinAudio3D(new WIN32_FIND_DATA()),
	entityPool(ObjPool<Entity>::GetObjPtr()),
	wayptManager(WayptManager::GetObjPtr())
{
}

GameScene::~GameScene(){
	if(dLightFromTop){
		delete dLightFromTop;
		dLightFromTop = nullptr;
	}
	if(dLightFromBottom){
		delete dLightFromBottom;
		dLightFromBottom = nullptr;
	}

	for(short i = 0; i < 3; ++i){
		if(guns[i]){
			delete guns[i];
			guns[i] = nullptr;
		}
	}
	
	for(int i = 0; i < (int)MeshType::Amt; ++i){
		if(Meshes::meshes[i]){
			delete Meshes::meshes[i];
			Meshes::meshes[i] = nullptr;
		}
	}
	for(int i = 0; i < (int)ModelType::Amt; ++i){
		if(models[i]){
			delete models[i];
			models[i] = nullptr;
		}
	}

	if(entityManager != nullptr){
		entityManager->Destroy();
		entityManager = nullptr;
	}
	regionManager = nullptr; //Deleted elsewhere

	if(luaManager != nullptr){
		luaManager->Destroy();
		luaManager = nullptr;
	}

	if(dataCoinAudio3D != nullptr){
		delete dataCoinAudio3D;
		dataCoinAudio3D = nullptr;
	}

	if(entityPool != nullptr){
		entityPool->Destroy();
		entityPool = nullptr;
	}
	
	if(wayptManager != nullptr){
		wayptManager->Destroy();
		wayptManager = nullptr;
	}
}

void GameScene::Enter(){
	SceneSupport::Enter();

	glfwSetInputMode(App::RetrieveWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	cam.SetPos(glm::vec3(0.0f, 1500.0f, 2400.0f));
	cam.SetTarget(glm::vec3(0.0f, 1500.0f, 0.0f));
	cam.SetUp(glm::vec3(0.f, 1.f, 0.f));

	const int gunsSize = sizeof(guns) / sizeof(guns[0]);
	for(int i = 0; i < gunsSize; ++i){
		Gun*& gun = guns[i];

		if(gun != nullptr){
			delete gun;
		}

		switch(i){
			case 0:
				gun = new Shotgun();
				break;
			case 1:
				gun = new Scar();
				break;
			case 2:
				gun = new Sniper();
				break;
		}
	}

	currGun = guns[0];
}

void GameScene::Exit(){
	SceneSupport::Exit();
}

void GameScene::EarlyInit(){
	SceneSupport::EarlyInit();

	terrainXScale = luaManager->Read<float>("Scripts/Terrain.lua", "terrainXScale", true);
	terrainYScale = luaManager->Read<float>("Scripts/Terrain.lua", "terrainYScale", true);
	terrainZScale = luaManager->Read<float>("Scripts/Terrain.lua", "terrainZScale", true);

	std::vector<cstr> fPaths = luaManager->ReadFromArr<cstr>("Scripts/Models.lua", "fPathModels", 1, (int)ModelType::Amt, true);
	const int modelsSize = sizeof(models) / sizeof(models[0]);

	for(int i = 0; i < modelsSize; ++i){
		models[i] = new Model(fPaths[i], {
			aiTextureType_DIFFUSE,
		});
	}

	wayptManager->InitWayptPool(999, 999);
	wayptManager->ReserveWaypts(999);

	const int amtOfWaypts = luaManager->CallLuaFunc<int>("Scripts/Waypts.lua", "AmtOfWaypts", {}, true);
	const std::vector<glm::vec3> wayptPos = luaManager->ReadFromJaggedArr<glm::vec3>("Scripts/Waypts.lua", "wayptPos", 1, amtOfWaypts, 1, 3, true);
	const int wayptPosSize = (int)wayptPos.size();

	for(int i = 0; i < wayptPosSize; ++i){
		wayptManager->AddWaypt(wayptPos[i]);
	}
}

void GameScene::Init(){
	SceneSupport::Init();

	InitEntities();

	entityManager->isCamDetached = isCamDetached;

	SetUpCubemap(cubemapRefID, luaManager->ReadFromArr<cstr>("Scripts/Skyboxes.lua", "skybox0", 1, 6, true));

	StrType fPathCoin;
	DoubleType myStartCoin;
	DoubleType myEndCoin;
	DoubleType timeCoin;

	luaManager->ReadCustomFromTable(
		"Scripts/CoinSpriteAnimData.lua",
		{&fPathCoin, &myStartCoin, &myEndCoin, &timeCoin},
		"coinSpriteAnimData",
		{"fPath", "myStart", "myEnd", "time"},
		true
	);

	Meshes::meshes[(int)MeshType::CoinSpriteAni]->AddTexMap({static_cast<StrType*>(&fPathCoin)->data, Mesh::TexType::Diffuse, 0});
	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::CoinSpriteAni])->AddAni(
		"CoinSpriteAni",
		(int)static_cast<DoubleType*>(&myStartCoin)->data,
		(int)static_cast<DoubleType*>(&myEndCoin)->data
	);
	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::CoinSpriteAni])->Play("CoinSpriteAni", -1, (float)static_cast<DoubleType*>(&timeCoin)->data);

	StrType fPathFire;
	DoubleType myStartFire;
	DoubleType myEndFire;
	DoubleType timeFire;

	luaManager->ReadCustomFromTable(
		"Scripts/FireSpriteAnimData.lua",
		{&fPathFire, &myStartFire, &myEndFire, &timeFire},
		"fireSpriteAnimData",
		{"fPath", "myStart", "myEnd", "time"},
		true
	);

	Meshes::meshes[(int)MeshType::FireSpriteAni]->AddTexMap({static_cast<StrType*>(&fPathFire)->data, Mesh::TexType::Diffuse, 0});
	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::FireSpriteAni])->AddAni(
		"FireSpriteAni",
		(int)static_cast<DoubleType*>(&myStartFire)->data,
		(int)static_cast<DoubleType*>(&myEndFire)->data
	);
	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::FireSpriteAni])->Play("FireSpriteAni", -1, (float)static_cast<DoubleType*>(&timeFire)->data);

	Meshes::meshes[(int)MeshType::Terrain]->AddTexMap({"Imgs/Floor.jpg", Mesh::TexType::Diffuse, 0});

	dLightFromTop = CreateLight(LightType::Directional);
	dLightFromBottom = CreateLight(LightType::Directional);

	dLightFromTop->ambient = glm::vec3(0.3f);
	static_cast<DirectionalLight*>(dLightFromBottom)->dir = glm::vec3(0.0f, 1.0f, 0.0f);
	dLightFromBottom->diffuse = glm::vec3(0.5f, 0.0f, 0.0f);
}

void GameScene::FixedUpdate(){
	SceneSupport::FixedUpdate();
}

void GameScene::Update(){
	SceneSupport::Update();

	StateEnemyIdle::elapsedTime = elapsedTime;

	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::CoinSpriteAni])->Update();
	static_cast<SpriteAni*>(Meshes::meshes[(int)MeshType::FireSpriteAni])->Update();

	TuneCoins("Scripts/CoinAudio3D.lua");

	static bool isPressedB = false;
	if(!isPressedB && Key(GLFW_KEY_B)){
		isCamDetached = !isCamDetached;
		entityManager->isCamDetached = isCamDetached;
		angularFOV = 45.f;

		isPressedB = true;
	} else if(isPressedB && !Key(GLFW_KEY_B)){
		isPressedB = false;
	}

	static bool isPressedM = false;
	if(!isPressedM && Key(GLFW_KEY_M)){
		shldRenderViewingFrustum = !shldRenderViewingFrustum;

		isPressedM = true;
	} else if(isPressedM && !Key(GLFW_KEY_M)){
		isPressedM = false;
	}

	const glm::vec3 camAttachedPos = myPlayer->GetPos() + glm::vec3(0.0f, myPlayer->GetScale().y * 0.5f, 0.0f);
	if(isCamDetached){
		cam.UpdateDetached(GLFW_KEY_E, GLFW_KEY_Q, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
	} else{
		cam.UpdateAttached(camAttachedPos);
		const_cast<Entity*>(myPlayer)->SetFacingDir(cam.CalcFront());

		if(RMB){ //Control angularFOV of perspective projection based on item selected in inv
			switch(inv[currSlot]){
				case ItemType::Shotgun:
					angularFOV = 40.0f;
					break;
				case ItemType::Scar:
					angularFOV = 30.0f;
					break;
				case ItemType::Sniper:
					if(angularFOV != 15.0f){
						ISound* const sound = soundEngine->play2D("Audio/Sounds/Scope.wav", false, true);
						sound->setVolume(luaManager->Read<float>("Scripts/Audio.lua", "scopeVol", true));
						sound->setIsPaused(false);
					}
					angularFOV = 15.0f;
					break;
			}
		} else{
			angularFOV = 45.f;
		}

		if(currGun){ //Control shooting and reloading of currGun
			if(LMB){
				currGun->Shoot(elapsedTime, cam.GetPos(), cam.CalcFront(), soundEngine);
			}
			if(Key(GLFW_KEY_R)){
				currGun->Reload(soundEngine);
			}
			currGun->Update();
		}

		//* Track changing of inv slots
		int keys[]{
			GLFW_KEY_1,
			GLFW_KEY_2,
			GLFW_KEY_3,
			GLFW_KEY_4,
			GLFW_KEY_5,
		};
		for(int i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i){
			if(Key(keys[i])){
				currSlot = keys[i] - 49;
				break;
			}
		}
		switch(inv[currSlot]){
			case ItemType::Shotgun:
			case ItemType::Scar:
			case ItemType::Sniper:
				currGun = guns[int(inv[currSlot])];
				break;
			default:
				currGun = nullptr;
		}
		//*/
	}

	const glm::vec3& playerFacingDir = myPlayer->GetFacingDir();
	const float camAspectRatio = cam.GetAspectRatio();

	static glm::vec3 prevCamAttachedPos = glm::vec3();
	static glm::vec3 prevPlayerFacingDir = glm::vec3();
	static float prevAngularFOV = 0.0f;
	static float prevCamAspectRatio = 0.0f;

	if(glm::any(glm::epsilonNotEqual(camAttachedPos, prevCamAttachedPos, glm::epsilon<float>()))
		|| glm::any(glm::epsilonNotEqual(playerFacingDir, prevPlayerFacingDir, glm::epsilon<float>()))
		|| glm::epsilonNotEqual(angularFOV, prevAngularFOV, glm::epsilon<float>())
		|| glm::epsilonNotEqual(camAspectRatio, prevCamAspectRatio, glm::epsilon<float>())
	){
		const glm::vec3& playerPos = myPlayer->GetPos();
		const float yMin = -FLT_MAX;
		const float yMax = terrainYScale * 4.0f;

		regionManager->UpdateFrustumCulling(glm::lookAt(camAttachedPos, camAttachedPos + playerFacingDir,
			glm::normalize(glm::cross(glm::normalize(glm::cross(playerFacingDir, glm::vec3(0.0f, 1.0f, 0.0f))), playerFacingDir))),
			glm::perspective(glm::radians(angularFOV), camAspectRatio, .1f, 70000.0f),
			yMin,
			yMax
		);

		frustumColor = glm::vec3(PseudorandMinMax(0.0f, 1.0f), PseudorandMinMax(0.0f, 1.0f), PseudorandMinMax(0.0f, 1.0f));

		prevCamAttachedPos = camAttachedPos;
		prevPlayerFacingDir = playerFacingDir;
		prevAngularFOV = angularFOV;
		prevCamAspectRatio = camAspectRatio;
	}

	#if defined DEBUGGING
	static bool isPressed6 = false;
	if(!isPressed6 && Key(GLFW_KEY_6)){
		shldUpdateEntityManager = !shldUpdateEntityManager;

		isPressed6 = true;
	} else if(isPressed6 && !Key(GLFW_KEY_6)){
		isPressed6 = false;
	}
	#endif

	if(shldUpdateEntityManager){
		entityManager->Update(cam);
	}
}

void GameScene::LateUpdate(){
	SceneSupport::LateUpdate();
}

void GameScene::PreRender(){
	SceneSupport::PreRender();

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
}

void GameScene::Render(){
	SceneSupport::Render();

	const glm::vec3 OGPos = cam.GetPos();
	const glm::vec3 OGTarget = cam.GetTarget();
	const glm::vec3 OGUp = cam.GetUp();

	view = cam.LookAt();
	projection = glm::perspective(glm::radians(angularFOV), cam.GetAspectRatio(), .1f, 99999.0f);
	forwardSP.SetMat4fv("PV", &(projection * glm::mat4(glm::mat3(view)))[0][0]);

	//* Render skybox
	Mesh* const cubeMesh = Meshes::meshes[(int)MeshType::Cube];
	glDepthFunc(GL_LEQUAL); //Modify comparison operators used for depth test such that frags with depth <= 1.f are shown
	glCullFace(GL_FRONT);
		forwardSP.Set1i("sky", 1);
		forwardSP.Set1i("skybox", 1);
		forwardSP.UseTex(cubemapRefID, "cubemapSampler", GL_TEXTURE_CUBE_MAP);
		cubeMesh->SetModel(modelStack.GetTopModel());
		cubeMesh->Render(forwardSP);
		forwardSP.Set1i("skybox", 0);
		forwardSP.Set1i("sky", 0);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	//*/

	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);

	//* Render terrain
	modelStack.PushModel({
		modelStack.Scale(glm::vec3(terrainXScale, terrainYScale, terrainZScale)),
	});
		Meshes::meshes[(int)MeshType::Terrain]->SetModel(modelStack.GetTopModel());
		Meshes::meshes[(int)MeshType::Terrain]->Render(forwardSP);
	modelStack.PopModel();
	//*/

	//* Render waypts
	const std::vector<Waypt*>& waypts = wayptManager->GetWaypts();
	const int wayPtsSize = (int)waypts.size();

	for(int i = 0; i < wayPtsSize; ++i){
		modelStack.PushModel({
			modelStack.Translate(glm::vec3(waypts[i]->pos)),
			modelStack.Scale(glm::vec3(70.0f)),
		});
			Meshes::meshes[(int)MeshType::Sphere]->SetModel(modelStack.GetTopModel());
			Meshes::meshes[(int)MeshType::Sphere]->Render(forwardSP);
		modelStack.PopModel();
	}
	//*/

	static float yStart = 400.0f;
	static float yEnd = -400.0f;
	const float factor = EaseOutBounce((cos(elapsedTime) + 1.0f) * 0.5f);

	modelStack.PushModel({
		modelStack.Translate(glm::vec3(0.0f, glm::mix(yStart, yEnd, factor), 0.0f)),
	});
		cubeMesh->SetModel(modelStack.GetTopModel());
		cubeMesh->InstancedRender(forwardSP);
	modelStack.PopModel();

	const glm::vec3 playerPos = myPlayer->GetPos();

	Model* const treeModel = treeLOD.GetModel(
		luaManager->CallLuaFunc<float>("Scripts/DistSquared.lua", "DistSquared", {playerPos.x, 0.0f, playerPos.z, 0.0f, 0.0f, -terrainZScale * 0.25f}, true)
		- luaManager->CallLuaFunc<float>("Scripts/ValSquared.lua", "ValSquared", {(angularFOV - 45.0f) * 200.0f}, true));
	if(treeModel != nullptr){
		treeModel->InstancedRender(forwardSP);
	}

	models[(int)ModelType::Flower]->InstancedRender(forwardSP);
	models[(int)ModelType::Grass]->InstancedRender(forwardSP);
	models[(int)ModelType::Rock]->InstancedRender(forwardSP);

	#if defined DEBUGGING
	static bool isPressed7 = false;
	if(!isPressed7 && Key(GLFW_KEY_7)){
		shldRenderEntityManager = !shldRenderEntityManager;

		isPressed7 = true;
	} else if(isPressed7 && !Key(GLFW_KEY_7)){
		isPressed7 = false;
	}
	#endif

	if(shldRenderEntityManager){
		entityManager->Render(forwardSP, cam);
	}

	if(shldRenderViewingFrustum){
		viewingFrustumSP.Use();

		modelStack.PushModel({
		});
			viewingFrustumSP.SetMat4fv("MVP", &(projection * view * modelStack.GetTopModel())[0][0]);
			viewingFrustumSP.Set3fv("frustumColor", frustumColor);
			Meshes::meshes[(int)MeshType::ViewingFrustum]->Render(viewingFrustumSP);
		modelStack.PopModel();

		forwardSP.Use();
	}

	///Render item held
	if(!isCamDetached){
		const glm::vec3 front = cam.CalcFront();
		const float sign = front.y < 0.f ? -1.f : 1.f;

		const glm::mat4 rotationMat = glm::rotate(glm::mat4(1.f), sign * acosf(glm::dot(front, glm::normalize(glm::vec3(front.x, 0.f, front.z)))),
			glm::normalize(glm::vec3(-front.z, 0.f, front.x)));

		switch(inv[currSlot]){
			case ItemType::Shotgun: {
				modelStack.PushModel({
					modelStack.Translate(cam.GetPos() +
						glm::vec3(rotationMat * glm::vec4(RotateVecIn2D(glm::vec3(5.5f, -7.f, -13.f), atan2(front.x, front.z) + glm::radians(180.f), Axis::y), 1.f))
				),
					modelStack.Rotate(glm::vec4(glm::vec3(-front.z, 0.f, front.x), sign * glm::degrees(acosf(glm::dot(front, glm::normalize(glm::vec3(front.x, 0.f, front.z))))))), 
					modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, glm::degrees(atan2(front.x, front.z)))), 
					modelStack.Scale(glm::vec3(3.f)),
				});
					models[(int)ModelType::Shotgun]->SetModelForAll(modelStack.GetTopModel());
					models[(int)ModelType::Shotgun]->Render(forwardSP);
				modelStack.PopModel();
				break;
			}
			case ItemType::Scar: {
				modelStack.PushModel({
					modelStack.Translate(cam.GetPos() +
						glm::vec3(rotationMat * glm::vec4(RotateVecIn2D(glm::vec3(5.f, -4.f, -12.f), atan2(front.x, front.z) + glm::radians(180.f), Axis::y), 1.f))
				),
					modelStack.Rotate(glm::vec4(glm::vec3(-front.z, 0.f, front.x), sign * glm::degrees(acosf(glm::dot(front, glm::normalize(glm::vec3(front.x, 0.f, front.z))))))), 
					modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, glm::degrees(atan2(front.x, front.z)))), 
					modelStack.Scale(glm::vec3(3.f)),
				});
					models[(int)ModelType::Scar]->SetModelForAll(modelStack.GetTopModel());
					models[(int)ModelType::Scar]->Render(forwardSP);
					modelStack.PopModel();
				break;
			}
			case ItemType::Sniper: {
				modelStack.PushModel({
					modelStack.Translate(cam.GetPos() +
					glm::vec3(rotationMat * glm::vec4(RotateVecIn2D(glm::vec3(5.f, -6.f, -13.f), atan2(front.x, front.z) + glm::radians(180.f), Axis::y), 1.f))
				),
					modelStack.Rotate(glm::vec4(glm::vec3(-front.z, 0.f, front.x), sign * glm::degrees(acosf(glm::dot(front, glm::normalize(glm::vec3(front.x, 0.f, front.z))))))), 
					modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, glm::degrees(atan2(front.x, front.z)) - 90.f)), 
					modelStack.Scale(glm::vec3(2.f)),
				});
					models[(int)ModelType::Sniper]->SetModelForAll(modelStack.GetTopModel());
					models[(int)ModelType::Sniper]->Render(forwardSP);
					modelStack.PopModel();
				break;
			}
		}
	}

	cam.SetPos(glm::vec3(0.f, 0.f, 5.f));
	cam.SetTarget(glm::vec3(0.f));
	cam.SetUp(glm::vec3(0.f, 1.f, 0.f));
	view = cam.LookAt();
	projection = glm::ortho(-float(windowWidth) / 2.f, float(windowWidth) / 2.f, -float(windowHeight) / 2.f, float(windowHeight) / 2.f, .1f, 99999.0f);
	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);

	if(!isCamDetached){
		if(!(RMB && inv[currSlot] == ItemType::Sniper)){
			///Render items in inv
			for(short i = 0; i < 5; ++i){
				forwardSP.Set1i("noNormals", 1);
				modelStack.PushModel({
					modelStack.Translate(glm::vec3(float(i) * 100.f - 200.f, -float(windowHeight) / 2.3f, -10.f)),
				});
				switch(inv[i]){
					case ItemType::Shotgun:
						modelStack.PushModel({
							modelStack.Translate(glm::vec3(18.f, -18.f, 0.f)),
							modelStack.Rotate(glm::vec4(0.f, 0.f, 1.f, 45.f)),
							modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, 90.f)),
							modelStack.Scale(glm::vec3(21.f)),
						});
						models[(int)ModelType::Shotgun]->SetModelForAll(modelStack.GetTopModel());
						models[(int)ModelType::Shotgun]->Render(forwardSP);
						modelStack.PopModel();
						break;
					case ItemType::Scar:
						modelStack.PushModel({
							modelStack.Rotate(glm::vec4(0.f, 0.f, 1.f, 45.f)),
							modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, 90.f)),
							modelStack.Scale(glm::vec3(18.f)),
						});
							models[(int)ModelType::Scar]->SetModelForAll(modelStack.GetTopModel());
							models[(int)ModelType::Scar]->Render(forwardSP);
						modelStack.PopModel();
						break;
					case ItemType::Sniper:
						modelStack.PushModel({
							modelStack.Translate(glm::vec3(16.f, -15.f, 0.f)),
							modelStack.Rotate(glm::vec4(0.f, 0.f, 1.f, 45.f)),
							modelStack.Scale(glm::vec3(10.f)),
						});
							models[(int)ModelType::Sniper]->SetModelForAll(modelStack.GetTopModel());
							models[(int)ModelType::Sniper]->Render(forwardSP);
						modelStack.PopModel();
						break;
					case ItemType::ShotgunAmmo:
						break;
					case ItemType::ScarAmmo:
						break;
					case ItemType::SniperAmmo:
						break;
				}
				modelStack.PopModel();
				forwardSP.Set1i("noNormals", 0);
			}
		}

		Mesh* const quadMesh = Meshes::meshes[(int)MeshType::Quad];

		if(RMB && inv[currSlot] == ItemType::Sniper){
			forwardSP.Set1i("nightVision", 1);

			forwardSP.Set1i("useCustomDiffuseTexIndex", 1);
			forwardSP.Set1i("noNormals", 1);

			modelStack.PushModel({
				modelStack.Translate(glm::vec3(0.f, 0.f, -9.f)),
				modelStack.Scale(glm::vec3(float(windowHeight) * 0.7f, float(windowHeight) * 0.7f, 1.f)),
			});
				forwardSP.Set1i("customDiffuseTexIndex", 1);
				quadMesh->SetModel(modelStack.GetTopModel());
				quadMesh->Render(forwardSP);
			modelStack.PopModel();
			modelStack.PushModel({
				modelStack.Translate(glm::vec3(0.f, 0.f, -9.1f)),
				modelStack.Scale(glm::vec3(float(windowWidth) / 2.f, float(windowHeight) / 2.f, 1.f)),
			});
				forwardSP.Set1i("customDiffuseTexIndex", -1);
				forwardSP.Set1i("useCustomColour", 1);
				forwardSP.Set4fv("customColour", glm::vec4(glm::vec3(0.f), 1.f));
				quadMesh->SetModel(modelStack.GetTopModel());
				quadMesh->Render(forwardSP);
				forwardSP.Set1i("useCustomColour", 0);
			modelStack.PopModel();

			forwardSP.Set1i("noNormals", 0);
			forwardSP.Set1i("useCustomDiffuseTexIndex", 0);
		} else{
			forwardSP.Set1i("nightVision", 0);

			///Render inv slots
			for(short i = 0; i < 5; ++i){
				modelStack.PushModel({
					modelStack.Translate(glm::vec3(float(i) * 100.f - 200.f, -float(windowHeight) / 2.3f, -10.f)),
					modelStack.Scale(glm::vec3(50.f, 50.f, 1.f)),
				});
					forwardSP.Set1i("noNormals", 1);
					if(i == currSlot){
						forwardSP.Set1i("useCustomColour", 1);
						forwardSP.Set4fv("customColour", glm::vec4(1.f, 1.f, 0.f, 1.f));
					}
					forwardSP.Set1i("useCustomDiffuseTexIndex", 1);
					forwardSP.Set1i("customDiffuseTexIndex", 3);
					quadMesh->SetModel(modelStack.GetTopModel());
					quadMesh->Render(forwardSP);
					forwardSP.Set1i("useCustomDiffuseTexIndex", 0);
					if(i == currSlot){
						forwardSP.Set1i("useCustomColour", 0);
					}
					forwardSP.Set1i("noNormals", 0);
				modelStack.PopModel();
			}

			///Render reticle
			if(inv[currSlot] == ItemType::Shotgun || inv[currSlot] == ItemType::Scar){
				modelStack.PushModel({
					modelStack.Scale(glm::vec3(40.f, 40.f, 1.f)),
				});
					forwardSP.Set1i("noNormals", 1);
					forwardSP.Set1i("useCustomColour", 1);
					forwardSP.Set4fv("customColour", reticleColour);
					forwardSP.Set1i("useCustomDiffuseTexIndex", 1);
					forwardSP.Set1i("customDiffuseTexIndex", 4);
					quadMesh->SetModel(modelStack.GetTopModel());
					quadMesh->Render(forwardSP);
					forwardSP.Set1i("useCustomDiffuseTexIndex", 0);
					forwardSP.Set1i("useCustomColour", 0);
					forwardSP.Set1i("noNormals", 0);

					modelStack.PushModel({
						modelStack.Translate(glm::vec3(0.f, 0.f, 1.f)),
					});
					if(RMB){
						modelStack.PushModel({
							modelStack.Scale(glm::vec3(.7f, .7f, 1.f)),
						});
					}
						forwardSP.Set1i("noNormals", 1);
						forwardSP.Set1i("useCustomColour", 1);
						forwardSP.Set4fv("customColour", reticleColour);
						forwardSP.Set1i("useCustomDiffuseTexIndex", 1);
						forwardSP.Set1i("customDiffuseTexIndex", inv[currSlot] == ItemType::Shotgun ? 5 : 6);
						quadMesh->SetModel(modelStack.GetTopModel());
						quadMesh->Render(forwardSP);
						forwardSP.Set1i("useCustomDiffuseTexIndex", 0);
						forwardSP.Set1i("useCustomColour", 0);
						forwardSP.Set1i("noNormals", 0);
					modelStack.PopModel();
					modelStack.PopModel();
				modelStack.PopModel();
			}
		}
	}

	cam.SetPos(OGPos);
	cam.SetTarget(OGTarget);
	cam.SetUp(OGUp);
	view = cam.LookAt();
	projection = glm::perspective(glm::radians(angularFOV), cam.GetAspectRatio(), .1f, 99999.0f);
	forwardSP.SetMat4fv("PV", &(projection * view)[0][0]);

	///Render bullet info
	if(!isCamDetached && currGun){
		textChief.RenderText(textSP, {
			std::to_string(currGun->GetLoadedBullets()) + " / " + std::to_string(currGun->GetUnloadedBullets()),
			float(windowWidth) * 0.5f,
			float(windowHeight) * 0.15f,
			1.f,
			glm::vec4(1.f, 1.f, 0.f, 1.f),
			0,
			TextChief::TextAlignment::Center
		});
	}

	const float FPS = 1.0f / dt;
	textChief.RenderText(textSP, {
		"FPS: " + std::to_string(FPS).substr(0, std::to_string((int)FPS).length() + 3),
		25.0f,
		(float)windowHeight * 0.02f,
		1.f,
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		0,
	});

	textChief.RenderText(textSP, {
		"Instanced draw calls: " + std::to_string(Mesh::instancedDrawCalls),
		25.f,
		(float)windowHeight * 0.55f,
		1.f,
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		0,
	});

	textChief.RenderText(textSP, {
		"Normal draw calls: " + std::to_string(Mesh::normalDrawCalls),
		25.f,
		(float)windowHeight * 0.65f,
		1.f,
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		0,
	});

	textChief.RenderText(textSP, {
		"Polygon count: " + std::to_string(Mesh::polygonCount),
		25.f,
		(float)windowHeight * 0.75f,
		1.f,
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		0,
	});

	textChief.RenderText(textSP, {
		"Index count: " + std::to_string(Mesh::indexCount),
		25.f,
		(float)windowHeight * 0.85f,
		1.f,
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		0,
	});

	textChief.RenderText(textSP, {
		"Vertex count: " + std::to_string(Mesh::vertexCount),
		25.f,
		(float)windowHeight * 0.95f,
		1.f,
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		0,
	});

	Mesh::instancedDrawCalls = 0;
	Mesh::normalDrawCalls = 0;
	Mesh::vertexCount = 0;
	Mesh::indexCount = 0;
	Mesh::polygonCount = 0;
}

void GameScene::PostRender(){
	SceneSupport::PostRender();

	glBlendFunc(GL_ONE, GL_ZERO);
}

void GameScene::InitEntities(){
	entityManager->Init();
	CreateEntities();
	entityManager->SetUpRegionsForStationary();

	CreateTreesAndCubes();
	CreateDecorations();

	const std::vector<float> vals = luaManager->ReadFromTable<float>("Scripts/LOD.lua", "distsSquaredTreeLOD", {
		"high",
		"medium",
		"low",
	}, true);
	treeLOD.SetDistSquaredAndModel(DetailLvl::High, vals[0],  models[(int)ModelType::Tree_High]);
	treeLOD.SetDistSquaredAndModel(DetailLvl::Medium, vals[1],  models[(int)ModelType::Tree_Medium]);
	treeLOD.SetDistSquaredAndModel(DetailLvl::Low, vals[2], models[(int)ModelType::Tree_Low]);
}

void GameScene::CreateEntities(){
	Terrain* const myTerrain = static_cast<Terrain*>(Meshes::meshes[(int)MeshType::Terrain]);

	EntityFactory* const entityFactory = entityManager->RetrieveEntityFactory();

	//* Create Player
	myPlayer = entityFactory->CreatePlayer({
		glm::vec3(),
		glm::vec3(50.0f, 200.0f, 50.0f),
		glm::vec4(1.0f),
		-1,
	});

	assert(myPlayer != nullptr && "Var 'myPlayer' is nullptr");
	//*/

	//* Create enemy
	const float xyScaleEnemyBody = 200.0f;
	const Entity* const enemyBody = entityFactory->CreateEnemyBody({
		glm::vec3(
			0.0f,
			terrainYScale * 2.0f,
			terrainZScale * 0.4f
		),
		glm::vec3(xyScaleEnemyBody, xyScaleEnemyBody, 5.0f),
		glm::vec4(0.3f, 0.3f, 0.3f, 0.4f),
		-1,
	});

	const Entity* enemyParts[6]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	Node* const rootNode = nodeManager->RetrieveRootNode();

	for(int i = 0; i < 6; ++i){
		const Entity*& enemyPart = enemyParts[i];
		enemyPart = entityFactory->CreateEnemyPart({
			glm::vec3(),
			glm::vec3(),
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			-1,
		});

		Node* const enemyPartNode = rootNode->DetachChild(enemyPart);
		Node* const enemyBodyNode = rootNode->FindChild(enemyBody);
		enemyBodyNode->AddChild(enemyPartNode);

		enemyPartNode->SetUseLocalRotationUpdate(true);
		enemyPartNode->SetLocalDilation(glm::vec3(0.5f));
		switch(i){
			case 0:
				enemyPartNode->SetLocalTranslation(glm::vec3(800.0f, 0.0f, 0.0f));
				enemyPartNode->SetLocalRotationUpdate(glm::quat(glm::vec3(0.0f, 10.0f, 0.0f)));
				break;
			case 1:
				enemyPartNode->SetLocalTranslation(glm::vec3(800.0f, 0.0f, 0.0f));
				enemyPartNode->SetLocalRotationUpdate(glm::quat(glm::vec3(0.0f, 0.0f, 10.0f)));
				break;
			case 2:
				enemyPartNode->SetLocalTranslation(glm::vec3(0.0f, 800.0f, 0.0f));
				enemyPartNode->SetLocalRotationUpdate(glm::quat(glm::vec3(10.0f, 0.0f, 0.0f)));
				break;
			case 3:
				enemyPartNode->SetLocalTranslation(glm::vec3(0.0f, 800.0f, 0.0f));
				enemyPartNode->SetLocalRotationUpdate(glm::quat(glm::vec3(0.0f, 0.0f, 10.0f)));
				break;
			case 4:
				enemyPartNode->SetLocalTranslation(glm::vec3(0.0f, 0.0f, 800.0f));
				enemyPartNode->SetLocalRotationUpdate(glm::quat(glm::vec3(10.0f, 0.0f, 0.0f)));
				break;
			case 5:
				enemyPartNode->SetLocalTranslation(glm::vec3(0.0f, 0.0f, 800.0f));
				enemyPartNode->SetLocalRotationUpdate(glm::quat(glm::vec3(0.0f, 10.0f, 0.0f)));
				break;
		}
	}
	//*/

	//* Create coins
	for(short i = 0; i < 20; ++i){
		const float scaleFactor = 70.0f;
		const float offsetFactor = 5.0f;
		const float xPos = PseudorandMinMax(-terrainXScale * 0.5f + offsetFactor, terrainXScale * 0.5f - offsetFactor);
		const float zPos = PseudorandMinMax(offsetFactor, terrainZScale * 0.5f - offsetFactor);
		const glm::vec3 pos = glm::vec3(
			xPos,
			terrainYScale * myTerrain->GetHeightAtPt(xPos / terrainXScale, zPos / terrainZScale, false) + scaleFactor,
			zPos
		);
		entityFactory->CreateCoin({
			pos,
			glm::vec3(scaleFactor),
			glm::vec4(1.0f),
			-1,
		});
	}
	//*/
}

void GameScene::CreateTreesAndCubes(){
	Model* const treeHigh = models[(int)ModelType::Tree_High];
	treeHigh->ReserveModelMatsForAll(2000);
	treeHigh->ReserveColorsForAll(2000);
	treeHigh->ReserveDiffuseTexIndicesForAll(2000);

	Model* const treeMedium = models[(int)ModelType::Tree_Medium];
	treeMedium->ReserveModelMatsForAll(2000);
	treeMedium->ReserveColorsForAll(2000);
	treeMedium->ReserveDiffuseTexIndicesForAll(2000);

	Model* const treeLow = models[(int)ModelType::Tree_Low];
	treeLow->ReserveModelMatsForAll(2000);
	treeLow->ReserveColorsForAll(2000);
	treeLow->ReserveDiffuseTexIndicesForAll(2000);

	Mesh* const cubeMesh = Meshes::meshes[(int)MeshType::Cube];
	cubeMesh->ReserveModelMats(2000);
	cubeMesh->ReserveColors(2000);
	cubeMesh->ReserveDiffuseTexIndices(2000);

	for(int i = 0; i < 2000; ++i){
		const float offsetFactor = 100.0f;
		const float xPos = PseudorandMinMax(-terrainXScale * 0.5f + offsetFactor, terrainXScale * 0.5f - offsetFactor);
		const float zPos = PseudorandMinMax(-terrainZScale * 0.5f + offsetFactor, -offsetFactor);
		const glm::vec3 pos = glm::vec3(
			xPos,
			terrainYScale * static_cast<Terrain*>(Meshes::meshes[(int)MeshType::Terrain])->GetHeightAtPt(xPos / terrainXScale, zPos / terrainZScale, false),
			zPos
		);

		modelStack.PushModel({
			modelStack.Translate(pos),
			modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, PseudorandMinMax(0.0f, 360.0f))),
			modelStack.Scale(glm::vec3((float)PseudorandMinMax(210, 400), (float)PseudorandMinMax(180, 300), (float)PseudorandMinMax(210, 400)))
		});
			const glm::mat4& modelMat = modelStack.GetTopModel();
			const glm::vec3& color = glm::vec3(PseudorandMinMax(0.25f, 1.0f), 0.0f, 0.0f);
			const int diffuseTexIndex = 0;

			treeHigh->AddModelMatForAll(modelMat);
			treeHigh->AddColorForAll(color);
			treeHigh->AddDiffuseTexIndexForAll(diffuseTexIndex);

			treeMedium->AddModelMatForAll(modelMat);
			treeMedium->AddColorForAll(color);
			treeMedium->AddDiffuseTexIndexForAll(diffuseTexIndex);

			treeLow->AddModelMatForAll(modelMat);
			treeLow->AddColorForAll(color);
			treeLow->AddDiffuseTexIndexForAll(diffuseTexIndex);
		modelStack.PopModel();

		modelStack.PushModel({
			modelStack.Translate(pos + glm::vec3(0.0f, PseudorandMinMax(1600.0f, 2000.0f), 0.0f)),
			modelStack.QuatRotate(glm::quat(glm::vec3((float)PseudorandMinMax(0, 360), (float)PseudorandMinMax(0, 360), (float)PseudorandMinMax(0, 360)))),
			modelStack.Scale(glm::vec3((float)PseudorandMinMax(35, 55))),
		});
			cubeMesh->AddModelMat(modelStack.GetTopModel());
			cubeMesh->AddColor(glm::vec4(1.0f));
			cubeMesh->AddDiffuseTexIndex(PseudorandMinMax(0, 11));
		modelStack.PopModel();
	}
}

void GameScene::CreateDecorations(){
	Model* const flower = models[(int)ModelType::Flower];
	flower->ReserveModelMatsForAll(2000);
	flower->ReserveColorsForAll(2000);
	flower->ReserveDiffuseTexIndicesForAll(2000);

	Model* const grass = models[(int)ModelType::Grass];
	grass->ReserveModelMatsForAll(2000);
	grass->ReserveColorsForAll(2000);
	grass->ReserveDiffuseTexIndicesForAll(2000);

	Model* const rock = models[(int)ModelType::Rock];
	rock->ReserveModelMatsForAll(2000);
	rock->ReserveColorsForAll(2000);
	rock->ReserveDiffuseTexIndicesForAll(2000);

	Terrain* const myTerrain = static_cast<Terrain*>(Meshes::meshes[(int)MeshType::Terrain]);

	for(int i = 0; i < 2000; ++i){
		const float offsetFactor0 = 50.0f;
		const float xPos0 = PseudorandMinMax(-terrainXScale * 0.5f + offsetFactor0, terrainXScale * 0.5f - offsetFactor0);
		const float zPos0 = PseudorandMinMax(offsetFactor0, terrainZScale * 0.5f - offsetFactor0);
		const glm::vec3 pos0 = glm::vec3(
			xPos0,
			terrainYScale * myTerrain->GetHeightAtPt(xPos0 / terrainXScale, zPos0 / terrainZScale, false),
			zPos0
		);

		const float offsetFactor1 = 55.0f;
		const float xPos1 = PseudorandMinMax(-terrainXScale * 0.5f + offsetFactor1, terrainXScale * 0.5f - offsetFactor1);
		const float zPos1 = PseudorandMinMax(offsetFactor1, terrainZScale * 0.5f - offsetFactor1);
		const glm::vec3 pos1 = glm::vec3(
			xPos1,
			terrainYScale * myTerrain->GetHeightAtPt(xPos1 / terrainXScale, zPos1 / terrainZScale, false),
			zPos1
		);

		const float offsetFactor2 = 80.0f;
		const float xPos2 = PseudorandMinMax(-terrainXScale * 0.5f + offsetFactor2, terrainXScale * 0.5f - offsetFactor2);
		const float zPos2 = PseudorandMinMax(offsetFactor2, terrainZScale * 0.5f - offsetFactor2);
		const glm::vec3 pos2 = glm::vec3(
			xPos2,
			terrainYScale * myTerrain->GetHeightAtPt(xPos2 / terrainXScale, zPos2 / terrainZScale, false),
			zPos2
		);

		const glm::vec3 color = glm::vec3(PseudorandMinMax(0.1f, 1.0f), PseudorandMinMax(0.1f, 1.0f), PseudorandMinMax(0.1f, 1.0f));

		modelStack.PushModel({
			modelStack.Translate(pos0),
			modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, PseudorandMinMax(0.0f, 360.0f))),
			modelStack.Scale(glm::vec3(140.0f))
		});
			flower->AddModelMatForAll(modelStack.GetTopModel());
			flower->AddColorForAll(color);
			flower->AddDiffuseTexIndexForAll(-1);
		modelStack.PopModel();

		modelStack.PushModel({
			modelStack.Translate(pos1),
			modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, PseudorandMinMax(0.0f, 360.0f))),
			modelStack.Scale(glm::vec3(140.0f))
		});
			grass->AddModelMatForAll(modelStack.GetTopModel());
			grass->AddColorForAll(color);
			grass->AddDiffuseTexIndexForAll(-1);
		modelStack.PopModel();

		modelStack.PushModel({
			modelStack.Translate(pos2),
			modelStack.Rotate(glm::vec4(0.f, 1.f, 0.f, PseudorandMinMax(0.0f, 360.0f))),
			modelStack.Scale(glm::vec3((float)PseudorandMinMax(60, 90)))
		});
			rock->AddModelMatForAll(modelStack.GetTopModel());
			rock->AddColorForAll(color);
			rock->AddDiffuseTexIndexForAll(-1);
		modelStack.PopModel();
	}
}

void GameScene::TuneCoins(cstr const fPath){
	static FILETIME lastWriteTime = {};
	const HANDLE handle = FindFirstFile((CStringW)(CString)fPath, dataCoinAudio3D);

	if(dataCoinAudio3D->ftLastWriteTime.dwLowDateTime != lastWriteTime.dwLowDateTime){
		std::cout << "Coins tuned.\n\n";

		const std::vector<Entity*>& entities = entityPool->GetActiveObjs();
		for(Entity* const entity: entities){
			if(entity && entity->type == Entity::EntityType::Coin){
				entity->audio3D->setIsPaused(luaManager->Read<bool>(fPath, "isPaused", true));

				ISoundEffectControl* const soundFX = entity->audio3D->getSoundEffectControl();
				soundFX->disableAllEffects();

				if(luaManager->Read<bool>(fPath, "shldEnableDistortionSoundEffect", true)){
					(void)soundFX->enableDistortionSoundEffect();
				}
				if(luaManager->Read<bool>(fPath, "shldEnableEchoSoundEffect", true)){
					(void)soundFX->enableEchoSoundEffect();
				}
				if(luaManager->Read<bool>(fPath, "shldEnableWavesReverbSoundEffect", true)){
					(void)soundFX->enableWavesReverbSoundEffect();
				}
			}
		}

		lastWriteTime.dwLowDateTime = dataCoinAudio3D->ftLastWriteTime.dwLowDateTime;
	}
}