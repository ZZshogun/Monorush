#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "Mesh.h"
#include "Sprite.h"
#include "Camera.h"
#include "Time.h"

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

	int scr_w = 1280, scr_h = 720;

	Game();
	void Set(int width, int height);
	void Run(void (*Start)(GameInfo& info), void (*Update)(GameInfo& info), void (*Render)(GameInfo& info));

};

#endif
