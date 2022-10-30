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
	bool fullscreen = false;
	float gameVolumeGain = 10;

	Ref<MenuLayer> menuLayer;
	Ref<GameLayer> gameLayer;
	Time time;

	glm::ivec2 WindowPos = { 0, 0 };
	glm::ivec2 ScreenResolution = { 1280, 720 };
	glm::ivec2 prevScreenResolution = { 1280, 720 };

	void ClearLayer(int layerIndex);
	bool LoadLayer(int layerIndex);
	void UpdateLayer(int layerIndex, Time time);
	bool ProcessLayerState(int layerIndex);

	void SetFullscreen(bool status);
	void SetVolume(float volume);

public:
	Game(glm::vec2 screen_resolution, bool logging = true);
};

#endif
