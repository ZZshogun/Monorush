
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

	defaultShader = std::make_unique<Shader>("default.vert", "default.frag");
	BGshader = std::make_unique<Shader>("BG.vert", "default.frag");

	int maxL = std::max(info.screenHeight, info.screenWidth);

	std::vector<Texture> texturesBG = { Texture("texture/tile.png", 0) };
	BG = std::make_unique<Sprite>(maxL, maxL, texturesBG, 4);

	std::vector<Texture> textures = { Texture("texture/man.png", 0) };
	man = std::make_unique<Sprite>(100, 100, textures);
}

void Update(GameInfo& info) {
	camera.Update(BGshader.get());
	camera.Update(defaultShader.get());

	glm::vec2 dir = glm::vec2(0);
	if (glfwGetKey(info.window, GLFW_KEY_W) == GLFW_PRESS) dir.y += 1;
	if (glfwGetKey(info.window, GLFW_KEY_S) == GLFW_PRESS) dir.y -= 1;
	if (glfwGetKey(info.window, GLFW_KEY_A) == GLFW_PRESS) dir.x -= 1;
	if (glfwGetKey(info.window, GLFW_KEY_D) == GLFW_PRESS) dir.x += 1;

	man->transform.position += glm::vec3(dir * speed * Time::deltaTime, 0);
	BG->transform.position = man->transform.position;
	camera.transform.position = man->transform.position;

	offset += dir * speed * Time::deltaTime;

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