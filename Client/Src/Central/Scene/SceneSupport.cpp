#include "SceneSupport.h"

extern float dt;
extern int windowWidth;
extern int windowHeight;

Cam SceneSupport::cam(glm::vec3(0.0f, 0.f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.f, 1.f, 0.0f), 0.0f, 1400.0f);
ISoundEngine* SceneSupport::soundEngine(nullptr);
TextChief SceneSupport::textChief;

glm::mat4 SceneSupport::view(glm::mat4(1.0f));
glm::mat4 SceneSupport::projection(glm::mat4(1.0f));

float SceneSupport::elapsedTime(0.0f);

ModelStack SceneSupport::modelStack;

int SceneSupport::polyModes[2];

float SceneSupport::textScaleFactors[]{
	1.f,
	1.f,
};

glm::vec4 SceneSupport::textColours[]{
	glm::vec4(1.f),
	glm::vec4(1.f),
};

SceneSupport::~SceneSupport(){
	if(soundEngine != nullptr){
		soundEngine->drop();
		soundEngine = nullptr;
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

		is1st = false;
	}
}

void SceneSupport::Init(){
	static bool is1st = true;

	if(is1st){
		soundEngine->play2D("Audio/Music/Theme.mp3", true);

		is1st = false;
	}
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
	textSP{"Shaders/Text.vertexS", "Shaders/Text.fragS"}
{
}