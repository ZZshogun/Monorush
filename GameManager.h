#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Time.h"

class GameManager {
public:
	enum GameState {
		WIN,
		LOSS,
		ONGOING,
		UNKNOWN,
	};

private:
	inline static float furyBonusTime = 10;

public:
	inline static float _furyTimer = 0;

	inline static bool gameOver = false;
	inline static float maxRemainingTime = 180;
	inline static float remainingTime = maxRemainingTime;
	inline static float difficulty = 1;
	inline static float difficultyIncRate = 0.02f;
	inline static bool half = false, fury = false;

	inline static GameState gameState = ONGOING;

	static void Update(Time time) {
		if (gameOver) return;
		difficulty += difficultyIncRate * time.deltaTime;

		if (fury) {
			_furyTimer += time.deltaTime;
			if (_furyTimer >= furyBonusTime) {
				_furyTimer = 0;
				fury = false;
				difficulty = 3.0f;
				difficultyIncRate = 0.02f;
				std::cout << "EVENT Fury has ended!\n";
			}
		}

		if (!fury && !half && remainingTime <= 90) {
			half = true;
			fury = true;
			difficulty = 6.0f;
			difficultyIncRate = 0.18f;
			std::cout << "EVENT Fury has started!\n";
		}

		remainingTime -= time.deltaTime;
		if (remainingTime <= 0) {
			remainingTime = 0;
			gameOver = true;
		}
	}

	static void Reset() {
		_furyTimer = 0;
		gameOver = false;
		remainingTime = 180;
		difficulty = 1;
		difficultyIncRate = 0.02f;
		half = false;
		fury = false;
	}
};

#endif
