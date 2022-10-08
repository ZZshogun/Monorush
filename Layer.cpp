#include "Layer.h"

Layer::Layer() {
	camera = scene->CreateEntity("Camera");
	camera.AddComponent<CameraComponent>();
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
	BGcomponent.material = BGmat;
	int maxL = glm::max(camera.GetComponent<CameraComponent>().resolution);
	BGcomponent.size = { maxL, maxL };
	BGcomponent.UVrepeat = 4;
}

void Layer::OnUpdate(float deltaTime) {

	// TODO : new delta time
	glm::vec2 dir = Input::WASDAxis();
	dir *= 4 * deltaTime;
	if (dir.x && dir.y) dir /= glm::sqrt(2);

	BG.GetComponent<SpriteRendererComponent>().textureOffset += dir / 4.0f;

	auto& man_pos = man.GetComponent<TransformComponent>().position;
	man_pos += glm::vec3(dir, 0);
	BG.GetComponent<TransformComponent>().position = man_pos;
	camera.GetComponent<TransformComponent>().position = man_pos;

	scene->OnUpdate(deltaTime);
}