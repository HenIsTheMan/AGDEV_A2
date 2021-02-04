#include "StateEnemyChase.h"

void StateEnemyChase::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.3f, 0.0f, 0.0f, 0.4f);
}

void StateEnemyChase::Update(Entity* const entity, const double dt){
}

void StateEnemyChase::Exit(Entity* const entity){
}