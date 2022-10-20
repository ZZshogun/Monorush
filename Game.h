#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "Layer.h"

class Game {
	GLFWwindow* window;

	void Setup();
	void Loop();

private:
	int layerIndex = 0;
	bool log = true;

	Ref<MenuLayer> menuLayer;
	Ref<GameLayer> gameLayer;
	Time time;

public:
	glm::vec2 ScreenResolution = { 1280, 720 };
	Game(glm::vec2 screen_resolution, bool logging = true);

	void ClearLayer(int layerIndex);
	bool LoadLayer(int layerIndex);
	void UpdateLayer(int layerIndex, Time time);
	bool ProcessLayerState(int layerIndex);
};

#endif
