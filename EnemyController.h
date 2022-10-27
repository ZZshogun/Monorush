#ifndef ENEMY_H
#define ENEMY_H

#include "ScriptableEntity.h"
#include "GameManager.h"

class EnemyController : public ScriptableEntity {

public:

	int healthPoint = 10;
	float speed = 2;
	bool run_once = false;

	TransformComponent* transform = NULL;
	TransformComponent* playerTransform = NULL;
	RigidbodyComponent* rigidBody = NULL;
	CollisionComponent* collider = NULL;

	void OnCreate() {
		playerTransform = &FindEntityOfName("Player").GetComponent<TransformComponent>();
		transform = &GetComponent<TransformComponent>();
		rigidBody = &GetComponent<RigidbodyComponent>();
		collider = &GetComponent<CollisionComponent>();

		healthPoint += (int)(GameManager::difficulty * 6);
		speed += (int)(GameManager::difficulty * 0.3f);

		auto& tag = GetComponent<TagComponent>();
		std::cout << tag.name << " : health " << healthPoint << ", speed " << speed << "\n";
	}

	void OnUpdate(Time time) {
		if (!playerTransform) return;
		glm::vec3 playerPos = playerTransform->position;
		glm::vec3 direction = playerPos - transform->position;
		if (GameManager::gameOver) {
			if (!run_once) {
				run_once = true;
				collider->active = false;
			}
			direction *= -1;
			speed += time.deltaTime;
		}

		rigidBody->velocity = glm::normalize(direction) * (float)speed;
	}
};

#endif