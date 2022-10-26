#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Time.h"

class GameManager {
public:
	inline static bool gameOver = false;
	inline static float remainingTime = 181;

	static void Update(Time time) {
		if (gameOver) return;
		remainingTime -= time.deltaTime;
		remainingTime = glm::clamp<float>(remainingTime, 0, INFINITY);
	}
};

#endif
