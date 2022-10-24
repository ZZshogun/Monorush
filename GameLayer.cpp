#include "Layer.h"
#include "PlayerController.h"
#include "EnemySpawner.h"
#include "BoxSpawner.h"

GameLayer::GameLayer() {
	scene = Scene::Create();
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary = true;
}

GameLayer::~GameLayer() {
	UI::ClearBuffers();
	Texture::ClearHandles();
	Audio::ClearBuffers();
	scene->Destroy();
}

void GameLayer::OnAttach() {
	Ref<Texture> playerIdletex = Texture::Create("playerIdle", "texture/player_idle_sheet.png");
	Ref<Texture> playerRuntex = Texture::Create("playerRun", "texture/player_run_sheet.png");
	Ref<Texture> enemyTex = Texture::Create("enemy", "texture/enemy.png");
	Ref<Texture> boxtex = Texture::Create("box","texture/box.png");
	Ref<Texture> heart = Texture::Create("heart","texture/heart.png");

	Audio::LoadSound("audio/bounce.wav", "bounce");

	Noise::RandomSeed();

	// Player
	player = scene->CreateEntity("Player");
	player.AddComponent<SpriteRendererComponent>().order = 1;
	auto& anim_player = player.AddComponent<AnimatorComponent>();
	anim_player.AddAnimation(1, playerIdletex, { 2, 1 }, 3);
	anim_player.AddAnimation(2, playerRuntex, { 6, 1 }, 6);
	player.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	player.AddComponent<RigidbodyComponent>();
	auto& player_col = player.AddComponent<CollisionComponent>();
	player_col.size = { 0.35f, 0.9f };
	player_col.drawBox = false;
	player.AddComponent<AudioSourceComponent>();

	// Box Spawner
	boxSpawner = scene->CreateEntity("Box Spawner");
	boxSpawner.AddComponent<NativeScriptComponent>().Bind<BoxSpawner>();

	auto& player_transform = player.GetComponent<TransformComponent>();
	camera.GetComponent<TransformComponent>().parent = &player_transform;
	boxSpawner.GetComponent<TransformComponent>().parent = &player_transform;

	Entity enemySpawner = scene->CreateEntity("Enemy Spawner");
	enemySpawner.AddComponent<NativeScriptComponent>().Bind<EnemySpawner>();

	UI::StartUI(glm::ivec2{ 1920, 1080 });
	UI::Anchor(CENTER);
	menuButton = UI::CreateButton(
		"MENU",
		0.75f,
		Color::White,
		{ -850, 480 },
		{ 160, 75 },
		Color::Black,
		[&]() { state.SceneAddition(-1); },
		Color::Black,
		[]() { UI::on_button->textScale = 1; }
	);
	UI::EndUI();
}

void GameLayer::OnUpdate(Time time) {
	scene->OnUpdate(time);

	if (time_left > 0) {
		time_left -= time.deltaTime;
		time_left = glm::clamp<float>(time_left, 0, INFINITY);
	}
	else if (outoftimeLimit <= 5)  {
		texCol = glm::vec4{ 1, 0.41f, 0.38f, 1 };

		outoftime += time.deltaTime;
		if (outoftime >= outoftimeLimit) {
			floatOffset += time.deltaTime * floatOffsetSpeed;
		}
	}

	UI::StartUI(glm::ivec2{ 1920, 1080 });

	std::string timestr = Time::FormatMinute(time_left);
	std::string millitimestr = "." + Time::FormatMilli(time_left);
	UI::Anchor(CENTER);
	UI::DrawString("- TIME LEFT -", { 0, 500 + floatOffset }, 1.25f, Color::Black);
	UI::Anchor(RIGHT);
	UI::DrawString(timestr, { 50, 430 + floatOffset }, 1.2f, texCol);
	UI::Anchor(LEFT);
	UI::DrawString(millitimestr, { 55, 430 + floatOffset }, 0.60f, texCol);

	UI::Anchor(CENTER);
	UI::DrawButton(menuButton, time);

	UI::EndUI();
}