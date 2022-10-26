#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include "Scene.h"

struct LayerState {
	// SETTING
	void SceneAddition(int addition) { currentSceneIndex += addition; }
	void SwitchVolume() { volumeGain += 2.5f; if (volumeGain > 10) volumeGain = 0; else if (volumeGain < 0) volumeGain = 10; }
	void ToggleFullScreen() { fullScreen = !fullScreen; }
	void Terminate() { terminate = true; }

	// RESPONSE
	int currentSceneIndex = 0;
	float volumeGain = 10;
	bool fullScreen = false;
	bool terminate = false;
};

class Layer {
public:
	static void SetClearColor(glm::vec4 color);
};

class MenuLayer {
private:
	Ref<Scene> scene;
	Entity camera;
private:
	bool in_setting = false;

	// Main Menu
	Ref<UI::Button> playButton;
	Ref<UI::Button> settingButton;
	Ref<UI::Button> quitButton;

	// Settings
	Ref<UI::Button> fulLScreenButton;
	Ref<UI::Button> volumeButton;
	Ref<UI::Button> backButton;


public:
	LayerState state{};

	MenuLayer();
	~MenuLayer();
	void OnAttach();
	void OnUpdate(Time time);
};

class GameLayer {
private:
	Ref<Scene> scene;
	Entity camera;
private:
	Entity player;
	Entity boxSpawner;
	Ref<UI::Button> menuButton;
public:
	LayerState state{};

	GameLayer();
	~GameLayer();
	void OnAttach();
	void OnStart();
	void OnUpdate(Time time);
};

#endif