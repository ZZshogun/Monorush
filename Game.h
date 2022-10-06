#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "Core.h"
#include "Camera.h"
#include "Animation.h"
#include "Time.h"
#include "Input.h"

struct GameInfo {
	glm::vec2 screenResolution;
	GLFWwindow* window;
};

class Game {

	GLFWwindow* window;
	bool initialised = false;
	bool fullScreen = false;

	bool _Setup();
	void _UpdateInfo();
	void _SetFullscreen(bool status);
	void _DefaultKeyBind();

	glm::vec2 _ScreenResolution = {1280, 720};

public:
	glm::vec2 ScreenResolution = { 1280, 720 };

	GameInfo info;
	Camera mainCamera;

	Game();
	void Set(glm::vec2 screen_resolution, bool fullScreen = false, glm::vec2 render_resolution = {16, 9});
	void Run(void (*Start)(GameInfo& info), void (*Update)(GameInfo& info), void (*Render)(GameInfo& info));
	void Stop();
};

#endif
