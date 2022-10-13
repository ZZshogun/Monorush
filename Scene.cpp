#include "Scene.h"

void CameraCreate(entt::registry& registry, entt::entity entity) {
	registry.emplace<AudioListenerComponent>(entity);
}

void SpriteCreate(entt::registry& registry, entt::entity entity) {
	auto& sprite = registry.get<SpriteRendererComponent>(entity);
	if (!sprite.texture) sprite.texture = Texture::defaultTex;
	sprite.handle = VAO::Create();
	Ref<Material> material = Material::Create(sprite.texture, sprite.albedo);
	Sprite::Create(sprite.handle, sprite.size, material);
}

void SpriteSheetCreate(entt::registry& registry, entt::entity entity) {
	assert(registry.any_of<SpriteRendererComponent>(entity));
	assert(!registry.any_of<AnimatorComponent>(entity));
}

void AnimatorCreate(entt::registry& registry, entt::entity entity) {
	assert(registry.any_of<SpriteRendererComponent>(entity));
	assert(!registry.any_of<SpriteSheetComponent>(entity));
}

void CollisionCreate(entt::registry& registry, entt::entity entity) {
	glm::vec3 size = { 1, 1, 0 };
	if (registry.any_of<SpriteRendererComponent>(entity))
		size = glm::vec3(registry.get<SpriteRendererComponent>(entity).size, 0);
	size *= registry.get<TransformComponent>(entity).scale;
	registry.get<CollisionComponent>(entity).size = { size.x, size.y };
}

void AudioSourceCreate(entt::registry& registry, entt::entity entity) {
	auto& audioSource = registry.get<AudioSourceComponent>(entity);
	audioSource.source = AudioSource::Create();
}

Scene::Scene() {
	this->scene_registry.on_construct<CameraComponent>().connect<&CameraCreate>();
	this->scene_registry.on_construct<SpriteRendererComponent>().connect<&SpriteCreate>();
	this->scene_registry.on_construct<SpriteSheetComponent>().connect<&SpriteSheetCreate>();
	this->scene_registry.on_construct<AnimatorComponent>().connect<&AnimatorCreate>();
	this->scene_registry.on_construct<CollisionComponent>().connect<&CollisionCreate>();
	this->scene_registry.on_construct<AudioSourceComponent>().connect<&AudioSourceCreate>();
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
	for (auto entity : rigidGroup) {
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
	for (auto entity : transformView) {
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
	for (auto entity : cameraGroup) {
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
	
	// Audio Sources
	auto audioGroup = 
		scene_registry.group<AudioSourceComponent>(entt::get<TransformComponent>);
	for (auto entity : audioGroup) {
		auto [audioSource, transform] = 
			audioGroup.get<AudioSourceComponent, TransformComponent>(entity);
		if (!audioSource.active) continue;

		AudioSource::SetPosition(audioSource.source, transform.position);
		if (scene_registry.any_of<RigidbodyComponent>(entity)) {
			auto& rigidbody = scene_registry.get<RigidbodyComponent>(entity);
			AudioSource::SetVelocity(audioSource.source, rigidbody.velocity);
		}
		else AudioSource::SetVelocity(audioSource.source, { 0, 0, 0 });

		AudioSource::SetAttenuationBeginDistance(audioSource.source, audioSource.reference);
		AudioSource::SetMaxAttenuationDistance(audioSource.source, audioSource.maxDistance);
		AudioSource::SetRollOffFactor(audioSource.source, audioSource.rolloff);

		AudioSource::SetLooping(audioSource.source, audioSource.loop);
		AudioSource::SetGain(audioSource.source, audioSource.gain);
		AudioSource::SetPitch(audioSource.source, audioSource.pitch);
	}

	// Audio Listener
	auto listenerGroup = 
		scene_registry.group<AudioListenerComponent>(entt::get<TransformComponent>);
	for (auto entity : listenerGroup) {
		auto [listener, transform] = 
			listenerGroup.get<AudioListenerComponent, TransformComponent>(entity);
		if (!listener.listening) continue;
		Audio::SetListenerPosition(transform.position);
		if (scene_registry.any_of<RigidbodyComponent>(entity)) {
			auto& rigidbody = scene_registry.get<RigidbodyComponent>(entity);
			Audio::SetListenerVelocity(rigidbody.velocity);
		}
		else Audio::SetListenerVelocity({0, 0, 0});
	}

	if (foundCamera) {

		// SpriteRenderer
		auto renderGroup = 
			scene_registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		// Render reordering
		renderGroup.sort<SpriteRendererComponent>(
			[](SpriteRendererComponent& a, SpriteRendererComponent& b) {
				return a.order < b.order;
			}
		);
		for (auto entity : renderGroup) {
			auto [transformC, sprite] =
				renderGroup.get<TransformComponent, SpriteRendererComponent>(entity);
			
			Transform transform = { transformC.position, transformC.rotation, transformC.scale };

			if (sprite.parallelTexture)
				sprite.textureOffset = glm::vec2{ transform.position.x, transform.position.y } / sprite.UVrepeat;

			Ref<Material> material;

			// Sprite sheet resizing
			if (scene_registry.any_of<SpriteSheetComponent>(entity)) {
				auto& spriteSheet = scene_registry.get<SpriteSheetComponent>(entity);
				if (!spriteSheet.sheet) continue;

				if (spriteSheet.drawIndex < 0) spriteSheet.drawIndex = (int)spriteSheet.size.x - 1;
				else spriteSheet.drawIndex %= (int)spriteSheet.size.x;

				float offset = (float)spriteSheet.drawIndex * spriteSheet.sizePerSprite;
				glm::vec2 start = glm::vec2{ offset, 0 } / spriteSheet.size;
				glm::vec2 end = glm::vec2{ offset + spriteSheet.sizePerSprite, 1 } / spriteSheet.size;

				material = Material::Create(spriteSheet.sheet, sprite.albedo);
				Sprite::Resize(sprite.handle, sprite.size, start, end);
			}
			// Animator resizing
			else if (scene_registry.any_of<AnimatorComponent>(entity)) {
				auto& animator = scene_registry.get<AnimatorComponent>(entity);
				if (animator.current_id == INT_MAX) continue;
				auto& aniObject = animator.animation_map[animator.current_id];

				if (aniObject.drawIndex < 0) aniObject.drawIndex = (int)aniObject.size.x - 1;
				else aniObject.drawIndex %= (int)aniObject.size.x;

				aniObject._currentTime += time.deltaTime;
				if (aniObject._currentTime >= 1.0f / aniObject.fps) {
					aniObject.drawIndex++;
					aniObject._currentTime = 0;
				}

				float offset = (float)aniObject.drawIndex * aniObject.sizePerSprite;
				glm::vec2 start = glm::vec2{ offset, 0 } / aniObject.size;
				glm::vec2 end = glm::vec2{ offset + aniObject.sizePerSprite, 1 } / aniObject.size;

				material = Material::Create(aniObject.animation, sprite.albedo);
				Sprite::Resize(sprite.handle, sprite.size, start, end);
			}
			// Default resizing
			else {
				material = Material::Create(sprite.texture, sprite.albedo);
				Sprite::Resize(sprite.handle, sprite.size, sprite.UVrepeat);
			}

			Renderer::DrawSprite(
				sprite.handle,
				sprite.size,
				transform,
				material,
				sprite.textureOffset
			);
		}
	}
	
}