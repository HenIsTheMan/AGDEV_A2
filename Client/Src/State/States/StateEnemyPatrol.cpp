#include "StateEnemyPatrol.h"

WayptManager* const StateEnemyPatrol::wayptManager = WayptManager::GetObjPtr();

void StateEnemyPatrol::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.3f, 0.3f, 0.3f, 0.4f);
}

void StateEnemyPatrol::Update(Entity* const entity, const double dt){
}

void StateEnemyPatrol::Exit(Entity* const entity){
}