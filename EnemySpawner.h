#ifndef ENEMY_SPAWNER_CLASS_H
#define ENEMY_SPAWNER_CLASS_H

#include "ScriptableEntity.h"

class EnemySpawner : public ScriptableEntity {
public:

	float elapsed = 0;
	float max_elapsed = 3;
	int count = 0;

	void OnUpdate(Time time) {
		elapsed += time.deltaTime;
		if (elapsed >= max_elapsed) {
			elapsed = 0;

			float rx = Math::Random(-5, 5);
			float ry = Math::Random(-5, 5);
			glm::vec2 rand_pos = { rx, ry };

			Entity enemy = Instantiate();
			Ref<Material> mat = Material::Create(Texture::library["texture/chest.png"]);
			enemy.AddComponent<SpriteRendererComponent>().material = mat;
			enemy.GetComponent<TransformComponent>().position = glm::vec3(rand_pos, 0);
			auto& name = enemy.GetComponent<TagComponent>().tag;
			name = "Enemy " + std::to_string(++count);
			std::cout << name << " spawned at (" << rand_pos.x << ", " << rand_pos.y << ")\n";
		}
	}
};

#endif