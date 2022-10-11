#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include "Scene.h"

class Layer {

private:
	Ref<Scene> scene = Scene::Create();
	Entity camera;
	Entity man;
	Entity chest;

public:
	Layer();
	void OnAttach();
	void OnUpdate(Time time);
};

#endif