void StateEnemyIdle::Enter(Entity* const entity){
	entity->colour = glm::vec4(0.7f, 0.7f, 0.7f, 0.4f);
}

void StateEnemyIdle::Update(Entity* const entity, const double dt){
}

void StateEnemyIdle::Exit(Entity* const entity){
	entity->colour = glm::vec4(0.3f, 0.3f, 0.3f, 0.4f);
}