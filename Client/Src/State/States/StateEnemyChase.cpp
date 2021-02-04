#include "StateEnemyChase.h"

#include "../../Lua/LuaManager.h"

extern float terrainXScale;
extern float terrainZScale;

float StateEnemyChase::distSquaredThreshold = 0.0f;
Entity* StateEnemyChase::targetEntity = nullptr;

void StateEnemyChase::Enter(Entity* const entity){
	distSquaredThreshold = (terrainXScale + terrainZScale) * 0.5f * 0.2f; //Accts for non-uni scaling of terrain
	distSquaredThreshold *= distSquaredThreshold;
	entity->colour = glm::vec4(0.3f, 0.0f, 0.0f, 0.4f);
}

void StateEnemyChase::Update(Entity* const entity, const double dt){
	const glm::vec3 localTranslation = entity->node->GetLocalTranslation();

	if(targetEntity != nullptr){ //Precaution
		glm::vec3 vecCheck = targetEntity->node->GetLocalTranslation() - localTranslation;
		vecCheck.y = 0.0f;
		if(LuaManager::GetObjPtr()->CallLuaFunc<float>("Scripts/LenSquared.lua", "LenSquared", {vecCheck.x, vecCheck.y, vecCheck.z}, true) > distSquaredThreshold){
			entity->nextState = entity->stateMachine->AcquireState(StateID::EnemyPatrol);
			return;
		}
	}
}

void StateEnemyChase::Exit(Entity* const entity){
}