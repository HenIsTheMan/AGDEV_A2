#include "StateEnemyChase.h"

#include "../../Lua/LuaManager.h"

extern float terrainXScale;
extern float terrainZScale;

float StateEnemyChase::distSquaredThreshold = 0.0f;
float StateEnemyChase::lifeThreshold = 0.0f;
Entity* StateEnemyChase::targetEntity = nullptr;

void StateEnemyChase::Enter(Entity* const entity){
	distSquaredThreshold = (terrainXScale + terrainZScale) * 0.5f * 0.15f; //Accts for non-uni scaling of terrain
	distSquaredThreshold *= distSquaredThreshold;

	lifeThreshold = entity->maxLife * 0.4f;

	entity->colour = glm::vec4(0.3f, 0.0f, 0.0f, 0.4f);
}

void StateEnemyChase::Update(Entity* const entity, const double dt){
	const glm::vec3 localTranslation = entity->node->GetLocalTranslation();

	if(entity->life <= lifeThreshold){
		entity->nextState = entity->stateMachine->AcquireState(StateID::EnemyEscape);
		return;
	}

	if(targetEntity != nullptr){ //Precaution
		glm::vec3 vecCheck = targetEntity->node->GetLocalTranslation() - localTranslation;
		vecCheck.y = 0.0f;
		if(LuaManager::GetObjPtr()->CallLuaFunc<float>("Scripts/LenSquared.lua", "LenSquared", {vecCheck.x, vecCheck.y, vecCheck.z}, true) > distSquaredThreshold){
			entity->nextState = entity->stateMachine->AcquireState(StateID::EnemyPatrol);
			return;
		}

		glm::vec3 vec = targetEntity->node->GetLocalTranslation() - localTranslation;
		vec.y = 0.0f;

		if(LuaManager::GetObjPtr()->CallLuaFunc<float>("Scripts/LenSquared.lua", "LenSquared", {vec.x, vec.y, vec.z}, true) < entity->chaseSpd * (float)dt * entity->chaseSpd * (float)dt){
			entity->node->SetLocalTranslation(glm::vec3(
				roundf(localTranslation.x),
				roundf(localTranslation.y),
				roundf(localTranslation.z)
			));
		} else{
			entity->node->LocalTranslate(glm::normalize(vec) * entity->chaseSpd * (float)dt);
		}
	}
}

void StateEnemyChase::Exit(Entity* const entity){
}