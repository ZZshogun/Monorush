#include "Layer.h"
#include "GameManager.h"
#include "PlayerController.h"
#include "EnemySpawner.h"
#include "BoxSpawner.h"

GameLayer::GameLayer() {
	scene = Scene::Create();
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary = true;
}

GameLayer::~GameLayer() {
	GameManager::Reset();
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
	player.GetComponent<TagComponent>().tag = "Player";
	player.AddComponent<SpriteRendererComponent>().order = 2;
	auto& anim_player = player.AddComponent<AnimatorComponent>();
	anim_player.AddAnimation(1, playerIdletex, { 2, 1 }, 3);
	anim_player.AddAnimation(2, playerRuntex, { 6, 1 }, 6);
	player.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	player.AddComponent<RigidbodyComponent>();
	auto& player_col = player.AddComponent<CollisionComponent>();
	player_col.Size({0.35f, 0.9f});
	player_col.DrawBox(false);
	player.AddComponent<AudioSourceComponent>();

	// Box Spawner
	boxSpawner = scene->CreateEntity("Box Spawner");
	boxSpawner.AddComponent<NativeScriptComponent>().Bind<BoxSpawner>();

	// Enemy Spawner
	Entity enemySpawner = scene->CreateEntity("Enemy Spawner");
	enemySpawner.AddComponent<NativeScriptComponent>().Bind<EnemySpawner>();

	auto& player_transform = player.GetComponent<TransformComponent>();
	camera.GetComponent<TransformComponent>().parent = &player_transform;
	boxSpawner.GetComponent<TransformComponent>().parent = &player_transform;
	enemySpawner.GetComponent<TransformComponent>().parent = &player_transform;

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

PlayerController* playerController = NULL;
glm::vec4 texCol = Color::Black;
float outoftimeLimit = 1.3f, outoftime = 0;
float floatOffsetSpeed = 250, floatOffset = 0;
float opacityIncSpeed = 3, resetScreenOpacity = 0;
float resetUIWaitTime = 0.67f, resetCountTime = 0;
float fury_end_time = 0;

void GameLayer::OnStart() {
	Time time;
	scene->OnUpdate(time);

	playerController = &player.GetScript<PlayerController>();

}

void GameLayer::OnUpdate(Time time) {
	GameManager::Update(time);
	scene->OnUpdate(time);

	UI::StartUI(glm::ivec2{ 1920, 1080 });
	UI::Anchor(CENTER);
	UI::DrawButton(menuButton, time);
	UI::EndUI();

	if (outoftime <= 5) {
		if (GameManager::remainingTime <= 0) {
			texCol = glm::vec4{ 1, 0.41f, 0.38f, 1 };

			outoftime += time.deltaTime;
			if (outoftime >= outoftimeLimit) {
				floatOffset += time.deltaTime * floatOffsetSpeed;
			}
		}

		UI::StartUI(glm::ivec2{ 1920, 1080 });

		std::string timestr = Time::FormatMinute(GameManager::remainingTime);
		std::string millitimestr = "." + Time::FormatMilli(GameManager::remainingTime);
		UI::Anchor(CENTER);
		UI::DrawString("- TIME LEFT -", { 0, 500 + floatOffset }, 1.25f, Color::Black);
		UI::Anchor(RIGHT);
		UI::DrawString(timestr, { 50, 430 + floatOffset }, 1.2f, texCol);
		UI::Anchor(LEFT);
		UI::DrawString(millitimestr, { 55, 440 + floatOffset }, 0.60f, texCol);

		UI::EndUI();

	}

	UI::StartUI(glm::ivec2{ 1920, 1080 });

	UI::Anchor(CENTER);
	if (GameManager::fury && GameManager::_furyTimer <= 3) {
		fury_end_time = 0;
		UI::DrawString("FURY HAS STARTED!", { 0, 300 }, 1, { 1, 0.41f, 0.38f, 1 });
	}
	else if (!GameManager::fury && GameManager::half && fury_end_time <= 3.5f) {
		fury_end_time += time.deltaTime;
		UI::DrawString("FURY HAS ENDED... FOR NOW", { 0, 300 }, 1, { 1, 0.41f, 0.38f, 1 });
	}

	std::stringstream ss;
	ss << std::setprecision(2) << std::fixed << GameManager::difficulty;
	UI::Anchor(RIGHT);
	UI::DrawString("DIFFICULTY : " + ss.str(), {940, 460}, 0.65f, Color::Black);
	ss.str(std::string());
	ss << std::setprecision(2) << std::fixed << GameManager::difficultyIncRate;
	UI::DrawString("DIFFICULTY RATE : " + ss.str(), {940, 420}, 0.65f, Color::Black);

	if (GameManager::gameOver) {
		menuButton->active = false;
		if (resetCountTime >= resetUIWaitTime) {
			resetScreenOpacity += opacityIncSpeed * time.deltaTime;
			resetScreenOpacity = glm::min<float>(resetScreenOpacity, 1);
		}
		else resetCountTime += time.deltaTime;

		UI::Anchor(CENTER);
		UI::DrawImage({ 0, 0 }, { 1920, 1080 }, { 0, 0, 0, resetScreenOpacity * 0.75f });
	}

	UI::EndUI();
}