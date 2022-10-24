#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "ScriptableEntity.h"
#include "EnemyController.h"

class EnemySpawner : public ScriptableEntity {
public:

	float elapsed = 0;
	float max_elapsed = 1;
	int count = 0;

	PlayerController* playerController = NULL;

	void OnUpdate(Time time) {
		elapsed += time.deltaTime;
		if (elapsed >= max_elapsed) {
			elapsed = 0;

			float rx = Math::Random(-5, 5);
			float ry = Math::Random(-5, 5);
			glm::vec2 rand_pos = { rx, ry };

			Entity enemy = Instantiate();
			enemy.AddComponent<SpriteRendererComponent>().order = 2;
			auto& spriteSheet = enemy.AddComponent<SpriteSheetComponent>();
			spriteSheet.sheet = Texture::library["enemy"];
			spriteSheet.size = { 2, 1 };
			spriteSheet.sizePerSprite = 1;
			enemy.AddComponent<RigidbodyComponent>();
			auto& enemy_col = enemy.AddComponent<CollisionComponent>();
			enemy_col.drawBox = true;
			enemy_col.size = {0.5f, 0.5f};
			enemy.GetComponent<TransformComponent>().position = glm::vec3(rand_pos, 0);
			auto& name = enemy.GetComponent<TagComponent>().tag;
			name = "Enemy " + std::to_string(++count);
			enemy.AddComponent<NativeScriptComponent>().Bind<EnemyController>();
			std::cout << name << " spawned at (" << rand_pos.x << ", " << rand_pos.y << ")\n";
		}
	}
};

#endif