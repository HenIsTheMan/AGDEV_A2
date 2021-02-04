#include "StateEnemyHeal.h"

float StateEnemyHeal::elapsedTime = 0.0f;
float StateEnemyHeal::healDelay = 2.0f;
float StateEnemyHeal::healMaxDelay = 2.0f;
float StateEnemyHeal::healMultiplier = 0.05f;

void StateEnemyHeal::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.0f, 0.3f, 0.0f, 0.4f);
}

void StateEnemyHeal::Update(Entity* const entity, const double dt){
	entity->node->SetLocalDilation(glm::vec3(200.0f + cosf(elapsedTime * 2.0f) * 40.0f, 200.0f + cosf(elapsedTime * 2.0f) * 40.0f, entity->node->GetLocalDilation().z));

	if(healDelay <= 0.0f){
		entity->life += entity->maxLife * healMultiplier;

		if(entity->life + entity->maxLife * healMultiplier > entity->maxLife){
			entity->nextState = entity->stateMachine->AcquireState(StateID::EnemyIdle);
			return;
		}

		healDelay = healMaxDelay;
	} else{
		healDelay -= (float)dt;
	}
}

void StateEnemyHeal::Exit(Entity* const entity){
}