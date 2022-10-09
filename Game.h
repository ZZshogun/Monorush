#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "Layer.h"

class Game {

	GLFWwindow* window;

	void _Setup();
	void _Loop();

private:
	Layer layer;
	Time time;

public:
	glm::vec2 ScreenResolution = { 1280, 720 };
	Game(glm::vec2 screen_resolution);

};

#endif
