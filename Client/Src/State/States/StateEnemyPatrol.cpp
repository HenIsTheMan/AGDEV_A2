#include "StateEnemyPatrol.h"

#include "../../Lua/LuaManager.h"

WayptManager* const StateEnemyPatrol::wayptManager = WayptManager::GetObjPtr();

void StateEnemyPatrol::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.3f, 0.3f, 0.3f, 0.4f);
	entity->currWaypt = wayptManager->RetrieveRandWaypt();
}

void StateEnemyPatrol::Update(Entity* const entity, const double dt){
	const glm::vec3 localTranslation = entity->node->GetLocalTranslation();
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