#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "ScriptableEntity.h"
#include "EnemyController.h"

class EnemySpawner : public ScriptableEntity {
public:

	float elapsed = 0;
	float max_elapsed = 0.25f;
	int count = 0;

	PlayerController* playerController = NULL;
	TransformComponent* transform;

	void OnCreate() {
		transform = &GetComponent<TransformComponent>();
	}

	void OnUpdate(Time time) {
		elapsed += time.deltaTime;
		if (elapsed >= max_elapsed) {
			elapsed = 0;

			float rx = Math::Random(-5, 5);
			float ry = Math::Random(-5, 5);
			glm::vec3 rand_pos = glm::vec3{ rx, ry, 0 } + transform->position;

			Entity enemy = Instantiate();
			enemy.AddComponent<SpriteRendererComponent>().order = 2;
			auto& spriteSheet = enemy.AddComponent<SpriteSheetComponent>();
			spriteSheet.SetSpriteSheet(Texture::library["enemy"]);
			spriteSheet.Size({2, 1});
			spriteSheet.SizePerSprite(1);
			enemy.AddComponent<RigidbodyComponent>().mass = 0.5f;
			auto& enemy_col = enemy.AddComponent<CollisionComponent>();
			enemy_col.DrawBox(true);
			enemy_col.Size({0.5f, 0.5f});
			enemy.GetComponent<TransformComponent>().position = rand_pos;
			auto& name = enemy.GetComponent<TagComponent>().tag;
			name = "Enemy " + std::to_string(++count);
			enemy.AddComponent<NativeScriptComponent>().Bind<EnemyController>();
			std::cout << name << " spawned at (" << rand_pos.x << ", " << rand_pos.y << ")\n";
		}
	}
};

#endif