#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include "Layer.h"

class Game {
	GLFWwindow* window;

	void _Setup();
	void _Loop();

private:
	int layerIndex = 0;

	Ref<MenuLayer> menuLayer;
	Ref<GameLayer> gameLayer;
	Time time;

public:
	glm::vec2 ScreenResolution = { 1280, 720 };
	Game(glm::vec2 screen_resolution);

	void ClearLayer(int layerIndex);
	void LoadLayer(int layerIndex);
	void UpdateLayer(int layerIndex, Time time);
	void ProcessLayerState(int layerIndex);
};

#endif
