#include "Layer.h"
#include "PlayerController.h"
#include "EnemySpawner.h"

Layer::Layer() {
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary = true;
}

void Layer::OnAttach() {
	Ref<Texture> BGtex = Texture::Create("texture/tile.png");
	Ref<Texture> mantex = Texture::Create("texture/man.png");
	Ref<Texture> chesttex = Texture::Create("texture/chest.png");

	Entity BG = scene->CreateEntity("Background");
	auto& BGcomponent = BG.AddComponent<SpriteRendererComponent>();
	float maxL = glm::max(camera.GetComponent<CameraComponent>().resolution);
	BGcomponent.material = Material::Create(BGtex);
	BGcomponent.size = { maxL, maxL };
	BGcomponent.UVrepeat = 4;
	BGcomponent.parallelTexture = true;

	Entity man = scene->CreateEntity("Man");
	man.AddComponent<SpriteRendererComponent>().material = Material::Create(mantex);
	man.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	auto& man_transform = man.GetComponent<TransformComponent>();

	BG.GetComponent<TransformComponent>().parent = &man_transform;
	camera.GetComponent<TransformComponent>().parent = &man_transform;

	Entity chest = scene->CreateEntity("Chest");
	chest.AddComponent<SpriteRendererComponent>().material = Material::Create(chesttex);
	chest.GetComponent<TransformComponent>().position = { -2, -1, 0 };

	//Entity enemySpawner = scene->CreateEntity("Enemy Spawner");
	//enemySpawner.AddComponent<NativeScriptComponent>().Bind<EnemySpawner>();
}

void Layer::OnUpdate(Time time) {
	scene->OnUpdate(time);
}