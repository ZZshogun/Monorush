#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include "Scene.h"

class Layer {
protected:
	Ref<Scene> scene = Scene::Create();
	Entity camera;
public:
	Layer() {
		camera = scene->CreateEntity("Camera");
		camera.AddComponent<CameraComponent>().primary = true;
	}
	virtual void OnAttach() {}
	virtual void OnUpdate(Time time) { 
		scene->OnUpdate(time); 
		UI::StartUI();
		UI::Anchor(CENTER);
		UI::DrawButtons(time); 
		UI::EndUI();
	}

	void Destroy() {
		
	}

	static void SetClearColor(glm::vec4 color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}
};

class MenuLayer : public Layer {
public:
	void OnAttach();
	void OnUpdate(Time time) override;
};

class GameLayer : public Layer {

public:
	static float time_left;

private:
	Entity man;
	Entity chest;

public:
	void OnAttach();
	void OnUpdate(Time time) override;
};

#endif