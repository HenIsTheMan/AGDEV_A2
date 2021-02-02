#include "SceneSupport.h"

SceneSupport::~SceneSupport(){
	if(soundEngine != nullptr){
		soundEngine->drop();
		soundEngine = nullptr;
	}
}

void SceneSupport::EarlyInit(){
	glGetIntegerv(GL_POLYGON_MODE, polyModes);

	soundEngine = createIrrKlangDevice(ESOD_AUTO_DETECT, ESEO_DEFAULT_OPTIONS & ~ESEO_PRINT_DEBUG_INFO_TO_DEBUGGER & ~ESEO_PRINT_DEBUG_INFO_TO_STDOUT);
	if(!soundEngine){
		(void)puts("Failed to init soundEngine!\n");
	}
	soundEngine->play2D("Audio/Music/Theme.mp3", true);
	//soundEngine->setSoundVolume(0);
}

void SceneSupport::Init(){
}

void SceneSupport::FixedUpdate(){
}

void SceneSupport::Update(){
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
	cam(glm::vec3(0.0f, 0.f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.f, 1.f, 0.0f), 0.0f, 1400.0f),
	soundEngine(nullptr),
	forwardSP{"Shaders/Forward.vertexS", "Shaders/Forward.fragS"},
	viewingFrustumSP{"Shaders/ViewingFrustum.vertexS", "Shaders/ViewingFrustum.fragS"},
	textSP{"Shaders/Text.vertexS", "Shaders/Text.fragS"},
	view(glm::mat4(1.0f)),
	projection(glm::mat4(1.0f)),
	elapsedTime(0.0f),
	modelStack(),
	polyModes(),
	textScaleFactors{
		1.f,
		1.f,
	},
	textColours{
		glm::vec4(1.f),
		glm::vec4(1.f),
	}
{
}