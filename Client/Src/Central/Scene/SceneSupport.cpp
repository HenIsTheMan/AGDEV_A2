#include "SceneSupport.h"

extern float dt;
extern int windowWidth;
extern int windowHeight;

#include <atlstr.h>

Cam SceneSupport::cam(glm::vec3(0.0f, 0.f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.f, 1.f, 0.0f), 0.0f, 1400.0f);
ISoundEngine* SceneSupport::soundEngine(nullptr);
TextChief SceneSupport::textChief;

glm::mat4 SceneSupport::view(glm::mat4(1.0f));
glm::mat4 SceneSupport::projection(glm::mat4(1.0f));

float SceneSupport::elapsedTime(0.0f);

ModelStack SceneSupport::modelStack;

int SceneSupport::polyModes[2];

SceneSupport::~SceneSupport(){
	if(soundEngine != nullptr){
		soundEngine->drop();
		soundEngine = nullptr;
	}

	if(luaManager != nullptr){
		luaManager->Destroy();
		luaManager = nullptr;
	}

	if(dataAudio != nullptr){
		delete dataAudio;
		dataAudio = nullptr;
	}
}

void SceneSupport::Enter(){
}

void SceneSupport::Exit(){
}

void SceneSupport::EarlyInit(){
	static bool is1st = true;

	if(is1st){
		glGetIntegerv(GL_POLYGON_MODE, polyModes);

		soundEngine = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_DEFAULT_OPTIONS & ~ESEO_PRINT_DEBUG_INFO_TO_DEBUGGER & ~ESEO_PRINT_DEBUG_INFO_TO_STDOUT);
		if(!soundEngine){
			(void)puts("Failed to init soundEngine!\n");
		}
		EntityFactory::GetObjPtr()->soundEngine = soundEngine;

		is1st = false;
	}
}

void SceneSupport::Init(){
	BGM = soundEngine->play2D("Audio/Music/Theme.mp3", true, true);
	BGM->setVolume(luaManager->Read<float>("Scripts/Audio.lua", "BGMVol", true));
}

void SceneSupport::FixedUpdate(){
}

void SceneSupport::Update(){
	elapsedTime += dt;
	if(windowHeight){ //Avoid division by 0 when win is minimised
		cam.SetDefaultAspectRatio(float(windowWidth) / float(windowHeight));
		cam.ResetAspectRatio();
	}

	static float polyModeBT = 0.f;
	if(Key(VK_F2) && polyModeBT <= elapsedTime){
		polyModes[0] += polyModes[0] == GL_FILL ? -2 : 1;
		glPolygonMode(GL_FRONT_AND_BACK, polyModes[0]);
		polyModeBT = elapsedTime + .5f;
	}

	TuneAudio("Scripts/Audio.lua");

	const glm::vec3& camPos = cam.GetPos();
	const glm::vec3& camFront = cam.CalcFront();
	soundEngine->setListenerPosition(vec3df(camPos.x, camPos.y, camPos.z), vec3df(camFront.x, camFront.y, camFront.z));
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
	IScene(),
	forwardSP{"Shaders/Forward.vertexS", "Shaders/Forward.fragS"},
	viewingFrustumSP{"Shaders/ViewingFrustum.vertexS", "Shaders/ViewingFrustum.fragS"},
	textSP{"Shaders/Text.vertexS", "Shaders/Text.fragS"},
	luaManager(LuaManager::GetObjPtr()),
	dataAudio(new WIN32_FIND_DATA()),
	BGM(nullptr)
{
}

void SceneSupport::TuneAudio(cstr const fPath){
	static FILETIME lastWriteTime = {};
	const HANDLE handle = FindFirstFile((CStringW)(CString)fPath, dataAudio);

	if(dataAudio->ftLastWriteTime.dwLowDateTime != lastWriteTime.dwLowDateTime){
		std::cout << "Audio Settings tuned.\n\n";

		BGM->setIsPaused(luaManager->Read<bool>(fPath, "isPausedBGM", true));
		soundEngine->setSoundVolume(luaManager->Read<float>(fPath, "soundVol", true));
		soundEngine->setRolloffFactor(luaManager->Read<float>(fPath, "rolloffFactor", true));
		soundEngine->setDopplerEffectParameters(
			luaManager->Read<float>(fPath, "dopplerFactorDopplerEffect", true),
			luaManager->Read<float>(fPath, "distFactorDopplerEffect", true)
		);

		lastWriteTime.dwLowDateTime = dataAudio->ftLastWriteTime.dwLowDateTime;
	}
}