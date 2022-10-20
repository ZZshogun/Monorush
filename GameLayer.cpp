#include "Layer.h"
#include "PlayerController.h"
#include "EnemySpawner.h"

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
	Ref<Texture> mantex = Texture::Create("man", "texture/man-sheet.png");
	Ref<Texture> mantex2 = Texture::Create("man2","texture/man-sheet2.png");
	Ref<Texture> chesttex = Texture::Create("chest","texture/chest.png");
	Ref<Texture> heart = Texture::Create("heart","texture/heart.png");

	Audio::LoadSound("audio/bounce.wav", "bounce");

	// Player
	man = scene->CreateEntity("Man");
	man.AddComponent<SpriteRendererComponent>().order = 1;
	auto& anim_man = man.AddComponent<AnimatorComponent>();
	anim_man.AddAnimation(1, mantex, { 2, 1 }, 3);
	anim_man.AddAnimation(2, mantex2, { 2, 1 }, 3);
	man.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	man.AddComponent<RigidbodyComponent>();
	man.AddComponent<CollisionComponent>().size.x = 0.45f;
	man.AddComponent<AudioSourceComponent>();

	// Chest
	chest = scene->CreateEntity("Chest");
	chest.AddComponent<SpriteRendererComponent>().texture = chesttex;
	chest.GetComponent<TransformComponent>().position = { -2, -1, 0 };
	auto& chest_colld = chest.AddComponent<CollisionComponent>();
	chest_colld.size.y = 0.75f;
	chest_colld.origin.y = -0.05f;

	auto& man_transform = man.GetComponent<TransformComponent>();
	camera.GetComponent<TransformComponent>().parent = &man_transform;

	//Entity enemySpawner = scene->CreateEntity("Enemy Spawner");
	//enemySpawner.AddComponent<NativeScriptComponent>().Bind<EnemySpawner>();

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