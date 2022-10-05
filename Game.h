#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "Core.h"
#include "Camera.h"
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
	glm::vec2 _CameraResolution = {16, 9};

public:
	GameInfo info;
	std::map<std::string, Ref<Shader>> shaderLUT;

	Camera mainCamera;

	glm::vec2 ScreenResolution = { 1280, 720 };

	Game();
	void Set(glm::vec2 screen_resolution, bool fullScreen = false, glm::vec2 render_resolution = {16, 9});
	void Run(void (*Start)(GameInfo& info), void (*Update)(GameInfo& info), void (*Render)(GameInfo& info));
	void Stop();

	void Draw(Ref<Sprite>& sprite, std::string shaderName = "unlit");
	void Draw(Ref<Mesh>& sprite, std::string shaderName = "unlit");
};

#endif
