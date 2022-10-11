#include "Layer.h"
#include "PlayerController.h"
#include "EnemySpawner.h"

Layer::Layer() {
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary = true;
}

void Layer::OnAttach() {
	Ref<Texture> BGtex = Texture::Create("texture/tile.png");
	Ref<Texture> mantex = Texture::Create("texture/man-sheet2.png");
	Ref<Texture> chesttex = Texture::Create("texture/chest.png");

	Entity BG = scene->CreateEntity("Background");
	auto& BGcomponent = BG.AddComponent<SpriteRendererComponent>();
	float maxL = glm::max(camera.GetComponent<CameraComponent>().resolution);
	BGcomponent.material = Material::Create(BGtex);
	BGcomponent.size = { maxL, maxL };
	BGcomponent.UVrepeat = 4;
	BGcomponent.parallelTexture = true;

	man = scene->CreateEntity("Man");
	man.AddComponent<SpriteRendererComponent>().material = Material::Create(mantex);
	man.AddComponent<SpriteSheetComponent>().size = {2, 1};
	man.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	man.AddComponent<RigidbodyComponent>();
	man.AddComponent<CollisionComponent>().size.x = 0.45f;

	auto& man_transform = man.GetComponent<TransformComponent>();
	BG.GetComponent<TransformComponent>().parent = &man_transform;
	camera.GetComponent<TransformComponent>().parent = &man_transform;

	chest = scene->CreateEntity("Chest");
	chest.AddComponent<SpriteRendererComponent>().material = Material::Create(chesttex);
	chest.GetComponent<TransformComponent>().position = { -2, -1, 0 };
	auto& chest_colld = chest.AddComponent<CollisionComponent>();
	chest_colld.size.y = 0.75f;
	chest_colld.origin.y = -0.05f;

	//Entity enemySpawner = scene->CreateEntity("Enemy Spawner");
	//enemySpawner.AddComponent<NativeScriptComponent>().Bind<EnemySpawner>();
}

float elapsed = 0;
float mElapsed = 0.15f;

void Layer::OnUpdate(Time time) {
	elapsed += time.deltaTime;
	if (elapsed >= mElapsed) {
		elapsed = 0;
		man.GetComponent<SpriteSheetComponent>().drawIndex++;
	}

	scene->OnUpdate(time);
}