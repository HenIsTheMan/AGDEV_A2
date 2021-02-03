#include "StateEnemyPatrol.h"

#include <glm/gtx/norm.hpp>

WayptManager* const StateEnemyPatrol::wayptManager = WayptManager::GetObjPtr();

void StateEnemyPatrol::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.3f, 0.3f, 0.3f, 0.4f);
}

void StateEnemyPatrol::Update(Entity* const entity, const double dt){
	if(entity->currWaypt == nullptr){
		entity->currWaypt = wayptManager->RetrieveRandWaypt();
	} else{
		const glm::vec3 localTranslation = entity->node->GetLocalTranslation();
		glm::vec3 vec = entity->currWaypt->pos - localTranslation;
		vec.y = 0.0f;

		if(glm::length2(vec) < entity->moveSpd * (float)dt * entity->moveSpd * (float)dt){ //??
			entity->node->SetLocalTranslation(glm::vec3(
				roundf(localTranslation.x),
				roundf(localTranslation.y),
				roundf(localTranslation.z)
			));

			entity->currWaypt = entity->currWaypt->nextWaypt;
		} else{
			entity->node->LocalTranslate(glm::normalize(vec) * entity->moveSpd * (float)dt);
		}
	}
}

void StateEnemyPatrol::Exit(Entity* const entity){
}