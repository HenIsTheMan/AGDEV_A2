#include "StateEnemyChase.h"

#include "../../Lua/LuaManager.h"

extern float terrainXScale;
extern float terrainZScale;

glm::vec3 StateEnemyChase::terrainPos = glm::vec3();
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
		entity->node->LocalTranslate(glm::normalize(vec) * entity->chaseSpd * (float)dt);

		glm::vec3 localTranslationNew = entity->node->GetLocalTranslation();
		localTranslationNew.x = glm::clamp(localTranslationNew.x, terrainPos.x - terrainXScale * 0.45f, terrainPos.x + terrainXScale * 0.45f);
		localTranslationNew.z = glm::clamp(localTranslationNew.z, terrainPos.z - terrainZScale * 0.45f, terrainPos.y + terrainZScale * 0.45f);
		entity->node->SetLocalTranslation(localTranslationNew);
	}
}

void StateEnemyChase::Exit(Entity* const entity){
}