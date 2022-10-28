#ifndef BOX_SPAWNER_H
#define BOX_SPAWNER_H

#include "ScriptableEntity.h"

class BoxSpawner : public ScriptableEntity {

public:
	glm::ivec2 size = { 20, 20 };
	int count = 100;
	float viewDistance = 20;

	std::vector<Entity> entities_pool;
	std::queue<size_t> available_index;
	std::vector<bool> indexes;

	std::set<std::pair<int, int>> box_pos;

	void OnCreate() {
		for (int i = 0; i < count; i++) {
			Entity entity = Instantiate();
			entity.AddComponent<CollisionComponent>().DrawBox(false);
			auto& sprite = entity.AddComponent<SpriteRendererComponent>();
			sprite.SetTexture(Texture::library["box"]);
			auto& tag = entity.GetComponent<TagComponent>();
			tag.active = false;
			tag.name = "Box " + std::to_string(i + 1);
			tag.tag = "Wall";
			entities_pool.push_back(entity);
			available_index.push(i);
			indexes.push_back(true);
		}
	}

	void OnUpdate(Time time) {
		auto& transform = GetComponent<TransformComponent>();
		glm::vec2 floor_pos = glm::floor(transform.position);

		std::vector<float> noise((size_t)size.x * (size_t)size.y);
		Noise::GetNoise2D(noise.data(), size, floor_pos);

		for (int i = 0; i < count; i++) {
			if (indexes[i]) continue;
			glm::vec3 pos = entities_pool[i].GetComponent<TransformComponent>().position;
			bool& active = entities_pool[i].GetComponent<TagComponent>().active;
			if (active && glm::distance(pos, transform.position) > viewDistance) {
				active = false;
				box_pos.erase({ (int)pos.x, (int)pos.y });
				indexes[i] = true;
				available_index.push(i);
			}
		}

		for (int j = 0; j < size.y; j++) {
			for (int i = 0; i < size.x; i++) {
				if (available_index.empty()) continue;

				size_t avail = available_index.front();
				bool& cur_act = entities_pool[avail].GetComponent<TagComponent>().active;

				if (!cur_act && glm::abs(noise[(size_t)(i + j * (size_t)size.y)]) > 0.62f) {
					auto& ent_tr = entities_pool[avail].GetComponent<TransformComponent>();

					glm::ivec3 pos = glm::ivec3{ floor_pos + glm::vec2{ i - size.x / 2, j - size.y / 2 }, 0 };
					if (pos == glm::ivec3{ floor_pos, 0 }) continue;
					ent_tr.position = pos;

					if (glm::distance(ent_tr.position, transform.position) > viewDistance) continue;
					if (box_pos.count({pos.x, pos.y})) continue;

					box_pos.insert({ pos.x, pos.y });
					available_index.pop();
					indexes[avail] = false;
					cur_act = true;
				}
			}
		}
	}
};

#endif