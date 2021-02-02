#include "Sniper.h"

#include "../../Lua/LuaManager.h"

Sniper::Sniper(){
	reloadTime = 3.f;
	shotCooldownTime = 1.0f;
	loadedBullets = 3;
	maxLoadedBullets = 3;
	unloadedBullets = 400;
	maxUnloadedBullets = 400;
}

void Sniper::Shoot(const float& elapsedTime, const glm::vec3& camPos, const glm::vec3& camFront, ISoundEngine* const& soundEngine){
	static LuaManager* luaManager = LuaManager::GetObjPtr();
	static float bulletBT = 0.f;

	if(canShoot && loadedBullets && bulletBT <= elapsedTime){
		ISound* const sound = soundEngine->play2D("Audio/Sounds/Sniper.wav", false, true);
		sound->setVolume(luaManager->Read<float>("Scripts/Audio.lua", "sniperVol", true));
		sound->setIsPaused(false);

		entityFactory->CreateSniperBullet(camPos, camFront);

		--loadedBullets;
		bulletBT = elapsedTime + shotCooldownTime;
	}
}