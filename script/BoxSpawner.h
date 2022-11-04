#ifndef BOX_SPAWNER_H
#define BOX_SPAWNER_H

#include "../header/ScriptableEntity.h"

class BoxSpawner : public ScriptableEntity {

public:
	glm::ivec2 size = { 24, 24 };
	int count = 100;
	float viewDistance = 24;

	std::vector<Entity> entities_pool;
	std::queue<size_t> available_index;
	std::vector<bool> indexes;

	std::set<std::pair<int, int>> box_pos;

	void OnCreate() {
		for (int i = 0; i < count; i++) {
			Entity entity = Instantiate();
			entity.AddComponent<CollisionComponent>().DrawBox(false);
			entity.AddComponent<SpriteRendererComponent>();
			auto& tag = entity.GetComponent<TagComponent>();
			tag.active = false;
			tag.name = "Wall " + std::to_string(i + 1);
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

		for (int j = size.y - 1, order = 3; j >= 0; j--, order++) {
			for (int i = 0; i < size.x; i++) {
				if (available_index.empty()) continue;

				size_t avail = available_index.front();
				bool& cur_act = entities_pool[avail].GetComponent<TagComponent>().active;

				size_t noise_location = (size_t)i + j * (size_t)size.y;
				if (!cur_act && glm::abs(noise[noise_location]) > 0.62f) {
					auto& ent_tr = entities_pool[avail].GetComponent<TransformComponent>();

					glm::ivec3 pos = glm::ivec3{ floor_pos + glm::vec2{ i - size.x / 2.0f, j - size.y / 2.0f }, 0 };
					if (pos == glm::ivec3{ floor_pos, 0 }) continue;
					ent_tr.position = pos;

					if (glm::distance(ent_tr.position, transform.position) > viewDistance) continue;
					if (box_pos.count({pos.x, pos.y})) continue;

					auto& sprite = entities_pool[avail].GetComponent<SpriteRendererComponent>();
					auto& colld = entities_pool[avail].GetComponent<CollisionComponent>();
					auto& tag = entities_pool[avail].GetComponent<TagComponent>();
					if (abs(noise[noise_location]) < 0.8f) {
						sprite.SetTexture(Texture::library["box"]);
						sprite.order = 0;
						sprite.Size({ 1, 1 });
						colld.Size({ 1, 1 });
						colld.Origin({ 0, 0 });
						tag.tag = "Box";
					}
					else {
						sprite.SetTexture(Texture::library["lamppost"]);
						sprite.order = order;
						sprite.Size({ 1, 2 });
						colld.Size({ 0.25, 0.6f });
						colld.Origin({ 0, -0.65f });
						tag.tag = "Lamp";
					}

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