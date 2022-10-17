#ifndef SCRIPTABLE_ENTITY_CLASS_H
#define SCRIPTABLE_ENTITY_CLASS_H

#include "Magia.h"
#include "Entity.h"
#include "Component.h"

class ScriptableEntity {

	Entity entity;
	friend class Scene;

protected:
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
		e.AddComponent<TagComponent>().tag = "Unnamed Entity";
		e.AddComponent<TransformComponent>();
		return e;
	}
};


#endif