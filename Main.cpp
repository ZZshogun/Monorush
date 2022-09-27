
#include "Mesh.h"
#include "Square.h"
#include "Camera.h"

int WIN_WIDTH = 1280;
int WIN_HEIGHT = 720;

void PlayerInputs(GLFWwindow*, Square&, float);

void OnResizeWindow(GLFWwindow* window, int w, int h) {
	//WIN_WIDTH = w;
	//WIN_HEIGHT = h;
	glViewport(0, 0, w, h);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Window", NULL, NULL);
	if (!window) {
		std::cout << "ERROR Window " << window << "\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();

	GLFWvidmode mode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode.width - WIN_WIDTH) / 2, (mode.height - WIN_HEIGHT) / 2);

	glfwSetWindowSizeCallback(window, OnResizeWindow);

	Shader defaultShader("default.vert", "default.frag");

	std::vector<Texture> texturesBG = {
		Texture("texture/tile.png", 0)
	};

	std::vector<Texture> textures = {
		Texture("texture/man.png", 0)
	};

	Square BG(2048, 2048, texturesBG, 3);
	Square man(200, 200, textures);

	Camera camera;

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
		camera.transform.position = man.transform.position;
		man.Draw(defaultShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

float playerSpeed = 300;

void PlayerInputs(GLFWwindow* window, Square& player, float dTime) {

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		player.transform.rotation.z += playerSpeed * dTime;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		player.transform.position.y += playerSpeed * dTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		player.transform.position.y -= playerSpeed * dTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player.transform.position.x += playerSpeed * dTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player.transform.position.x -= playerSpeed * dTime;
	}
}