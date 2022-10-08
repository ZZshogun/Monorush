#include "Game.h"

Game::Game(glm::vec2 screen_resolution) {
	ScreenResolution = screen_resolution;
	_Setup();
}

void Game::_Setup() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor* monitor = NULL;
	window = 
		glfwCreateWindow(
			(int)ScreenResolution.x, 
			(int)ScreenResolution.y, 
			"Game", 
			monitor, 
			NULL
		);
	if (!window) {
		std::cout << "ERROR Window " << window << "\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	GLFWvidmode mode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(
		window, 
		(int)(mode.width - ScreenResolution.x) / 2, 
		(int)(mode.height - ScreenResolution.y) / 2
	);

	glfwSetScrollCallback(window, Input::ScanMouseScroll);
	Input::SetWindowInput(window);

	glViewport(0, 0, (int)ScreenResolution.x, (int)ScreenResolution.y);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader::CompileAll();
	Texture::defaultTex = Texture::Create("texture/sprite_default.png");

	std::cout << "START Game " << ScreenResolution.x << "x" << ScreenResolution.y << "\n";
	_Loop();
}

void Game::_Loop() {
	float prevTime = 0;
	float currentTime = (float)glfwGetTime();

	layer.OnAttach();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.07f, 0.09f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		prevTime = currentTime;
		currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - prevTime;

		Input::ScanKey(window);
		Input::ScanMouse(window);

		layer.OnUpdate(deltaTime);

		Input::ClearInputBuffer();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "---------------- CLEAN UP -----------------\n";
	glfwDestroyWindow(window);
	glfwTerminate();
}