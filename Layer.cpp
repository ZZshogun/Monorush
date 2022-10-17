#include "Layer.h"
#include "PlayerController.h"
#include "EnemySpawner.h"

Layer::Layer() {
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary = true;
}

void Layer::OnAttach() {
	Ref<Texture> BGtex = Texture::Create("texture/tile.png");
	Ref<Texture> mantex = Texture::Create("texture/man-sheet.png");
	Ref<Texture> mantex2 = Texture::Create("texture/man-sheet2.png");
	Ref<Texture> chesttex = Texture::Create("texture/chest.png");

	Audio::LoadSound("audio/bounce.wav", "bounce");

	// Background
	Entity BG = scene->CreateEntity("Background");
	auto& BGcomponent = BG.AddComponent<SpriteRendererComponent>();
	float maxL = glm::max(camera.GetComponent<CameraComponent>().resolution);
	BGcomponent.texture = BGtex;
	BGcomponent.size = { maxL, maxL };
	BGcomponent.UVrepeat = 4;
	BGcomponent.parallelTexture = true;
	BGcomponent.order = -1;

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
	BG.GetComponent<TransformComponent>().parent = &man_transform;
	camera.GetComponent<TransformComponent>().parent = &man_transform;

	//Entity enemySpawner = scene->CreateEntity("Enemy Spawner");
	//enemySpawner.AddComponent<NativeScriptComponent>().Bind<EnemySpawner>();
}

void Layer::OnUpdate(Time time) {
	scene->OnUpdate(time);

	UI::StartUI();

	UI::Anchor(CENTER);
	UI::DrawString("Hello World", { 0, 100 }, 1, { 0, 0, 0, 1 });

	UI::EndUI();
}