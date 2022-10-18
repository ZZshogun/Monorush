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

	Texture::defaultTex = Texture::Create("sprite_default", "texture/sprite_default.png");

	Audio::Init();

	UI::Init();

	std::cout << "START Game " << ScreenResolution.x << "x" << ScreenResolution.y << "\n";
	_Loop();
}

void Game::_Loop() {

	glClearColor(0.95f, 0.97f, 1, 1);

	LoadLayer(layerIndex);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		time._UpdateTime((float)glfwGetTime());

		Input::ScanKey(window);
		Input::ScanMouse(window);
		UI::PollsEvent(window);

		UpdateLayer(layerIndex, time);

		Input::ClearInputBuffer();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "---------------- CLEAN UP -----------------\n";
	glfwDestroyWindow(window);
	glfwTerminate();
	Audio::Destroy();
	UI::Destroy();
}

void Game::ClearLayer(int layerIndex) {
	switch (layerIndex) {
		case 0:
			menuLayer = {};
			break;
		case 1:
			gameLayer = {};
			break;
		default:
			break;
	}
}

void Game::LoadLayer(int layerIndex) {
	switch (layerIndex) {
	case 0:
		menuLayer.OnAttach();
		break;
	case 1:
		gameLayer.OnAttach();
		break;
	default:
		break;
	}
}

void Game::UpdateLayer(int layerIndex, Time time) {
	switch (layerIndex) {
	case 0:
		menuLayer.OnUpdate(time);
		break;
	case 1:
		gameLayer.OnUpdate(time);
		break;
	default:
		break;
	}
}