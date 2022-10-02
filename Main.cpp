
#include "Game.h"

void Start(GameInfo& info);
void Update(GameInfo& info);
void Render(GameInfo& info);

int WIN_WIDTH = 1600;
int WIN_HEIGHT = 900;

float playerSpeed = 300;

Game game;
std::unique_ptr<Shader> defaultShader;
std::unique_ptr<Shader> BGshader;
std::unique_ptr<Sprite> BG;
std::unique_ptr<Sprite> man;
Camera camera;

glm::vec2 offset = glm::vec2(0);
float speed = 1;

void Start(GameInfo& info) {
	camera.Set(info.screenWidth, info.screenHeight);

	defaultShader = Shader::Create("default.vert", "default.frag");
	BGshader = Shader::Create("BG.vert", "default.frag");

	int maxL = std::max(info.screenHeight, info.screenWidth);
	BG = Sprite::Create(maxL, maxL, "texture/tile.png", 4);
	man = Sprite::Create(100, 100, "texture/man.png");
}

void Update(GameInfo& info) {
	camera.Update(BGshader.get());
	camera.Update(defaultShader.get());

	glm::vec2 dir = glm::vec2(0);
	if (glfwGetKey(info.window, GLFW_KEY_W) == GLFW_PRESS) dir.y += 1;
	if (glfwGetKey(info.window, GLFW_KEY_S) == GLFW_PRESS) dir.y -= 1;
	if (glfwGetKey(info.window, GLFW_KEY_A) == GLFW_PRESS) dir.x -= 1;
	if (glfwGetKey(info.window, GLFW_KEY_D) == GLFW_PRESS) dir.x += 1;

	dir *= speed * Time::deltaTime;
	if (dir.x && dir.y) dir *= (1 / sqrt(2));
	offset += dir;

	man->transform.position += glm::vec3(dir, 0);
	BG->transform.position = man->transform.position;
	camera.transform.position = man->transform.position;
}

void Render(GameInfo& info) {
	BGshader->Bind();
	glUniform2f(glGetUniformLocation(BGshader->handle, "offset"), offset.x, offset.y);
	BG->Draw(BGshader.get());
	man->Draw(defaultShader.get());
}


int main() {
	game.Set(WIN_WIDTH, WIN_HEIGHT);
	game.Run(Start, Update, Render);

	return 0;
}