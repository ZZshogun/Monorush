#include "Collision.h"

BoxPacket Collision::_BoxCollision(CollisionComponent& a, CollisionComponent& b,
	glm::vec2 translate_a, glm::vec2 translate_b) 
{
	glm::vec2 a_low = a.origin - a.size / 2.0f + translate_a;
	glm::vec2 a_high = a.origin + a.size / 2.0f + translate_a;

	glm::vec2 b_low = b.origin - b.size / 2.0f + translate_b;
	glm::vec2 b_high = b.origin + b.size / 2.0f + translate_b;

	float depth = std::numeric_limits<float>::max();
	glm::vec2 normal = {0, 0};

	if (a_low.x > b_high.x || a_high.x < b_low.x) return { false, 0, normal };

	if (b_high.x - a_low.x < depth) {
		depth = b_high.x - a_low.x;
		normal = { 1, 0 };
	}
	if (a_high.x - b_low.x < depth)  {
		depth = a_high.x - b_low.x;
		normal = { -1, 0 };
	}

	if (a_low.y > b_high.y || a_high.y < b_low.y) return { false, 0, normal };

	if (b_high.y - a_low.y < depth) {
		depth = b_high.y - a_low.y;
		normal = { 0, 1 };
	}
	if (a_high.y - b_low.y < depth) {
		depth = a_high.y - b_low.y;
		normal = { 0, -1 };
	}

	return {true, glm::abs(depth), normal};
}

CollisionPacket Collision::Check(entt::entity entity, entt::registry& registry) {
	CollisionPacket packet;
	auto view = registry.view<CollisionComponent>();
	for (auto other : view) {
		if (entity == other) continue;
		auto& other_collision = view.get<CollisionComponent>(other);
		if (!other_collision.active || !registry.get<TagComponent>(other).active) continue;
		auto& collision = view.get<CollisionComponent>(entity);
		auto& transformA = registry.get<RigidbodyComponent>(entity);
		auto& transformB = registry.get<TransformComponent>(other);
		glm::vec2 translate_a = { transformA.position.x, transformA.position.y };
		glm::vec2 translate_b = { transformB.position.x, transformB.position.y };
		BoxPacket boxpacketA = _BoxCollision(collision, other_collision, translate_a, translate_b);
		if (boxpacketA.collided) packet.boxes.push_back(boxpacketA);
	}

	return packet;
}