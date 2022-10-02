#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "Mesh.h"
#include "Sprite.h"
#include "Camera.h"
#include "Time.h"
#include "Input.h"

struct GameInfo {
	int screenWidth;
	int screenHeight;
	GLFWwindow* window;
};

class Game {

	GLFWwindow* window;
	bool initialised = false;
	bool _Setup();
	void _UpdateInfo();

public:
	GameInfo info;
	std::vector<Ref<Shader>> shaders;

	Camera mainCamera;

	int scr_w = 1280, scr_h = 720;

	Game();
	void Set(int width, int height);
	void Run(void (*Start)(GameInfo& info), void (*Update)(GameInfo& info), void (*Render)(GameInfo& info));
	
	void Draw(Sprite* sprite, int shaderID = 0);
	void Draw(Mesh* sprite, int shaderID = 0);
	void Draw(Ref<Sprite>& sprite, int shaderID = 0);
	void Draw(Ref<Mesh>& sprite, int shaderID = 0);
};

#endif
