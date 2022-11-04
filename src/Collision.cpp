#include "../header/Collision.h"

CollisionPacket Collision::emptyPacket = {{}};
std::unordered_map<entt::registry*, Collision::EntityPacket> Collision::collisionMap;

BoxPacket Collision::_BoxCollision(CollisionComponent& a, CollisionComponent& b,
	glm::vec2 translate_a, glm::vec2 translate_b) 
{
	glm::vec2 a_low = a.Origin() - a.Size() / 2.0f + translate_a;
	glm::vec2 a_high = a.Origin() + a.Size() / 2.0f + translate_a;

	glm::vec2 b_low = b.Origin() - b.Size() / 2.0f + translate_b;
	glm::vec2 b_high = b.Origin() + b.Size() / 2.0f + translate_b;

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

void Collision::Update(entt::entity entity, entt::registry& registry) {
	if (!registry.valid(entity)) return;

	auto collisionGroup =
		registry.group(entt::get<CollisionComponent, TransformComponent, TagComponent>);
	auto [a_cold, a_tran, a_tag] =
		collisionGroup.get<CollisionComponent, TransformComponent, TagComponent>(entity);
	if (!a_cold.active || !a_tag.active) return;

	CollisionPacket packet;

	for (auto jt = collisionGroup.begin(); jt != collisionGroup.end(); ++jt) {
		if (entity == *jt) continue;
		auto [b_cold, b_tran, b_tag] =
			collisionGroup.get<CollisionComponent, TransformComponent, TagComponent>(*jt);
		if (!b_cold.active || !b_tag.active) continue;
		if (a_cold.ignoredTags.count(b_tag.tag) || b_cold.ignoredTags.count(a_tag.tag)) continue;

		BoxPacket boxpacket = _BoxCollision(a_cold, b_cold, a_tran.position, b_tran.position);
		if (boxpacket.collided) {
			boxpacket.entity = Entity{ *jt, &registry };
			if (packet.empty) packet.empty = false;
			packet.boxes.insert(boxpacket);
		}
	}

	collisionMap[&registry][entity] = packet;
}

CollisionPacket& Collision::Check(entt::entity entity, entt::registry& registry) {
	if (collisionMap.find(&registry) == collisionMap.end())
		return emptyPacket;
	if (collisionMap[&registry].find(entity) == collisionMap[&registry].end())
		return emptyPacket;
	return collisionMap[&registry][entity];
}

void Collision::ClearCollisionData() {
	for (auto& scene : collisionMap)
		scene.second.clear();
	collisionMap.clear();
}