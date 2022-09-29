#ifndef TIME_CLASS_H
#define TIME_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Time {
	Time();
public:

	static float timeScale;
	static float deltaTime;
	static float unscaledDeltaTime;
};

#endif
