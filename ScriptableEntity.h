#ifndef SCRIPTABLE_ENTITY_CLASS_H
#define SCRIPTABLE_ENTITY_CLASS_H

#include "Magia.h"
#include "Entity.h"

class ScriptableEntity {

	Entity entity;
	friend class Scene;

protected:
	virtual void OnCreate() {}
	virtual void OnUpdate(Time time) {}
	virtual void OnDestroy() {}

public:
	virtual ~ScriptableEntity() {}

	template <typename T>
	T& GetComponent() {
		return this->entity.GetComponent<T>();
	}
};


#endif