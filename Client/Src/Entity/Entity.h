#pragma once
#include <Core.h>

#include "../Collision/Collider/Collider.h"

#include "../State/State.h"

#include "../Quadtree/Node.h"

#include "../Waypt/Waypt.h"

enum struct StateID: int;

#include "../State/StateMachine.h"

struct Entity final{
	template <class T>
	class ObjPool;
public:
	void Reset();

	///Getters
	const glm::vec3& GetPos() const;
	const glm::vec3& GetScale() const;
	const glm::vec3& GetFacingDir() const;

	///Setter
	void SetFacingDir(const glm::vec3& facingDir);
	void SetPos(const glm::vec3& pos);
	void SetScale(const glm::vec3& scale);

	enum struct EntityType: int{
		Bullet,
		Coin,
		Fire,
		Player,
		ThinObj,
		EnemyBody,
		EnemyPart,
		Tree,
		Amt
	};

	Entity();

	EntityType type;
	bool movable;
	float life;
	float maxLife;
	glm::vec4 colour;
	int diffuseTexIndex;
	glm::vec3 scale;

	glm::vec3 pos;
	glm::vec3 vel;
	float mass;
	glm::vec3 force;

	Collider* collider;
	glm::vec3 facingDir;
	float moveSpd;

	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;

	glm::vec3 prevPos;

	StateMachine<StateID, Entity>* stateMachine;
	State* currState;
	State* nextState;

	ISound* audio3D;

	Node* node;

	//* Vars for specific entities (help la, I wanna do ECS alr)
	float idleTime;
	Waypt* currWaypt;
	//*/
};