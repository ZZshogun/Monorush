#include "Layer.h"
#include "GameManager.h"
#include "PlayerController.h"
#include "EnemySpawner.h"
#include "BoxSpawner.h"

GameLayer::GameLayer() {
	state = std::make_shared<LayerState>();

	scene = Scene::Create();
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary = true;
}

GameLayer::~GameLayer() {
	GameManager::Reset();
	UI::ClearBuffers();
	Texture::ClearHandles();
	Audio::ClearBuffers();
	Collision::ClearCollisionData();
	scene->Destroy();
}

void GameLayer::OnAttach() {
	Ref<Texture> playerIdletex = Texture::Create("playerIdle", "texture/player_idle_sheet.png");
	Ref<Texture> playerRuntex = Texture::Create("playerRun", "texture/player_run_sheet.png");
	Ref<Texture> enemyTex = Texture::Create("enemy", "texture/enemy.png");
	Ref<Texture> boxtex = Texture::Create("box","texture/box.png");
	Ref<Texture> heart = Texture::Create("heart","texture/heart.png");
	Ref<Texture> bullet = Texture::Create("bullet","texture/bullet.png");
	Ref<Texture> lamppost = Texture::Create("lamppost", "texture/lamp_post.png");
	Ref<Texture> tile = Texture::Create("tile", "texture/tile.png");

	Audio::LoadSound("audio/bounce.wav", "bounce");

	Noise::RandomSeed();

	// Background
	Entity background = scene->CreateEntity("Background");
	auto& bgSprite = background.AddComponent<SpriteRendererComponent>();
	bgSprite.SetTexture(tile);
	bgSprite.UVRepeat(16);
	bgSprite.ScreenSpace(true);
	bgSprite.Color({ 1, 1, 1, 0.2f });
	bgSprite.parallelTexture = true;
	bgSprite.order = -1;
	bgSprite.shader = "unlit-edgefade";

	// Player
	player = scene->CreateEntity("Player");
	player.GetComponent<TagComponent>().tag = "Player";
	player.AddComponent<SpriteRendererComponent>().order = 2;
	auto& anim_player = player.AddComponent<AnimatorComponent>();
	anim_player.AddAnimation(1, playerIdletex, { 2, 1 }, 3);
	anim_player.AddAnimation(2, playerRuntex, { 6, 1 }, 6);
	player.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	player.AddComponent<RigidbodyComponent>().mass = 30;
	auto& player_col = player.AddComponent<CollisionComponent>();
	player_col.Size({0.35f, 0.92f});
	player_col.DrawBox(false);
	player.AddComponent<AudioSourceComponent>();

	playerController = &player.GetScript<PlayerController>();

	// Box Spawner
	Entity boxSpawner = scene->CreateEntity("Box Spawner");
	boxSpawner.AddComponent<NativeScriptComponent>().Bind<BoxSpawner>();

	// Enemy Spawner
	Entity enemySpawner = scene->CreateEntity("Enemy Spawner");
	enemySpawner.AddComponent<NativeScriptComponent>().Bind<EnemySpawner>();

	auto& player_transform = player.GetComponent<TransformComponent>();
	camera.GetComponent<TransformComponent>().parent = &player_transform;
	boxSpawner.GetComponent<TransformComponent>().parent = &player_transform;
	enemySpawner.GetComponent<TransformComponent>().parent = &player_transform;
	background.GetComponent<TransformComponent>().parent = &player_transform;

	UI::StartUI(glm::ivec2{ 1920, 1080 });
	UI::Anchor(CENTER);
	menuButton = UI::CreateButton(
		"MENU",
		0.75f,
		Color::White,
		{ -850, 480 },
		{ 160, 75 },
		Color::Black,
		[&]() { state->SceneAddition(-1); },
		Color::Black,
		[]() { UI::on_button->textScale = 1; }
	);
	retryButton = UI::CreateButton(
		"RETRY",
		1.5f,
		Color::White,
		{ 0, -300 },
		{ 270, 80 },
		Color::Transparent,
		[&]() { state->ReloadScene(); },
		Color::Transparent,
		[]() { UI::on_button->textScale = 2; }
	);
	endMenuButton = UI::CreateButton(
		"RETURN TO TITLE SCREEN",
		0.8f,
		Color::White,
		{0, -400},
		{530, 75},
		Color::Transparent,
		[&] () { state->SceneAddition(-1); },
		Color::Transparent,
		[]() { UI::on_button->textScale = 0.9f; }
	);
	UI::EndUI();
}

void GameLayer::OnUpdate(Time time) {
	GameManager::Update(time);
	if (!state->update) scene->OnUpdate(state, time);

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
		UI::DrawImage({ 0, 0 }, { 1920, 1080 }, { 0, 0, 0, resetScreenOpacity * 0.9f });
		if (resetScreenOpacity >= 1) {

			switch (GameManager::gameState) {
			case GameManager::WIN:
				{
					std::string winMessage = "YOU WIN!";
					std::string winDesc = "YOU\'VE SURVIVED FOR 180 SECONDS";
					if (playerController) {
						if (playerController->playerHeath == playerController->maxPlayerHealth) {
							winMessage = "UNTOUCHABLE";
							winDesc = "THIS IS JUST TOO EZ";
						}
						if (playerController->playerHeath == 1) {
							winMessage = "CLUTCHED";
							winDesc = "YOU\'VE SURVIVED... BARELY";
						}
					}

					UI::DrawString(winMessage, { 0, 350 }, 2, Color::White);
					UI::DrawString(winDesc, { 0, 280 }, 0.65f, Color::White);
				}
				break;
			case GameManager::LOSS:
				UI::DrawString("DEFEATED", { 0, 350 }, 2, Color::White);
				UI::DrawString("THAT\'S UNFORTUNATE...", { 0, 280 }, 0.65f, Color::White);
				{
					std::string timestr = 
						"YOU\'VE SURVIVED FOR " + 
						std::to_string((int)GameManager::maxRemainingTime - (int)GameManager::remainingTime) + 
						" SECONDS";
					UI::DrawString(timestr, { 0, 150 }, 0.85f, Color::White);
				}
				break;
			default:
				UI::DrawString("HOW CAN THIS BE POSSIBLE?!?", { 0, 350 }, 2, Color::White);
				break;
			}

			UI::DrawButton(retryButton, time);
			UI::DrawButton(endMenuButton, time);
		}
	}

	UI::EndUI();
}