#pragma once

#include "../SceneSupport.h"

class SceneManager;

class MenuScene final: public SceneSupport{
public:
	MenuScene();
	~MenuScene() = default;

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
	enum struct MenuButtons: int{
		Play,
		Shop,
		Scores,
		Settings,
		Credits,
		Quit,
		Amt
	};
	const str menuButtonTexts[(int)MenuButtons::Amt]{
		"Play",
		"Shop",
		"Scores",
		"Settings",
		"Credits",
		"Quit",
	};

	float textScaleFactors[(int)MenuButtons::Amt]{
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
	};
	glm::vec4 textColours[(int)MenuButtons::Amt]{
		glm::vec4(1.0f),
		glm::vec4(1.0f),
		glm::vec4(1.0f),
		glm::vec4(1.0f),
		glm::vec4(1.0f),
		glm::vec4(1.0f),
	};

	float hue;

	float textOffsetX;
	float textOffsetY;
};