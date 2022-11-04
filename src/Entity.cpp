#include "../header/Entity.h"

Entity::Entity() {}

Entity::Entity(entt::entity handle, entt::registry* registry) {
	this->handle = handle;
	this->registry = registry;
}