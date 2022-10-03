
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

float speed = 300;
float squareSize = 320;
glm::vec2 prevOffset{ 0, 0 };
glm::vec2 curOffset{ 0, 0 };

void Start(GameInfo& info) {

	Ref<Texture> BG_tex = Texture::Create("texture/tile.png");
	float maxL = std::max(game.mainCamera.width, game.mainCamera.height);
	BG = Sprite::Create(maxL, maxL, BG_tex, maxL / squareSize);

	Ref<Texture> man_tex = Texture::Create("texture/man.png");
	man = Sprite::Create(100, 100, man_tex);

	Ref<Texture> chest_tex = Texture::Create("texture/chest.png");
	chest = Sprite::Create(100, 100, chest_tex);
	chest->transform.position.x = 220;
}

void Update(GameInfo& info) {

	glm::vec2 dir = Input::WASDAxis();

	dir *= speed * Time::deltaTime;
	if (dir.x && dir.y) dir *= (1 / sqrt(2));

	BG->mesh->texOffset += dir / squareSize;
	man->transform.position += glm::vec3(dir, 0);
	BG->transform.position = man->transform.position;
	game.mainCamera.transform.position = man->transform.position;
}

void Render(GameInfo& info) {
	game.Draw(BG);
	game.Draw(man);
	game.Draw(chest);
}


int main() {
	game.Set(WIN_WIDTH, WIN_HEIGHT);
	game.Run(Start, Update, Render);

	return 0;
}