#include "Scene.h"

void SpriteCreate(entt::registry& registry, entt::entity entity) {
	auto& sprite = registry.get<SpriteRendererComponent>(entity);
	if (!sprite.material.get()) sprite.material = Material::Create();
	sprite.handle = VAO::Create();
	Sprite::Create(sprite.handle, sprite.size, sprite.material);
}

Scene::Scene() {
	this->scene_registry.on_construct<SpriteRendererComponent>().connect<&SpriteCreate>();
}

Ref<Scene> Scene::Create() {
	return std::make_shared<Scene>();
}

Entity Scene::CreateEntity(std::string name) {
	Entity entity = { this->scene_registry.create(), &(this->scene_registry) };
	entity.AddComponent<TransformComponent>();
	entity.name = name.empty() ? "Unnamed Entity" : name;
	return entity;
}

void Scene::OnUpdate(float deltaTime) {

	auto cameraView = this->scene_registry.view<CameraComponent>();
	for (entt::entity entity : cameraView) {
		CameraComponent& camera = cameraView.get<CameraComponent>(entity);
		if (!camera.active) continue;
		TransformComponent& transform = this->scene_registry.get<TransformComponent>(entity);
		Transform t = { transform.position, transform.rotation, transform.scale };
		for (auto it = Shader::Begin(); it != Shader::End(); it++) {
			Camera::Update(t, camera.resolution, it->second);
		}
	}

	auto renderView = this->scene_registry.view<SpriteRendererComponent>();
	for (entt::entity entity : renderView) {
		SpriteRendererComponent& sprite = renderView.get<SpriteRendererComponent>(entity);
		TransformComponent& transformC = this->scene_registry.get<TransformComponent>(entity);
		Transform transform = { transformC.position, transformC.rotation, transformC.scale };
		Renderer::DrawSprite(
			sprite.handle, 
			sprite.size, 
			transform, 
			sprite.material, 
			sprite.textureOffset,
			sprite.UVrepeat
		);
	}
}