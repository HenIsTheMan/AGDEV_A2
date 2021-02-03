#include "StateEnemyIdle.h"

#include "../../Collision/Collider/Colliders/BoxCollider.h"

float StateEnemyIdle::elapsedTime = 0.0f;

void StateEnemyIdle::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.7f, 0.7f, 0.7f, 0.4f);
}

void StateEnemyIdle::Update(Entity* const entity, const double dt){
	entity->node->SetLocalDilation(glm::vec3(200.0f + sinf(elapsedTime * 4.0f) * 40.0f, 200.0f + sinf(elapsedTime * 4.0f) * 40.0f, entity->node->GetLocalDilation().z));
}

void StateEnemyIdle::Exit(Entity* const entity){
	entity->colour = glm::vec4(0.3f, 0.3f, 0.3f, 0.4f);
}