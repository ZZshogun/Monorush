#include "Game.h"

Game::Game(glm::vec2 screen_resolution, bool logging) {
	ScreenResolution = screen_resolution;
	log = logging;
	Setup();
}

void Game::Setup() {
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
	WindowPos = glm::vec2{ (int)(mode.width - ScreenResolution.x), (int)(mode.height - ScreenResolution.y) } / 2.0f;
	glfwSetWindowPos(
		window, 
		(int)WindowPos.x, 
		(int)WindowPos.y
	);

	glfwSetScrollCallback(window, Input::ScanMouseScroll);
	Input::SetWindowInput(window);

	//glfwSwapInterval(0);

	glViewport(0, 0, (int)ScreenResolution.x, (int)ScreenResolution.y);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Audio::log = true;
	UI::log = false;
	Texture::log = false;
	Shader::log = true;
	Noise::log = true;

	Shader::Init();
	Audio::Init();
	UI::Init();
	Noise::Init();
	Texture::Init();
	Loop();
}

void Game::Loop() {
	glClearColor(0.95f, 0.97f, 1, 1);
	if (!LoadLayer(layerIndex)) return;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		time._UpdateTime((float)glfwGetTime());

		Input::ScanKey(window);
		Input::ScanMouse(window);
		UI::PollsEvent(window, time);

		if (ProcessLayerState(layerIndex)) {
			UpdateLayer(layerIndex, time);
		}
		else glfwSetWindowShouldClose(window, true);

		Input::ClearInputBuffer();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	if (log) std::cout << "---------------- CLEAN UP -----------------\n";
	glfwDestroyWindow(window);
	glfwTerminate();
	Audio::Destroy();
	UI::Destroy();
}

void Game::SetFullscreen(bool status) {
	fullscreen = status;
	if (fullscreen) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		prevScreenResolution = ScreenResolution;
		ScreenResolution = { mode->width, mode->height };
		glViewport(0, 0, mode->width, mode->height);
	}
	else {
		ScreenResolution = prevScreenResolution;
		glfwSetWindowMonitor(window, NULL, WindowPos.x, WindowPos.y,
			ScreenResolution.x, ScreenResolution.y, GLFW_DONT_CARE);
		glViewport(0, 0, ScreenResolution.x, ScreenResolution.y);
	}
}

void Game::SetVolume(float volume) {
	gameVolumeGain = volume;
	if (gameVolumeGain < 0) gameVolumeGain = 10;
	else if (gameVolumeGain > 10) gameVolumeGain = 0;

	Audio::SetListenerGain(gameVolumeGain);
}

void Game::ClearLayer(int layerIndex) {
	switch (layerIndex) {
		case 0:
			menuLayer.reset();
			break;
		case 1:
			gameLayer.reset();
			break;
		default:
			break;
	}
}

bool Game::LoadLayer(int layerIndex) {
	switch (layerIndex) {
	case 0:
		this->layerIndex = 0;
		menuLayer = std::make_shared<MenuLayer>();
		menuLayer->state->volumeGain = gameVolumeGain;
		menuLayer->state->currentSceneIndex = 0;
		menuLayer->state->fullScreen = this->fullscreen;
		menuLayer->state->resolution = this->ScreenResolution;
		ProcessLayerState(0);
		menuLayer->OnAttach();
		return true;
	case 1:
		this->layerIndex = 1;
		gameLayer = std::make_shared<GameLayer>();
		gameLayer->state->volumeGain = gameVolumeGain;
		gameLayer->state->currentSceneIndex = 1;
		gameLayer->state->fullScreen = this->fullscreen;
		gameLayer->state->resolution = this->ScreenResolution;
		ProcessLayerState(1);
		gameLayer->OnAttach();
		return true;
	default:
		return false;
	}
}

void Game::UpdateLayer(int layerIndex, Time time) {
	switch (layerIndex) {
	case 0:
		menuLayer->OnUpdate(time);
		break;
	case 1:
		gameLayer->OnUpdate(time);
		break;
	default:
		break;
	}
}

bool Game::ProcessLayerState(int layerIndex) {
	Ref<LayerState> layerState = NULL;

	switch (layerIndex) {
	case 0:
		layerState = menuLayer->state;
		break;
	case 1:
		layerState = gameLayer->state;
		break;
	default:
		return false;
	}

	if (!layerState->update) {
		layerState->volumeGain = gameVolumeGain;
		layerState->currentSceneIndex = this->layerIndex;
		layerState->fullScreen = this->fullscreen;
		layerState->resolution = this->ScreenResolution;
		return true;
	}

	if (layerState->terminate) glfwSetWindowShouldClose(window, true);

	if (layerState->fullScreen != this->fullscreen) {
		SetFullscreen(layerState->fullScreen);
	}

	if (layerState->volumeGain != this->gameVolumeGain) {
		SetVolume(layerState->volumeGain);
	}

	if (layerState->reload) {
		ClearLayer(layerIndex);
		if (!LoadLayer(layerIndex)) return false;
		if (log) std::cout << "RELOAD Scene " << layerIndex << "\n";
		return true;
	}

	if (layerState->currentSceneIndex != this->layerIndex) {
		int prevLayer = this->layerIndex;
		this->layerIndex = glm::clamp<int>(layerState->currentSceneIndex, 0, 256);
		if (log) std::cout << "SWITCH Scene from " << prevLayer << " to " << this->layerIndex << "\n";
		ClearLayer(prevLayer);
		if (!LoadLayer(this->layerIndex)) return false;
	}

	layerState.reset();
	layerState = std::make_shared<LayerState>();
	layerState->volumeGain = gameVolumeGain;
	layerState->currentSceneIndex = this->layerIndex;
	layerState->fullScreen = this->fullscreen;
	layerState->resolution = this->ScreenResolution;
	return true;
}