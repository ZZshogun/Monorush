
#include "Game.h"

void Start(GameInfo& info);
void Update(GameInfo& info);
void Render(GameInfo& info);

int WIN_WIDTH = 1600;
int WIN_HEIGHT = 900;

float playerSpeed = 300;

Game game;
Ref<Sprite> BG;
Ref<Sprite> man;
Ref<Sprite> chest;

float speed = 5;
float squareSize = 4;
glm::vec2 prevOffset{ 0, 0 };
glm::vec2 curOffset{ 0, 0 };

void Start(GameInfo& info) {

	Ref<Texture> BG_tex = Texture::Create("texture/tile.png");
	game.mainCamera.SetBackgroundMode(CameraBackgroundMode::TexBackground);
	game.mainCamera.SetBackground(BG_tex, squareSize);

	Ref<Texture> man_tex = Texture::Create("texture/man.png");
	man = Sprite::Create({ 1, 1 }, man_tex);

	Ref<Texture> chest_tex = Texture::Create("texture/chest.png");
	chest = Sprite::Create({ 1, 1 }, chest_tex);
	chest->transform.position.x = 2;
}

void Update(GameInfo& info) {

	glm::vec2 dir = Input::WASDAxis();

	dir *= speed * Time::deltaTime;
	if (dir.x && dir.y) dir *= (1 / sqrt(2));

	man->transform.position += glm::vec3(dir, 0);
	game.mainCamera.transform.position = man->transform.position;
	game.mainCamera.OffsetBackground(dir / squareSize);
}

void Render(GameInfo& info) {
	game.Draw(man);
	game.Draw(chest);
}

int main() {
	game.Set({ WIN_WIDTH, WIN_HEIGHT });
	game.Run(Start, Update, Render);

	return 0;
}