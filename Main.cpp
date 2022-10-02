
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

float speed = 1;

void Start(GameInfo& info) {

	int maxL = std::max(info.screenHeight, info.screenWidth);
	Ref<Texture> BG_tex = Texture::Create("texture/tile.png", 0);
	BG = Sprite::Create(maxL, maxL, BG_tex, 4);
	Ref<Texture> man_tex = Texture::Create("texture/man.png", 0);
	man = Sprite::Create(100, 100, man_tex);
}

void Update(GameInfo& info) {

	glm::vec2 dir = Input::WASDAxis();

	dir *= speed * Time::deltaTime;
	if (dir.x && dir.y) dir *= (1 / sqrt(2));
	BG->mesh->texOffset += dir;

	man->transform.position += glm::vec3(dir, 0);
	BG->transform.position = man->transform.position;
	game.mainCamera.transform.position = man->transform.position;
}

void Render(GameInfo& info) {
	game.Draw(BG);
	game.Draw(man);
}


int main() {
	game.Set(WIN_WIDTH, WIN_HEIGHT);
	game.Run(Start, Update, Render);

	return 0;
}