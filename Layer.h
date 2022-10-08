#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include "Magia.h"
#include "Scene.h"

class Layer {

private:
	Ref<Scene> scene = Scene::Create();
	Entity camera;

	Entity man, BG;

public:
	Layer();
	void OnAttach();
	void OnUpdate(float deltaTime);
};

#endif