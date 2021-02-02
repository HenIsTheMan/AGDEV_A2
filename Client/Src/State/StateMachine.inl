template <class T, typename Type>
StateMachine<T, Type>::StateMachine():
	im_States()
{
}

template <class T, typename Type>
StateMachine<T, Type>::~StateMachine<T, Type>(){
	for(std::pair<T, State*> element: im_States){
		delete element.second;
		element.second = nullptr;
	}
}

template <class T, typename Type>
void StateMachine<T, Type>::AddState(State* const state){
	assert(state && "Var 'state' must be initialized!");

	im_States.insert({state->GetID(), state});
}

template <class T, typename Type>
void StateMachine<T, Type>::CheckForStateTransition(Type* const entity){
	assert(entity && "entity must be initialized!");
	assert(entity->nextState && "entity->nextState must be initialized!");

	if(entity->currState != entity->nextState){
		if(entity->currState){
			entity->currState->Exit(entity);
			entity->currState = entity->nextState;
			entity->currState->Enter(entity);
		} else{
			entity->currState = entity->nextState;
			entity->currState->Enter(entity);
		}
	}
}

template <class T, typename Type>
void StateMachine<T, Type>::UpdateCurrState(Type* const entity, const double dt){
	assert(entity && "entity must be initialized!");
	assert(entity->currState && "entity->currState must be initialized!");

	entity->currState->Update(entity, dt);
}

template <class T, typename Type>
State* StateMachine<T, Type>::AcquireState(const T& stateID) const{
	assert(im_States.find(stateID) != im_States.end() && "stateID cannot be found!");

	return im_States.at(stateID);
}