#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "Component.h"
#include "Entity.h"
#include "Renderer.h"
#include "ScriptableEntity.h"

class Scene {
	entt::registry scene_registry;

	friend class ScriptableEntity;
public:
	Scene();
	static Ref<Scene> Create();

	Entity CreateEntity(std::string name = "");

	void OnUpdate(Time time);
};

#endif