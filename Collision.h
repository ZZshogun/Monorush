#ifndef COLLISION_CLASS_H
#define COLLISION_CLASS_H

#include "Component.h"
#include "Entity.h"

struct BoxPacket {
	bool collided = false;
	float depth = 0;
	glm::vec2 normal = { 0, 0 };
	Entity entity;

	bool operator<(const BoxPacket& b) const {
		return this->entity < b.entity;
	}
};

struct CollisionPacket {
	bool empty = true;
	std::set<BoxPacket> boxes;
};

class Collision {
	Collision();
	static BoxPacket _BoxCollision(CollisionComponent& a, CollisionComponent& b,
		glm::vec2 translate_a, glm::vec2 translate_b);

	static CollisionPacket emptyPacket;

	using EntityPacket = std::map<entt::entity, CollisionPacket>;
	static std::map<entt::registry*, EntityPacket> collisionMap;
public:
	static void Update(entt::entity entity, entt::registry& registry);
	static CollisionPacket& Check(entt::entity entity, entt::registry& registry);
	static void ClearCollisionData();
};

#endif
