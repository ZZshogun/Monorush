#ifndef COLLISION_CLASS_H
#define COLLISION_CLASS_H

#include "Component.h"

struct BoxPacket {
	bool collided = false;
	float depth = 0;
	glm::vec2 normal = { 0, 0 };
};

struct CollisionPacket {
	std::vector<BoxPacket> boxes;
};

class Collision {
	static BoxPacket _BoxCollision(CollisionComponent& a, CollisionComponent& b,
		glm::vec2 translate_a, glm::vec2 translate_b);
	Collision();
public:
	static CollisionPacket Check(entt::entity entity, entt::registry& registry);
};

#endif
