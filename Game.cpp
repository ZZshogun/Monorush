#include "Game.h"

Game::Game() {
	window = NULL;
	info = { scr_w, scr_h, window };
}

void Game::Set(int width, int height) {
	scr_w = width;
	scr_h = height;
	info = { width, height, window };

	if (!_Setup()) return;

	std::cout << "CREATE Game " << width << "x" << height << "\n";
}

bool Game::_Setup() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(scr_w, scr_h, "Game", NULL, NULL);
	if (!window) {
		std::cout << "ERROR Window " << window << "\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

	GLFWvidmode mode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode.width - scr_w) / 2, (mode.height - scr_h) / 2);

	//glfwSetWindowSizeCallback(window, OnResizeWindow);

	glViewport(0, 0, scr_w, scr_h);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return initialised = true;
}

void Game::Run(void (*Start)(GameInfo& info), void (*Update)(GameInfo& info), void (*Render)(GameInfo& info)) {
	if (!initialised) {
		if (!_Setup()) return;
	}

	_UpdateInfo();

	Start(info);

	float prevTime = 0;
	float currentTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.45, 0.67, 0.89, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		prevTime = currentTime;
		currentTime = glfwGetTime();
		float deltaTime = currentTime - prevTime;
		Time::unscaledDeltaTime = deltaTime;
		Time::deltaTime = Time::timeScale * deltaTime;

		_UpdateInfo();

		Update(info);
		Render(info);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "DELETE Game\n";
	return;
}

void Game::_UpdateInfo() {
	info.screenHeight = scr_h;
	info.screenWidth = scr_w;
	info.window = window;
}