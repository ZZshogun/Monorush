#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include "Scene.h"

class PlayerController;

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
	Ref<LayerState> state;

	MenuLayer();
	~MenuLayer();
	void OnAttach();
	void OnUpdate(Time& time);
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
	Ref<UI::Button> pauseButton;
	Ref<UI::Button> resumeButton;
	Ref<UI::Button> settingButton;
	Ref<UI::Button> menuButton;
	Ref<UI::Button> retryButton;
	Ref<UI::Button> endMenuButton;
	Ref<UI::Button> fulLScreenButton;
	Ref<UI::Button> volumeButton;
	Ref<UI::Button> backButton;

public:
	Ref<LayerState> state;

	GameLayer();
	~GameLayer();
	void OnAttach();
	void OnUpdate(Time& time);
};

#endif