
#include "Game.h"

void Start(GameInfo& info);
void Update(GameInfo& info);
void Render(GameInfo& info);

Game game;
Ref<Sprite> BG;
Ref<AnimatedSprite> man;
Ref<Sprite> chest;

float speed = 4;
float squareSize = 4;
int anim = 0;

void Start(GameInfo& info) {

	game.mainCamera.SetBackgroundMode(CameraBackgroundMode::TexBackground);
	Ref<Texture> BG_tex = Texture::Create("texture/tile.png");
	Material BG_mat = Material::Create(Shader::Get("unlit"), BG_tex);
	game.mainCamera.SetBackground(BG_mat, squareSize);

	Ref<Texture> man_tex = Texture::Create("texture/man-sheet.png");
	Ref<Texture> man_tex2 = Texture::Create("texture/man-sheet2.png");
	Material man_mat = Material::Create(Shader::Get("unlit"), man_tex);
	Material man_mat2 = Material::Create(Shader::Get("unlit"), man_tex2);
	Ref<SpriteSheet> man_sheet = SpriteSheet::Create({ 1, 1 }, { 2, 1 }, man_mat);
	Ref<SpriteSheet> man_sheet2 = SpriteSheet::Create({ 1, 1 }, { 2, 1 }, man_mat2);
	man = AnimatedSprite::Create(480);
	man->Add(man_sheet);
	man->Add(man_sheet2);

	Ref<Texture> chest_tex = Texture::Create("texture/chest.png");
	Material chest_mat = Material::Create(Shader::Get("unlit"), chest_tex);
	chest = Sprite::Create({ 1, 1 }, chest_mat);
	chest->transform.position.x = 2;
}

void Update(GameInfo& info) {

	glm::vec2 dir = Input::WASDAxis();

	dir *= speed * Time::deltaTime;
	if (dir.x && dir.y) dir *= (1 / sqrt(2));

	if (Input::GetMouseDown(GLFW_MOUSE_BUTTON_LEFT)) anim = 0;
	if (Input::GetMouseDown(GLFW_MOUSE_BUTTON_RIGHT)) anim = 1;

	man->transform.position += glm::vec3(dir, 0);
	game.mainCamera.transform.position = man->transform.position;
	game.mainCamera.OffsetBackground(dir / squareSize);
}

void Render(GameInfo& info) {
	man->Draw(anim, Time::deltaTime);
	chest->Draw();
}

int main() {
	game.Set({ 1600, 900 });
	game.Run(Start, Update, Render);

	return 0;
}