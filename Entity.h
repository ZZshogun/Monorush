#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include "Magia.h"
#include "Component.h"

class Scene;
// rem
class ScriptableEntity;

class Entity {

	entt::entity handle = entt::null;
	entt::registry* registry = NULL;

	friend class ScriptableEntity;

public:
	Entity();
	Entity(entt::entity handle, entt::registry* registry);

	bool IsValid() {
		return this->registry->valid(handle);
	}

	template <typename T>
	bool HasComponent() {
		return this->registry->any_of<T>(this->handle);
	}

	template <typename T>
	T& GetComponent() {
		assert(HasComponent<T>());
		return this->registry->get<T>(this->handle);
	}

	template <typename T>
	T& GetScript() {
		assert(HasComponent<NativeScriptComponent>());
		return this->registry->get<NativeScriptComponent>(this->handle).GetScript<T>();
	}

	template <typename T, typename ...Args>
	T& AddComponent(Args&&... args) {
		assert(!HasComponent<T>());
		return this->registry->emplace<T>(this->handle, std::forward<Args>(args)...);
	}

	template <typename T>
	size_t RemoveComponent() {
		return this->registry->remove<T>(this->handle);
	}

};
#endif