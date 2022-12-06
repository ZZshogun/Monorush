#include "../header/Layer.h"
#include "../script/GameManager.h"
#include "../script/PlayerController.h"
#include "../script/EnemySpawner.h"
#include "../script/BoxSpawner.h"

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
	Ref<Texture> playerIdletex = Texture::Create("playerIdleBody", "texture/player_idle_body.png");
	Ref<Texture> playerIdlecapetex = Texture::Create("playerIdleCape", "texture/player_idle_cape.png");
	Ref<Texture> playerRuntex = Texture::Create("playerRunBody", "texture/player_run_body.png");
	Ref<Texture> playerRuncapetex = Texture::Create("playerRunCape", "texture/player_run_cape.png");
	Ref<Texture> enemyTex = Texture::Create("enemy", "texture/enemy.png");
	Ref<Texture> boxtex = Texture::Create("box","texture/box.png");
	Ref<Texture> heart = Texture::Create("heart","texture/heart.png");
	Ref<Texture> bullet = Texture::Create("bullet","texture/bullet.png");
	Ref<Texture> lamppost = Texture::Create("lamppost", "texture/lamp_post.png");
	Ref<Texture> tile = Texture::Create("tile", "texture/tile.png");

	Noise::RandomSeed();

	// Background
	Entity background = scene->CreateEntity("Background");
	auto& bgSprite = background.AddComponent<SpriteRendererComponent>();
	bgSprite.SetTexture(tile);
	bgSprite.UVRepeat(18);
	bgSprite.ScreenSpace(true);
	bgSprite.Color({ 1, 1, 1, 0.2f });
	bgSprite.parallelTexture = true;
	bgSprite.order = -1;
	bgSprite.shader = "unlit-edgefade";

	// Player cape
	Entity playerCape = scene->CreateEntity("Player cape");
	auto& playerCaperenderer = playerCape.AddComponent<SpriteRendererComponent>();
	playerCaperenderer.order = 3;
	auto& anim_player_cape = playerCape.AddComponent<AnimatorComponent>();
	anim_player_cape.AddAnimation(1, playerIdlecapetex, { 2, 1 }, 3);
	anim_player_cape.AddAnimation(2, playerRuncapetex, { 6, 1 }, 6);

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
	playerCape.GetComponent<TransformComponent>().parent = &player_transform;

	UI::StartUI(glm::ivec2{ 1920, 1080 });
	UI::Anchor(CENTER);

	auto OnButtonHover = []() {
		auto& button = UI::on_button;
		button->textScale *= 1.2f;
		button->text = "- " + button->text + " -";
	};

	// Pause screen
	pauseButton = UI::CreateButton(
		"PAUSE",
		0.75f,
		Color::White,
		{ -850, 480 },
		{ 160, 75 },
		Color::Black,
		[&]() { GameManager::pause = true; },
		Color::Black,
		[]() { UI::on_button->textScale = 0.85f; }
	);

	resumeButton = UI::CreateButton(
		"RESUME",
		1.25f,
		Color::Black,
		{ 0, 0 },
		{ 270, 60 },
		{ 0, 1, 0, 1 },
		[&]() { GameManager::pause = false; GameManager::setting = false; },
		Color::White,
		OnButtonHover
	);

	settingButton = UI::CreateButton(
		"SETTING",
		1.25f,
		Color::Black,
		{ 0, -100 },
		{ 280, 60 },
		{ 0, 1, 0, 1 },
		[&]() { GameManager::setting = true; },
		Color::White,
		OnButtonHover
	);

	menuButton = UI::CreateButton(
		"MENU",
		1.25f,
		Color::Black,
		{ 0, -200 },
		{ 190, 60 },
		{ 0, 1, 0, 1 },
		[&]() { state->SceneAddition(-1); },
		Color::White,
		OnButtonHover
	);

	// Setting screen
	fulLScreenButton = UI::CreateButton(
		state->fullScreen ? "SCREEN : FULLSCREEN" : "SCREEN : WINDOWED",
		1.0f,
		Color::Black,
		{ 0, 100 },
		{ 550, 65 },
		{ 0, 1, 0, 1 },
		[&]() {
			state->ToggleFullScreen();
			UI::clicked_button->text = state->fullScreen ? "SCREEN : FULLSCREEN" : "SCREEN : WINDOWED";
		},
		Color::Transparent,
			OnButtonHover
	);

	volumeButton = UI::CreateButton(
		"VOLUME : " + std::to_string((int)(state->volumeGain * 100)) + "%",
		1.0f,
		Color::Black,
		{ 0, 0 },
		{ 400, 65 },
		{ 0, 1, 0, 1 },
		[&]() {
			state->SwitchVolume();
			UI::clicked_button->text = "VOLUME : " + std::to_string((int)(state->volumeGain * 100)) + "%";
		},
		Color::Transparent,
			OnButtonHover
	);

	backButton = UI::CreateButton(
		"BACK",
		0.75f,
		Color::White,
		{ -850, 480 },
		{ 160, 75 },
		Color::Black,
		[&]() { GameManager::setting = false; },
		Color::Black,
		[]() { UI::on_button->textScale = 1.0f; }
	);

	// End screen
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

void GameLayer::OnUpdate(Time& time) {
	GameManager::Update(time);
	if (!state->update) scene->OnUpdate(state, time);

	UI::StartUI(glm::ivec2{ 1920, 1080 });

	UI::Anchor(CENTER);
	UI::DrawButton(pauseButton, time);

	// score display
	if (outoftime <= 5) {
		if (GameManager::remainingTime <= 0) {
			texCol = glm::vec4{ 1, 0.41f, 0.38f, 1 };

			outoftime += time.deltaTime;
			if (outoftime >= outoftimeLimit) {
				floatOffset += time.deltaTime * floatOffsetSpeed;
			}
		}
		std::string scorestr = std::to_string(GameManager::score);
		UI::Anchor(CENTER);
		UI::DrawString("- SCORE -", { 0, 500 + floatOffset }, 1.25f, Color::Black);
		UI::DrawString(scorestr, { 0, 450 + floatOffset }, 1.25f, Color::Black);
	}

	// fury event display
	UI::Anchor(CENTER);
	if (GameManager::fury && GameManager::_furyTimer <= 3) {
		fury_end_time = 0;
		UI::DrawString("FURY HAS STARTED!", { 0, 300 }, 1, { 1, 0.41f, 0.38f, 1 });
	}
	else if (!GameManager::fury && GameManager::half && fury_end_time <= 3.5f) {
		fury_end_time += time.deltaTime;
		UI::DrawString("FURY HAS ENDED... FOR NOW", { 0, 300 }, 1, { 1, 0.41f, 0.38f, 1 });
	}

	if (Input::GetKeyDown(Key::Tab)) GameManager::f3 = !GameManager::f3;
	if (Input::GetKeyDown(Key::Escape)) GameManager::pause = !GameManager::pause;

	// display info
	if (GameManager::f3) {
		std::stringstream ss;
		ss << std::setprecision(2) << std::fixed << GameManager::difficulty;
		UI::Anchor(RIGHT);
		UI::DrawString("DIFFICULTY : " + ss.str(), { 940, 460 }, 0.65f, Color::Black);
		ss.str(std::string());
		ss << std::setprecision(2) << std::fixed << GameManager::difficultyIncRate;
		UI::DrawString("DIFFICULTY RATE : " + ss.str(), { 940, 420 }, 0.65f, Color::Black);
	}
	else {
		std::string timestr = Time::FormatMinute(GameManager::remainingTime);
		std::string millitimestr = "." + Time::FormatMilli(GameManager::remainingTime);
		UI::Anchor(RIGHT);
		UI::DrawString("TIME LEFT " + timestr + millitimestr, { 940, 460 }, 0.65f, Color::Black);
	}

	// gameover
	if (GameManager::gameOver) {
		menuButton->active = false;
		GameManager::pause = false;
		GameManager::setting = false;
		if (resetCountTime >= resetUIWaitTime) {
			resetScreenOpacity += opacityIncSpeed * time.unscaledDeltaTime;
			resetScreenOpacity = glm::min<float>(resetScreenOpacity, 1);
		}
		else resetCountTime += time.unscaledDeltaTime;

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
					UI::DrawString("THANKS FOR PLAYING!", {0, 150}, 0.85f, Color::White);
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

			UI::DrawString("TOTAL SCORE", { 0, 0 }, 0.85f, Color::White);
			UI::DrawString(std::to_string(GameManager::score), { 0, -50 }, 0.85f, Color::White);
			UI::DrawButton(retryButton, time);
			UI::DrawButton(endMenuButton, time);
		}
	}

	UI::Anchor(CENTER);
	if (GameManager::pause) {
		if (GameManager::setting) {
			UI::DrawImage({ 0, 0 }, { 1920, 1080 }, Color::White);
			UI::DrawButton(backButton, time);
			UI::DrawButton(fulLScreenButton, time);
			UI::DrawButton(volumeButton, time);
			UI::DrawString("== SETTING ==", { 0, 400 }, 1.5f, Color::Black);
		}
		else {
			time.timeScale = 0;
			pauseButton->active = false;

			UI::DrawImage({ 0, 0 }, { 1920, 1080 }, { 0, 0, 0, 0.6f });
			UI::DrawImage({ 0, 0 }, { 600, 540 }, Color::White);

			UI::DrawString("PAUSED", { 0, 150 }, 2, Color::Black);

			UI::DrawButton(resumeButton, time);
			UI::DrawButton(settingButton, time);
			UI::DrawButton(menuButton, time);
		}
	}
	if (!GameManager::pause && !GameManager::setting && time.timeScale == 0) {
		time.timeScale = 1;
		pauseButton->active = true;
	}
	else if (!GameManager::pause && GameManager::setting) {
		time.timeScale = 1;
		GameManager::setting = false;
		pauseButton->active = true;
	}

	UI::EndUI();
}