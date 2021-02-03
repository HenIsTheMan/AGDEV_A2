#include "StateEnemyIdle.h"

#include "Engine.h"

#include "../../Collision/Collider/Colliders/BoxCollider.h"

float StateEnemyIdle::elapsedTime = 0.0f;

void StateEnemyIdle::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.7f, 0.7f, 0.7f, 0.4f);
	entity->idleTime = PseudorandMinMax(2.5f, 4.0f);
}

void StateEnemyIdle::Update(Entity* const entity, const double dt){
	entity->idleTime -= (float)dt;
	if(entity->idleTime <= 0.0f){
		entity->nextState = entity->stateMachine->AcquireState(StateID::EnemyPatrol);
		return;
	}

	entity->node->SetLocalDilation(glm::vec3(200.0f + sinf(elapsedTime * 4.0f) * 40.0f, 200.0f + sinf(elapsedTime * 4.0f) * 40.0f, entity->node->GetLocalDilation().z));
}

void StateEnemyIdle::Exit(Entity* const entity){
}