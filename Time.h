#ifndef TIME_CLASS_H
#define TIME_CLASS_H

#include "Core.h"

class Time {
	float _prevTime = 0;
	float _currentTime = 0;

	void _UpdateTime(float time);

	friend class Game;
public:
	float timeScale = 1;
	float deltaTime = 0;
	float unscaledDeltaTime = 0;

	static std::string FormatMinute(float seconds, bool spacing = false);
	static std::string FormatMilli(float seconds);
};


#endif