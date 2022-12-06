#include "../header/Game.h"

Game::Game(bool logging) {
	ScreenResolution = {0, 0};
	prevScreenResolution = {0, 0};
	log = logging;
}

Game::Game(glm::vec2 screen_resolution, bool logging) {
	ScreenResolution = screen_resolution;
	prevScreenResolution = screen_resolution;
	log = logging;
}

void Game::Run() {
	Setup();
}

void Game::Setup() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWvidmode mode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
	if (ScreenResolution.x <= 0 || ScreenResolution.y <= 0) {
		ScreenResolution = glm::vec2{ mode.width, mode.height } * 0.8f;
		prevScreenResolution = ScreenResolution;
	}

	GLFWmonitor* monitor = NULL;
	window = 
		glfwCreateWindow(
			(int)ScreenResolution.x, 
			(int)ScreenResolution.y, 
			"Monorush", 
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

	WindowPos = glm::vec2{ (int)(mode.width - ScreenResolution.x), (int)(mode.height - ScreenResolution.y) } / 2.0f;
	glfwSetWindowPos(
		window, 
		(int)WindowPos.x, 
		(int)WindowPos.y
	);

	glfwSetErrorCallback(Game::WindowErrorCallback);
	glfwSetWindowSizeCallback(window, Game::WindowResizeCallback);
	glfwSetScrollCallback(window, Input::ScanMouseScroll);
	Input::SetWindowInput(window);

	glfwSetWindowSizeLimits(window, 640, 360, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowAspectRatio(window, 16, 9);
	//glfwSwapInterval(0);

	SetIcon("texture/icon.png");

	glViewport(0, 0, (int)ScreenResolution.x, (int)ScreenResolution.y);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Audio::log = log;
	UI::log = log;
	Texture::log = log;
	Shader::log = log;
	Noise::log = log;

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
		glClear(GL_COLOR_BUFFER_BIT);
		
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

void Game::WindowErrorCallback(int err, const char* str) {
	if (err) {
		std::cout << "ERROR GL " << err << "\n";
	}
}

void Game::WindowResizeCallback(GLFWwindow* window, int x, int y) {
	glViewport(0, 0, x, y);
}

void Game::SetIcon(std::string file) {
	std::string path = MAGIA_PATH(file);
	if (log) std::cout << "LOAD Icon " << path << "\n";
	GLFWimage image{};
	image.pixels = stbi_load(path.c_str(), &image.width, &image.height, 0, 4);
	assert(image.pixels);
	glfwSetWindowIcon(window, 1, &image);
	stbi_image_free(image.pixels);
}

void Game::ClearLayer(int layerIndex) {
	switch (layerIndex) {
		case 0:
			menuLayer.~shared_ptr();
			menuLayer.reset();
			break;
		case 1:
			gameLayer.~shared_ptr();
			gameLayer.reset();
			break;
		default:
			break;
	}
}

bool Game::LoadLayer(int layerIndex) {
	this->layerIndex = layerIndex;
	switch (layerIndex) {
	case 0:
		menuLayer = std::make_shared<MenuLayer>();
		UpdatePacket(menuLayer->state);
		ProcessLayerState(layerIndex);
		menuLayer->OnAttach();
		return true;
	case 1:
		gameLayer = std::make_shared<GameLayer>();
		UpdatePacket(gameLayer->state);
		ProcessLayerState(layerIndex);
		gameLayer->OnAttach();
		return true;
	default:
		return false;
	}
}

void Game::UpdateLayer(int layerIndex, Time& time) {
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

void Game::UpdatePacket(Ref<LayerState>& state) {
	glm::ivec2 winSize = { 1280, 720 };
	glfwGetWindowSize(window, &winSize.x, &winSize.y);

	if (!fullscreen && ScreenResolution != winSize)
		prevScreenResolution = winSize;
	ScreenResolution = winSize;

	state->update = false;
	state->window = window;
	state->volumeGain = gameVolumeGain;
	state->currentSceneIndex = layerIndex;
	state->fullScreen = fullscreen;
	state->resolution = ScreenResolution;
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
		UpdatePacket(layerState);
		return true;
	}

	if (layerState->terminate) glfwSetWindowShouldClose(window, true);

	if (layerState->fullScreen != this->fullscreen) {
		fullscreen = layerState->fullScreen;
		if (fullscreen) {
			glm::ivec2 winPos = { 0, 0 };
			glfwGetWindowPos(window, &winPos.x, &winPos.y);
			WindowPos = winPos;

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			auto mode = glfwGetVideoMode(monitor);
			glm::ivec2 sz = { mode->width, mode->height };
			glfwSetWindowMonitor(window, monitor, 0, 0, sz.x, sz.y, GLFW_DONT_CARE);

			prevScreenResolution = ScreenResolution;
			ScreenResolution = sz;
			glViewport(0, 0, sz.x, sz.y);
		}
		else {
			ScreenResolution = prevScreenResolution;
			glfwSetWindowMonitor(window, NULL, WindowPos.x, WindowPos.y,
				ScreenResolution.x, ScreenResolution.y, GLFW_DONT_CARE);
			glViewport(0, 0, ScreenResolution.x, ScreenResolution.y);
		}
	}

	if (layerState->volumeGain != this->gameVolumeGain) {
		gameVolumeGain = layerState->volumeGain;
		if (gameVolumeGain < 0) gameVolumeGain = 1;
		else if (gameVolumeGain > 1) gameVolumeGain = 0;
		Audio::SetListenerGain(gameVolumeGain);
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

	UpdatePacket(layerState);
	return true;
}