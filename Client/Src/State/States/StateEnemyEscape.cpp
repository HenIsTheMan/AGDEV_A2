#include "StateEnemyEscape.h"

#include "../../Lua/LuaManager.h"

extern float terrainXScale;
extern float terrainZScale;

glm::vec3 StateEnemyEscape::terrainPos = glm::vec3();
float StateEnemyEscape::distSquaredThreshold = 0.0f;
Entity* StateEnemyEscape::targetEntity = nullptr;

void StateEnemyEscape::Enter(Entity* const entity){
	distSquaredThreshold = (terrainXScale + terrainZScale) * 0.5f * 0.5f; //Accts for non-uni scaling of terrain
	distSquaredThreshold *= distSquaredThreshold;

	entity->colour = glm::vec4(0.3f, 0.3f, 0.0f, 0.4f);
}

void StateEnemyEscape::Update(Entity* const entity, const double dt){
	const glm::vec3 localTranslation = entity->node->GetLocalTranslation();

	if(targetEntity != nullptr){ //Precaution
		glm::vec3 vec = localTranslation - targetEntity->node->GetLocalTranslation();
		vec.y = 0.0f;

		if(LuaManager::GetObjPtr()->CallLuaFunc<float>("Scripts/LenSquared.lua", "LenSquared", {vec.x, vec.y, vec.z}, true) > distSquaredThreshold){
			entity->nextState = entity->stateMachine->AcquireState(StateID::EnemyHeal);
			return;
		} else{
			entity->node->LocalTranslate(glm::normalize(vec) * entity->chaseSpd * (float)dt);
		}

		glm::vec3 localTranslationNew = entity->node->GetLocalTranslation();
		localTranslationNew.x = glm::clamp(localTranslationNew.x, terrainPos.x - terrainXScale * 0.45f, terrainPos.x + terrainXScale * 0.45f);
		localTranslationNew.z = glm::clamp(localTranslationNew.z, terrainPos.z - terrainZScale * 0.45f, terrainPos.y + terrainZScale * 0.45f);
		entity->node->SetLocalTranslation(localTranslationNew);
	}
}

void StateEnemyEscape::Exit(Entity* const entity){
}