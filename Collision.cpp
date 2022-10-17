#include "Collision.h"

bool Collision::_InBox(glm::vec2 box_low, glm::vec2 box_high, glm::vec2 point) {
	return (point.x >= box_low.x && point.x <= box_high.x) && (point.y >= box_low.y && point.y <= box_high.y);
}

bool Collision::_BoxCollision(CollisionComponent& a, CollisionComponent& b,
	glm::vec2 translate_a, glm::vec2 translate_b) 
{
	glm::vec2 a_low = a.origin - a.size / 2.0f + translate_a;
	glm::vec2 a_high = a.origin + a.size / 2.0f + translate_a;

	//std::cout << "a_low (" << a_low.x << ", " << a_low.y << ") ";
	//std::cout << "a_high (" << a_high.x << ", " << a_high.y << ")\n";

	glm::vec2 b_low = b.origin - b.size / 2.0f + translate_b;
	glm::vec2 b_high = b.origin + b.size / 2.0f + translate_b;
	glm::vec2 b_low_up = glm::vec2{ b_low.x, b_high.y };
	glm::vec2 b_high_down = glm::vec2{ b_high.x, b_low.y };

	//std::cout << "b_low (" << b_low.x << ", " << b_low.y << ") ";
	//std::cout << "b_high (" << b_high.x << ", " << b_high.y << ")\n";

	bool b1 = _InBox(a_low, a_high, b_low);
	bool b2 = _InBox(a_low, a_high, b_high);
	bool b3 = _InBox(a_low, a_high, b_low_up);
	bool b4 = _InBox(a_low, a_high, b_high_down);

	//std::cout << "DOWN_LEFT : " << b1 << "\n";
	//std::cout << "UP_LEFT : " << b3 << "\n";
	//std::cout << "DOWN_RIGHT : " << b4 << "\n";
	//std::cout << "UP_RIGHT : " << b2 << "\n";

	return b1 || b2 || b3 || b4;
}

CollisionPacket Collision::Check(entt::entity entity, entt::registry& registry) {
	CollisionPacket packet;
	auto view = registry.view<CollisionComponent>();
	for (auto other : view) {
		if (entity == other) continue;
		auto& other_collision = view.get<CollisionComponent>(other);
		if (!other_collision.active) continue;
		auto& collision = view.get<CollisionComponent>(entity);
		auto& transformA = registry.get<TransformComponent>(entity);
		auto& transformB = registry.get<TransformComponent>(other);
		glm::vec2 translate_a = { transformA.position.x, transformA.position.y };
		glm::vec2 translate_b = { transformB.position.x, transformB.position.y };
		if (_BoxCollision(collision, other_collision, translate_a, translate_b)
			|| _BoxCollision(other_collision, collision, translate_b, translate_a)
			) {
			packet.count++;
			packet.positions.push_back(transformB.position);
			//auto& nameA = registry.get<TagComponent>(entity).tag;
			//auto& nameB = registry.get<TagComponent>(other).tag;
			//std::cout << nameA << " Collide with " << nameB << "\n";
		}
	}

	return packet;
}