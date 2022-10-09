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
	entity.AddComponent<TagComponent>().tag = name.empty() ? "Unnamed Entity" : name;
	entity.AddComponent<TransformComponent>();
	return entity;
}

void Scene::OnUpdate(Time time) {
	
	bool foundCamera = false;
	{
		auto cameraGroup = scene_registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : cameraGroup) {
			auto [transform, camera] =
				cameraGroup.get<TransformComponent, CameraComponent>(entity);
			if (camera.primary) {
				Transform t = { transform.position, transform.rotation, transform.scale };
				for (auto& it : Shader::LUT)
					Camera::Update(t, camera.resolution, it.second);
				foundCamera = true;
				break;
			}
		}
	}

	{
		scene_registry.view<NativeScriptComponent>().each([=](auto entity, auto& script) {
			if (!script.instance) {
				script.instance = script.InstantiateScript();
				script.instance->entity = Entity{ entity, &scene_registry };
				script.instance->OnCreate();
			}
			script.instance->OnUpdate(time);
		});
	}

	{
		if (foundCamera) {
			auto renderGroup = scene_registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (entt::entity entity : renderGroup) {
				auto [transformC, sprite] =
					renderGroup.get<TransformComponent, SpriteRendererComponent>(entity);
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
	}
}