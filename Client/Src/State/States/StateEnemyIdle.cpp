#include "StateEnemyIdle.h"

float StateEnemyIdle::elapsedTime = 0.0f;

void StateEnemyIdle::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.7f, 0.7f, 0.7f, 0.4f);
}

void StateEnemyIdle::Update(Entity* const entity, const double dt){
	entity->scale.x = entity->scale.y = 200.0f + sinf(elapsedTime * 4.0f) * 100.0f;
}

void StateEnemyIdle::Exit(Entity* const entity){
	entity->colour = glm::vec4(0.3f, 0.3f, 0.3f, 0.4f);
}