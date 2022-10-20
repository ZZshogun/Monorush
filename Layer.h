#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include "Scene.h"

struct LayerState {
	int sceneIndex = -1;
	int sceneAddition = 0;
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
	Ref<UI::Button> addVolume;
	Ref<UI::Button> subVolume;
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
	Entity man;
	Entity chest;
	Ref<UI::Button> menuButton;

	glm::vec4 texCol = Color::Black;
	float time_left = 301;
	float outoftimeLimit = 1.3f;
	float outoftime = 0;
	float floatOffsetSpeed = 250;
	float floatOffset = 0;

public:
	LayerState state{};

	GameLayer();
	~GameLayer();
	void OnAttach();
	void OnUpdate(Time time);
};

#endif