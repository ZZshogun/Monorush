#include "Game.h"

Game::Game() {
	window = NULL;
	info = { ScreenResolution, window };
}

void Game::Set(glm::vec2 screen_resolution, bool fullScreen, glm::vec2 render_resolution) {
	this->fullScreen = fullScreen;
	_ScreenResolution = screen_resolution;
	ScreenResolution = _ScreenResolution;
	info = { ScreenResolution, window };
	mainCamera.Set(render_resolution);
}

bool Game::_Setup() {
	if (initialised) return true;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor* monitor = NULL;
	if (fullScreen) monitor = glfwGetPrimaryMonitor();
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
		return false;
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

	std::cout << "START Game " << ScreenResolution.x << "x" << ScreenResolution.y << "\n";
	initialised = true;
	return true;
}

void Game::Run(void (*Start)(GameInfo& info), void (*Update)(GameInfo& info), void (*Render)(GameInfo& info)) {
	if (!_Setup()) return;

	std::cout << "------------------ SETUP ------------------\n";
	_UpdateInfo();

	Shader::CompileAll();
	mainCamera.cameraShader = Shader::Get("unlit");

	Start(info);
	std::cout << "--------------- END SETUP -----------------\n";

	float prevTime = 0;
	float currentTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		//glClearColor(0.45f, 0.67f, 0.89f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		mainCamera.Draw();
		
		prevTime = currentTime;
		currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - prevTime;
		Time::unscaledDeltaTime = deltaTime;
		Time::deltaTime = Time::timeScale * deltaTime;

		Input::ScanKey(window);
		Input::ScanMouse(window);
		_DefaultKeyBind();
		_UpdateInfo();

		Update(info);

		for (auto it = Shader::Begin(); it != Shader::End(); it++)
			mainCamera.Update(it->second);

		Render(info);

		Input::ClearInputBuffer();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	std::cout << "---------------- CLEAN UP -----------------\n";
	glfwDestroyWindow(window);
	glfwTerminate();
	return;
}

void Game::_SetFullscreen(bool status) {
	fullScreen = status;

	GLFWmonitor* monitor = NULL;
	GLFWmonitor* cur = glfwGetPrimaryMonitor();
	GLFWvidmode mode = *glfwGetVideoMode(cur);
	glm::vec2 pos = {0, 0};

	if (fullScreen) {

		float max_h = mode.width / 16.0f * 9;
		float max_w = mode.height / 9.0f * 16;

		if (max_h <= mode.height) 
			ScreenResolution = { mode.width, max_h };
		else
			ScreenResolution = { max_w, mode.height };
		monitor = cur;
	}
	else ScreenResolution = _ScreenResolution;

	pos.x = mode.width - ScreenResolution.x;
	pos.y = mode.height - ScreenResolution.y;
	pos /= 2;

	glViewport(0, 0, (int)ScreenResolution.x, (int)ScreenResolution.y);
	glfwSetWindowMonitor(
		window, 
		monitor, 
		(int)pos.x, 
		(int)pos.y, 
		(int)ScreenResolution.x, 
		(int)ScreenResolution.y, 
		GLFW_DONT_CARE
	);
}

void Game::_DefaultKeyBind() {

	if (Input::GetKey(GLFW_KEY_LEFT_ALT)) {
		if (Input::GetKeyDown(GLFW_KEY_ENTER)) _SetFullscreen(!fullScreen);
		if (Input::GetKeyDown(GLFW_KEY_ESCAPE)) Stop();
	}
}

void Game::_UpdateInfo() {
	info.screenResolution = ScreenResolution;
	info.window = window;
}

void Game::Stop() {
	glfwSetWindowShouldClose(window, GL_TRUE);
	std::cout << "STOP Game\n";
}