#include "StateEnemyPatrol.h"

#include "../../Lua/LuaManager.h"

extern float terrainXScale;
extern float terrainZScale;

float StateEnemyPatrol::distSquaredThreshold = 0.0f;
Entity* StateEnemyPatrol::targetEntity = nullptr;
WayptManager* const StateEnemyPatrol::wayptManager = WayptManager::GetObjPtr();

void StateEnemyPatrol::Enter(Entity* const entity){
	distSquaredThreshold = (terrainXScale + terrainZScale) * 0.5f * 0.2f; //Accts for non-uni scaling of terrain
	distSquaredThreshold *= distSquaredThreshold;
	entity->colour = glm::vec4(0.3f, 0.3f, 0.3f, 0.4f);
	entity->currWaypt = wayptManager->RetrieveRandWaypt();
}

void StateEnemyPatrol::Update(Entity* const entity, const double dt){
	const glm::vec3 localTranslation = entity->node->GetLocalTranslation();

	if(targetEntity != nullptr){ //Precaution
		glm::vec3 vecCheck = targetEntity->node->GetLocalTranslation() - localTranslation;
		vecCheck.y = 0.0f;
		if(LuaManager::GetObjPtr()->CallLuaFunc<float>("Scripts/LenSquared.lua", "LenSquared", {vecCheck.x, vecCheck.y, vecCheck.z}, true) <= distSquaredThreshold){
			entity->nextState = entity->stateMachine->AcquireState(StateID::EnemyChase);
			return;
		}
	}

	glm::vec3 vec = entity->currWaypt->pos - localTranslation;
	vec.y = 0.0f;

	if(LuaManager::GetObjPtr()->CallLuaFunc<float>("Scripts/LenSquared.lua", "LenSquared", {vec.x, vec.y, vec.z}, true) < entity->moveSpd * (float)dt * entity->moveSpd * (float)dt){
		entity->node->SetLocalTranslation(glm::vec3(
			roundf(localTranslation.x),
			roundf(localTranslation.y),
			roundf(localTranslation.z)
		));

		if(PseudorandMinMax(1, 100) <= 10){
			entity->nextState = entity->stateMachine->AcquireState(StateID::EnemyIdle);
		} else{
			entity->currWaypt = entity->currWaypt->nextWaypt;
		}
	} else{
		entity->node->LocalTranslate(glm::normalize(vec) * entity->moveSpd * (float)dt);
	}
}

void StateEnemyPatrol::Exit(Entity* const entity){
	entity->currWaypt = nullptr;
}