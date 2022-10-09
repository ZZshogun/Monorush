#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "Component.h"
#include "Entity.h"
#include "Renderer.h"

class Scene {
	entt::registry scene_registry;

public:
	Scene();
	static Ref<Scene> Create();

	Entity CreateEntity(std::string name = "");

	void OnUpdate(Time time);
};

#endif