#include "Game.h"

Game::Game() {
	window = NULL;
	info = { scr_w, scr_h, window };
}

void Game::Set(int width, int height) {
	scr_w = width;
	scr_h = height;
	info = { width, height, window };
}

bool Game::_Setup() {
	if (initialised) return true;

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
	//glfwSetKeyCallback(window, Input::ScanKey);

	glViewport(0, 0, scr_w, scr_h);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Input::SetWindowInput(window);

	std::cout << "CREATE Game " << scr_w << "x" << scr_h << "\n";
	return initialised = true;
}

void Game::Run(void (*Start)(GameInfo& info), void (*Update)(GameInfo& info), void (*Render)(GameInfo& info)) {
	if (!_Setup()) return;

	std::cout << "------------------ SETUP ------------------\n";
	_UpdateInfo();
	mainCamera.Set(info.screenWidth, info.screenHeight);
	shaders.push_back(Shader::Create("default.vert", "default.frag"));

	Start(info);
	std::cout << "--------------- END SETUP -----------------\n";

	float prevTime = 0;
	float currentTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.45f, 0.67f, 0.89f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		prevTime = currentTime;
		currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - prevTime;
		Time::unscaledDeltaTime = deltaTime;
		Time::deltaTime = Time::timeScale * deltaTime;

		_UpdateInfo();
		Input::ScanKey(window);
		Input::ScanMouse(window);

		for(int i = 0; i < shaders.size(); i++)
			mainCamera.Update(shaders[i].get());

		Update(info);
		Render(info);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	std::cout << "---------------- CLEAN UP -----------------\n";
	glfwDestroyWindow(window);
	glfwTerminate();
	return;
}

void Game::Draw(Sprite* sprite, int shaderID) {
	sprite->Draw(shaders[shaderID].get());
}

void Game::Draw(Mesh* mesh, int shaderID) {
	mesh->Draw(shaders[shaderID].get());
}

void Game::Draw(Ref<Sprite>& sprite, int shaderID) {
	Draw(sprite.get());
}

void Game::Draw(Ref<Mesh>& mesh, int shaderID) {
	Draw(mesh.get());
}


void Game::_UpdateInfo() {
	info.screenHeight = scr_h;
	info.screenWidth = scr_w;
	info.window = window;
}