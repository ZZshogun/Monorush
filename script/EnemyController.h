#ifndef ENEMY_H
#define ENEMY_H

#include "../header/ScriptableEntity.h"
#include "GameManager.h"

class EnemyController : public ScriptableEntity {

public:

	int healthPoint = 10;
	float speed = 2;
	float turnSpeed = 2;
	bool run_once = false;
	bool death = false;
	bool hurt = false;

	float disposeWaitTime = 0.4f, _counter = 0;
	float hurtWaitTime = 0.25f, _hcounter = 0;

	void Hurt(int damage) {
		healthPoint -= damage;
		healthPoint = glm::max(0, healthPoint);

		hurt = true;
		_hcounter = 0;
		GetComponent<SpriteSheetComponent>().DrawAtIndex(1);

		auto& tag = GetComponent<TagComponent>();
		std::cout << tag.name << " : health " << healthPoint << ", " << damage << " damage\n";
	}

	void OnCreate() {
		healthPoint += (int)(GameManager::difficulty * 5);
		speed += (int)(GameManager::difficulty * 0.5f);
		turnSpeed += (int)(glm::pow(GameManager::difficulty, 2) * 3);

		turnSpeed = glm::min<float>(turnSpeed, 10);
		speed = glm::min<float>(speed, 4.25f);
		healthPoint = glm::min<int>(healthPoint, 45);

		auto& tag = GetComponent<TagComponent>();
		//std::cout << tag.name << " : health " << healthPoint << ", speed " << speed << ", turnSpeed " << turnSpeed << "\n";
	}

	void OnUpdate(Time time) {

		auto& playerTransform = FindEntityOfName("Player").GetComponent<TransformComponent>();
		auto& rigidBody = GetComponent<RigidbodyComponent>();
		auto& spritesheet = GetComponent<SpriteSheetComponent>();

		glm::vec3 direction = playerTransform.position - GetComponent<TransformComponent>().position;
		if (GameManager::gameOver) {
			if (!run_once) {
				run_once = true;
				GetComponent<CollisionComponent>().active = false;
			}
			direction *= -1;
			speed += time.deltaTime;
		}

		if (healthPoint <= 0) {
			if (!death) {
				death = true;
				if (GameManager::fury) GameManager::score += 100;
				else GameManager::score += 50;
				spritesheet.DrawAtIndex(2);
				GetComponent<SpriteRendererComponent>().Color({ 1, 1, 1, 0.75f });
				GetComponent<CollisionComponent>().active = false;
			}
			if (_counter >= disposeWaitTime) {
				entity.Destroy();
			}
			else _counter += time.deltaTime;
			return;
		}

		if (hurt) {
			_hcounter += time.deltaTime;
			if (_hcounter >= hurtWaitTime) {
				hurt = false;
				spritesheet.DrawAtIndex(0);
			}
		}

		glm::vec3 curDir = glm::length(rigidBody.velocity) < 1 ? 
			rigidBody.velocity : glm::normalize(rigidBody.velocity);
		glm::vec3 dirNormalize = glm::normalize(direction);
		direction = glm::lerp(curDir, dirNormalize, turnSpeed * 0.01f);

		rigidBody.velocity = direction * (float)speed;
	}
};

#endif