#include "Layer.h"
#include "PlayerController.h"

Layer::Layer() {
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary = true;
}

void Layer::OnAttach() {
	Ref<Texture> BGtex = Texture::Create("texture/tile.png");
	Ref<Material> BGmat = Material::Create(BGtex);

	Ref<Texture> mantex = Texture::Create("texture/man.png");
	Ref<Material> manmat = Material::Create(mantex);

	Ref<Texture> chesttex = Texture::Create("texture/chest.png");
	Ref<Material> chestmat = Material::Create(chesttex);

	Entity chest = scene->CreateEntity("Chest");
	chest.AddComponent<SpriteRendererComponent>().material = chestmat;
	chest.GetComponent<TransformComponent>().position = { 2, 2, 0 };

	Entity man = scene->CreateEntity("Man");
	man.AddComponent<SpriteRendererComponent>().material = manmat;
	man.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
	auto& man_transform = man.GetComponent<TransformComponent>();

	Entity BG = scene->CreateEntity("Background");
	auto& BGcomponent = BG.AddComponent<SpriteRendererComponent>();
	float maxL = glm::max(camera.GetComponent<CameraComponent>().resolution);
	BGcomponent.material = BGmat;
	BGcomponent.size = { maxL, maxL };
	BGcomponent.UVrepeat = 4;
	BGcomponent.parallelTexture = true;
	BG.GetComponent<TransformComponent>().parent = &man_transform;

	camera.GetComponent<TransformComponent>().parent = &man_transform;


}

void Layer::OnUpdate(Time time) {
	scene->OnUpdate(time);
}