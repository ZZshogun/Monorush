#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "Mesh.h"
#include "Sprite.h"
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
	std::vector<Ref<Shader>> shaders;

	Camera mainCamera;

	glm::vec2 ScreenResolution = { 1280, 720 };

	Game();
	void Set(glm::vec2 screen_resolution, glm::vec2 render_resolution = {16, 9});
	void Run(void (*Start)(GameInfo& info), void (*Update)(GameInfo& info), void (*Render)(GameInfo& info));
	void Stop();

	void Draw(Sprite* sprite, int shaderID = 0);
	void Draw(Mesh* sprite, int shaderID = 0);
	void Draw(Ref<Sprite>& sprite, int shaderID = 0);
	void Draw(Ref<Mesh>& sprite, int shaderID = 0);
};

#endif
