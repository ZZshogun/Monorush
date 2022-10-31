#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "ScriptableEntity.h"
#include "GameManager.h"
#include "EnemyController.h"
#include "PlayerController.h"

class EnemySpawner : public ScriptableEntity {
	const float spawnInterval = 1.22f;
	const float spawnFuryInterval = 0.45f;
public:
	float elapsed = 0;
	float max_elapsed = spawnInterval;
	int count = 0;
	float spawnRange = 12.0f;

	std::mt19937 rng;
	std::uniform_real_distribution<float> randFloat;

	void OnCreate() {
		std::random_device dev;
		rng = std::mt19937(dev());
		rng.seed((unsigned)time(NULL));
		randFloat = std::uniform_real_distribution<float>(-1, 1);
	}

	void OnUpdate(Time time) {
		if (GameManager::gameOver) return;
		auto& transform = GetComponent<TransformComponent>();

		if (GameManager::fury) {
			max_elapsed = spawnFuryInterval;
		}
		else max_elapsed = spawnInterval;

		elapsed += time.deltaTime;
		if (elapsed >= max_elapsed) {
			elapsed = 0;
			float x_dir = randFloat(rng);
			float y_dir = randFloat(rng);
			float extra = glm::abs(randFloat(rng) * 5);

			if (x_dir == 0) x_dir += 0.01f;
			if (y_dir == 0) y_dir += 0.01f;
			glm::vec3 rand_pos = (spawnRange + extra) * glm::normalize(glm::vec3{ x_dir, y_dir, 0 }) + transform.position;

			Entity enemy = Instantiate();
			enemy.AddComponent<SpriteRendererComponent>().order = 1;
			auto& spriteSheet = enemy.AddComponent<SpriteSheetComponent>();
			spriteSheet.SetSpriteSheet(Texture::library["enemy"]);
			spriteSheet.Size({2, 1});
			spriteSheet.SizePerSprite(1);
			enemy.AddComponent<RigidbodyComponent>().mass = 10;
			auto& enemy_col = enemy.AddComponent<CollisionComponent>();
			enemy_col.Size({0.5f, 0.5f});
			enemy.GetComponent<TransformComponent>().position = rand_pos;
			auto& tag = enemy.GetComponent<TagComponent>();
			tag.name = "Enemy " + std::to_string(++count);
			tag.tag = "Enemy";
			enemy.AddComponent<NativeScriptComponent>().Bind<EnemyController>();
		}
	}
};

#endif