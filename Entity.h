#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include "Magia.h"

class Scene;

class Entity {

	entt::entity handle = entt::null;
	entt::registry* registry = NULL;

public:
	Entity();
	Entity(entt::entity handle, entt::registry* registry);

	template <typename T, typename ...Args>
	T& AddComponent(Args&&... args) {
		return this->registry->emplace<T>(this->handle, std::forward<Args>(args)...);
	}

	template <typename T>
	T& GetComponent() {
		return this->registry->get<T>(this->handle);
	}

	template <typename T>
	bool HasComponent() {
		return this->registry->any_of<T>(this->handle);
	}

	template <typename T>
	size_t RemoveComponent() {
		return this->registry->remove<T>(this->handle);
	}

};
#endif