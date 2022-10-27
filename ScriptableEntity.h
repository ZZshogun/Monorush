#ifndef SCRIPTABLE_ENTITY_CLASS_H
#define SCRIPTABLE_ENTITY_CLASS_H

#include "Magia.h"
#include "Entity.h"
#include "Component.h"

class ScriptableEntity {

	friend class Scene;

protected:
	Entity entity;
	virtual void OnCreate() {}
	virtual void OnUpdate(Time time) {}
	virtual void OnDrawUI(Time time) {}

public:
	virtual ~ScriptableEntity() {}

	template <typename T>
	T& GetComponent() {
		return this->entity.GetComponent<T>();
	}

	Entity Instantiate() {
		Entity e = Entity{ entity.registry->create(), entity.registry };
		e.AddComponent<TagComponent>().name = "Unnamed Entity";
		e.AddComponent<TransformComponent>();
		return e;
	}

	Entity FindEntityOfName(std::string name) {
		auto view = entity.registry->view<TagComponent>();
		Entity search;
		for (auto entity : view) {
			if (!view.get<TagComponent>(entity).active) continue;
			if (view.get<TagComponent>(entity).name == name) {
				search = { entity, this->entity.registry };
				break;
			}
		}
		return search;
	}
};


#endif