#include "Scene.h"

void SpriteCreate(entt::registry& registry, entt::entity entity) {
	auto& sprite = registry.get<SpriteRendererComponent>(entity);
	if (!sprite.material.get()) sprite.material = Material::Create();
	sprite.handle = VAO::Create();
	Sprite::Create(sprite.handle, sprite.size, sprite.material);
}

void SpriteSheetCreate(entt::registry& registry, entt::entity entity) {
	assert(registry.any_of<SpriteRendererComponent>(entity));
}

void CollisionCreate(entt::registry& registry, entt::entity entity) {
	glm::vec3 size = { 1, 1, 0 };
	if (registry.any_of<SpriteRendererComponent>(entity))
		size = glm::vec3(registry.get<SpriteRendererComponent>(entity).size, 0);
	size *= registry.get<TransformComponent>(entity).scale;
	registry.get<CollisionComponent>(entity).size = { size.x, size.y };
}

Scene::Scene() {
	this->scene_registry.on_construct<SpriteRendererComponent>().connect<&SpriteCreate>();
	this->scene_registry.on_construct<SpriteSheetComponent>().connect<&SpriteSheetCreate>();
	this->scene_registry.on_construct<CollisionComponent>().connect<&CollisionCreate>();
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
	
	// Native Script Component
	scene_registry.view<NativeScriptComponent>().each([=](auto entity, auto& script) {
		if (!script.instance) {
			script.instance = script.InstantiateScript();
			script.instance->entity = Entity{ entity, &scene_registry };
			script.instance->OnCreate();
		}
		script.instance->OnUpdate(time);
	});

	// Rigidbody & Collision
	auto rigidGroup = 
		scene_registry.group<RigidbodyComponent>(entt::get<TransformComponent, CollisionComponent>);
	for (entt::entity entity : rigidGroup) {
		auto [transform, rigidbody, collision] = 
			rigidGroup.get<TransformComponent, RigidbodyComponent, CollisionComponent>(entity);
		rigidbody.position = transform.position;

		if (!rigidbody.simulate || !collision.collision) continue;

		glm::vec3 vel = rigidbody.velocity;

		CollisionPacket packet = Collision::Check(entity, scene_registry);
		if (packet.count) {
			for (glm::vec3 other : packet.positions) {
				float dot = glm::dot(glm::vec3{0, 1, 0}, glm::normalize(transform.position - other));
				const float cos45deg = 0.707107f * 1.15f;
				if (glm::abs(dot) > cos45deg) {
					if (dot > 0 && vel.y < 0) vel.y = 0;
					else if (dot < 0 && vel.y > 0) vel.y = 0;
				}
				else {
					if (transform.position.x >= other.x && vel.x < 0) vel.x = 0;
					else if (transform.position.x <= other.x && vel.x > 0) vel.x = 0;
				}
			}
		}

		rigidbody.position += vel * time.deltaTime;
		transform.position = rigidbody.position;
	}

	// Transform parent inheritance
	auto transformView = scene_registry.view<TransformComponent>();
	for (entt::entity entity : transformView) {
		auto& transform = transformView.get<TransformComponent>(entity);
		if (transform.parent) {
			transform.position = transform.parent->position;
			transform.rotation = transform.parent->rotation;
			transform.scale = transform.parent->scale;
		}
	}

	// Camera Update
	bool foundCamera = false;
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
	
	if (foundCamera) {

		// SpriteSheet


		// SpriteRenderer
		auto renderGroup = 
			scene_registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		for (auto it = renderGroup.rbegin(); it != renderGroup.rend(); it++) {
			auto [transformC, sprite] =
				renderGroup.get<TransformComponent, SpriteRendererComponent>(*it);
			
			Transform transform = { transformC.position, transformC.rotation, transformC.scale };

			if (sprite.parallelTexture)
				sprite.textureOffset = glm::vec2{ transform.position.x, transform.position.y } / sprite.UVrepeat;

			if (scene_registry.any_of<SpriteSheetComponent>(*it)) {
				auto& spriteSheet = scene_registry.get<SpriteSheetComponent>(*it);

				if (spriteSheet.drawIndex < 0) spriteSheet.drawIndex = (int)spriteSheet.size.x - 1;
				else spriteSheet.drawIndex %= (int)spriteSheet.size.x;

				float offset = (float)spriteSheet.drawIndex * spriteSheet.sizePerSprite;
				glm::vec2 start = glm::vec2{ offset, 0 } / spriteSheet.size;
				glm::vec2 end = glm::vec2{ offset + spriteSheet.sizePerSprite, 1 } / spriteSheet.size;

				Sprite::Resize(sprite.handle, sprite.size, start, end);
			}
			else {
				Sprite::Resize(sprite.handle, sprite.size, sprite.UVrepeat);
			}

			Renderer::DrawSprite(
				sprite.handle,
				sprite.size,
				transform,
				sprite.material,
				sprite.textureOffset
			);
		}
	}
	
}