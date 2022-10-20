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

	glfwSwapInterval(1);

	Audio::log = false;
	UI::log = true;
	Texture::log = false;
	Shader::log = false;

	Shader::Init();
	Audio::Init();
	UI::Init();
	Texture::defaultTex = Texture::Create("sprite_default", "texture/sprite_default.png");

	if (log) std::cout << "START Game " << ScreenResolution.x << "x" << ScreenResolution.y << "\n";
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
		menuLayer = std::make_shared<MenuLayer>();
		menuLayer->OnAttach();
		return true;
	case 1:
		gameLayer = std::make_shared<GameLayer>();
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
	LayerState* layerState = NULL;

	switch (layerIndex) {
	case 0:
		layerState = &menuLayer->state;
		break;
	case 1:
		layerState = &gameLayer->state;
		break;
	default:
		return false;
	}

	if (layerState->terminate) glfwSetWindowShouldClose(window, true);

	if (layerState->sceneIndex != -1 || layerState->sceneAddition != 0) {
		int prevLayer = this->layerIndex;
		if (layerState->sceneIndex != -1) this->layerIndex = layerState->sceneIndex;
		else this->layerIndex = glm::clamp<int>(this->layerIndex + layerState->sceneAddition, 0, 256);
		if (log) std::cout << "SWITCH Scene from " << prevLayer << " to " << this->layerIndex << "\n";
		ClearLayer(prevLayer);
		if (!LoadLayer(this->layerIndex)) return false;
		*layerState = {};
	}
	
	return true;
}