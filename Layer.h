#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include "Scene.h"

struct LayerState {
	int sceneIndex = -1;
	int sceneAddition = 0;
	bool terminate = false;
};

class Layer {
protected:
	Ref<Scene> scene = Scene::Create();
	Entity camera;
public:
	LayerState state{};

	Layer() {
		camera = scene->CreateEntity("Camera");
		camera.AddComponent<CameraComponent>().primary = true;
	}
	~Layer() {
		UI::ClearBuffers();
		Texture::ClearHandles();
		Audio::ClearBuffers();
		scene->Destroy();
	}

	virtual void OnAttach() {}
	virtual void OnUpdate(Time time) { 
		scene->OnUpdate(time); 
		UI::StartUI();
		UI::Anchor(CENTER);
		UI::DrawButtons(time); 
		UI::EndUI();
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
private:
	Entity man;
	Entity chest;

	glm::vec4 texCol = glm::vec4{ 0, 0, 0, 1 };
	float time_left = 300;
	float outoftimeLimit = 1.3f;
	float outoftime = 0;
	float floatOffsetSpeed = 250;
	float floatOffset = 0;

public:
	void OnAttach();
	void OnUpdate(Time time) override;
};

#endif