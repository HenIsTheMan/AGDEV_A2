#pragma once

#include "../SceneSupport.h"

#ifndef BIT
	#define BIT(x) 1 << x
#endif

class GameScene final: public SceneSupport{
public:
	GameScene();
	~GameScene();

	void Enter() override;
	void Exit() override;

	void EarlyInit() override;
	void Init() override;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;

	void PreRender() override;
	void Render() override;
	void PostRender() override;
private:
	LOD treeLOD;

	enum struct ModelType{
		Shotgun = 0,
		Scar,
		Sniper,
		Flower,
		Grass,
		Rock,
		Tree_Low,
		Tree_Medium,
		Tree_High,
		Dragon_Low,
		Dragon_Medium,
		Dragon_High,
		Amt
	};
	Model* models[(int)ModelType::Amt];

	uint cubemapRefID;

	enum struct ItemType: int{
		None = -1,
		Shotgun,
		Scar,
		Sniper,
		ShotgunAmmo,
		ScarAmmo,
		SniperAmmo,
		Amt
	};
	int currSlot;
	ItemType inv[5];

	Gun* currGun;
	Gun* guns[3];
	glm::vec4 reticleColour;

	void InitEntities();

	void CreateEntities();
	void CreateTreesAndCubes();
	void CreateDecorations();

	EntityManager* entityManager;

	NodeManager* nodeManager;
	RegionManager* regionManager;

	bool shldUpdateEntityManager;
	bool shldRenderEntityManager;

	bool isCamDetached;
	bool shldRenderViewingFrustum;

	glm::vec3 frustumColor;

	Light* dLightFromTop;
	Light* dLightFromBottom;

	const Entity* myPlayer;
};