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

	bool operator<(const Entity& other) const {
		return this->handle < other.handle;
	}

	bool IsValid() const {
		return this->registry->valid(handle);
	}

	template <typename T>
	bool HasComponent() const {
		assert(IsValid());
		return this->registry->any_of<T>(this->handle);
	}

	template <typename T>
	T& GetComponent() const {
		assert(HasComponent<T>());
		return this->registry->get<T>(this->handle);
	}

	template <typename T>
	T* TryGetComponent() const {
		return this->registry->try_get<T>(this->handle);
	}

	template <typename T>
	T& GetScript() const {
		assert(HasComponent<NativeScriptComponent>());
		return this->registry->get<NativeScriptComponent>(this->handle).GetScript<T>();
	}

	template <typename T, typename ...Args>
	T& AddComponent(Args&&... args) {
		assert(!HasComponent<T>());
		return this->registry->emplace<T>(this->handle, std::forward<Args>(args)...);
	}

	template <typename T>
	void RemoveComponent() {
		assert(HasComponent<T>());
		this->registry->erase<T>(this->handle);
	}

	void Destroy() {
		if (HasComponent<NativeScriptComponent>())
			RemoveComponent<NativeScriptComponent>();
		this->registry->destroy(this->handle);
	}

};
#endif