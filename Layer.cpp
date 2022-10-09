#include "Layer.h"

Layer::Layer() {
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>().primary;
}

void Layer::OnAttach() {
	Ref<Texture> BGtex = Texture::Create("texture/tile.png");
	Ref<Material> BGmat = Material::Create(BGtex);

	Ref<Texture> mantex = Texture::Create("texture/man.png");
	Ref<Material> manmat = Material::Create(mantex);

	man = scene->CreateEntity("Man");
	man.AddComponent<SpriteRendererComponent>().material = manmat;

	BG = scene->CreateEntity("Background");
	auto& BGcomponent = BG.AddComponent<SpriteRendererComponent>();
	float maxL = glm::max(camera.GetComponent<CameraComponent>().resolution);
	BGcomponent.material = BGmat;
	BGcomponent.size = { maxL, maxL };
	BGcomponent.UVrepeat = 4;
}

void Layer::OnUpdate(Time time) {
	scene->OnUpdate(time);
}