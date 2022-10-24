#ifndef ENEMY_H
#define ENEMY_H

#include "ScriptableEntity.h"

class EnemyController : public ScriptableEntity {

public:

	int healthPoint = 10;
	float speed = 1.5f;

	Entity player;
	TransformComponent* transform;
	RigidbodyComponent* rigidBody;

	void OnCreate() {
		player = FindEntityOfName("Player");
		transform = &GetComponent<TransformComponent>();
		rigidBody = &GetComponent<RigidbodyComponent>();
	}

	void OnUpdate(Time time) {
		if (!player.IsValid()) return;
		glm::vec3 playerPos = player.GetComponent<TransformComponent>().position;
		glm::vec3 direction = playerPos - transform->position;

		rigidBody->velocity = glm::normalize(direction) * speed;
	}
};

#endif