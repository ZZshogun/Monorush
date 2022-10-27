#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include "Scene.h"

class PlayerController;

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

	PlayerController* playerController = NULL;
	glm::vec4 texCol = Color::Black;
	float outoftimeLimit = 1.3f, outoftime = 0;
	float floatOffsetSpeed = 250, floatOffset = 0;
	float opacityIncSpeed = 3, resetScreenOpacity = 0;
	float resetUIWaitTime = 0.67f, resetCountTime = 0;
	float fury_end_time = 0;

private:
	Entity player;
	Entity boxSpawner;
	Ref<UI::Button> menuButton;
	Ref<UI::Button> retryButton;
	Ref<UI::Button> endMenuButton;


public:
	LayerState state{};

	GameLayer();
	~GameLayer();
	void OnAttach();
	void OnStart();
	void OnUpdate(Time time);
};

#endif