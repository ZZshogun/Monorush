#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "Collision.h"
#include "Component.h"
#include "Entity.h"
#include "Renderer.h"
#include "UI.h"
#include "ScriptableEntity.h"

struct LayerState {
	// SETTING
	void ReloadScene() {
		reload = true;
		update = true;
	}
	void SceneAddition(int addition) {
		currentSceneIndex += addition; update = true;
	}
	void SwitchVolume() {
		volumeGain += 2.5f;
		if (volumeGain > 10) volumeGain = 0;
		else if (volumeGain < 0) volumeGain = 10;
		update = true;
	}
	void ToggleFullScreen() {
		fullScreen = !fullScreen;
		update = true;
	}
	void Terminate() {
		terminate = true;
		update = true;
	}

	// RESPONSE
	bool update = false;
	int currentSceneIndex = 0;
	float volumeGain = 10;
	bool reload = false;
	bool fullScreen = false;
	bool terminate = false;
	glm::vec2 resolution = { 1280, 720 };
};

class Scene {
	entt::registry scene_registry;

	friend class ScriptableEntity;
public:
	Scene();
	static Ref<Scene> Create();
	void Destroy();
	
	Entity CreateEntity(std::string name = "");

	void OnUpdate(Ref<LayerState>& layerState, Time time);
};

#endif