#ifndef COLLISION_CLASS_H
#define COLLISION_CLASS_H

#include "Component.h"

struct CollisionPacket {
	int count = 0;
	std::vector<glm::vec3> positions;
};

class Collision {
	static bool _InBox(glm::vec2 box_low, glm::vec2 box_high, glm::vec2 point);
	static bool _BoxCollision(CollisionComponent& a, CollisionComponent& b,
		glm::vec2 translate_a, glm::vec2 translate_b);
	Collision();
public:
	static CollisionPacket Check(entt::entity entity, entt::registry& registry);
};

#endif
