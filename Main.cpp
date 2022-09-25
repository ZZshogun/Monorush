
#include "Mesh.h"
#include "Camera.h"

int WIN_WIDTH = 1280;
int WIN_HEIGHT = 720;

void PlayerInputs(GLFWwindow*, Mesh&, float);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Window", NULL, NULL);
	if (!window) {
		MagiaLog::Error("WINDOW", (int)window);
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

	GLFWvidmode mode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode.width - WIN_WIDTH) / 2, (mode.height - WIN_HEIGHT) / 2);

	Shader defaultShader("default.vert", "default.frag");

	std::vector<GLuint> indices = {
		0, 3, 1,
		1, 3, 2
	};

	std::vector<Vertex> verticesBG = {
		{glm::vec3{-WIN_WIDTH, -WIN_WIDTH, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, 0}},
		{glm::vec3{-WIN_WIDTH, WIN_WIDTH, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, 10}},
		{glm::vec3{WIN_WIDTH, WIN_WIDTH, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{10, 10}},
		{glm::vec3{WIN_WIDTH, -WIN_WIDTH, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{10, 0}},
	};

	std::vector<Texture> texturesBG = {
		Texture("texture/tile.png", 0)
	};

	std::vector<Vertex> vertices = {
		{glm::vec3{-100, -100, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, 0}},
		{glm::vec3{-100, 100, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, 1}},
		{glm::vec3{100, 100, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{1, 1}},
		{glm::vec3{100, -100, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{1, 0}},
	};

	std::vector<Texture> textures = {
		Texture("texture/man.png", 0)
	};

	Mesh BG(verticesBG, indices, texturesBG);
	Mesh man(vertices, indices, textures);

	Camera camera(glm::vec3(0));

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float deltaTime = 0;
	float prevTime = 0;
	float currTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.45, 0.67, 0.89, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		prevTime = currTime;
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;

		camera.Update(defaultShader, WIN_WIDTH, WIN_HEIGHT);

		BG.Draw(defaultShader);

		PlayerInputs(window, man, deltaTime);
		man.Draw(defaultShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

float playerSpeed = 300;

void PlayerInputs(GLFWwindow* window, Mesh& player, float dTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		player.position.y += playerSpeed * dTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		player.position.y -= playerSpeed * dTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player.position.x += playerSpeed * dTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player.position.x -= playerSpeed * dTime;
	}
}