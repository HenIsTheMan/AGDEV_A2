#pragma once

#include "../SceneSupport.h"

class SceneManager;

class SettingsScene final: public SceneSupport{
public:
	SettingsScene();
	~SettingsScene() = default;

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
	enum struct SettingsType: int{
		IsPausedBGM,
		SoundVol,
		RolloffFactor,
		DopplerFactorDopplerEffect,
		DistFactor,
		CoinVol,
		ScopeVol,
		PopVol,
		SelectVol,
		BGMVol,
		ReloadVol,
		ShotgunVol,
		ScarVol,
		SniperVol,
		Amt
	};
	const str txts[(int)SettingsType::Amt]{
		"BGM Is Paused",
		"Global Vol",
		"Rolloff Factor",
		"Doppler Factor",
		"Dist Factor",
		"Coin Vol",
		"Scope Vol",
		"Pop Vol",
		"Select Vol",
		"BGM Vol",
		"Reload Vol",
		"Shotgun Vol",
		"Scar Vol",
		"Sniper Vol",
	};
	cstr const keyNames[(int)SettingsType::Amt]{
		"isPausedBGM",
		"soundVol",
		"rolloffFactor",
		"dopplerFactorDopplerEffect",
		"distFactorDopplerEffect",
		"coinVol",
		"scopeVol",
		"popVol",
		"selectVol",
		"BGMVol",
		"reloadVol",
		"shotgunVol",
		"scarVol",
		"sniperVol",
	};

	SettingsType currSettingsType;

	float backScaleFactor;
	glm::vec4 backColor;
};