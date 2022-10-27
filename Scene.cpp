#include "Scene.h"

void CameraCreate(entt::registry& registry, entt::entity entity) {
	registry.emplace<AudioListenerComponent>(entity);
}

void SpriteCreate(entt::registry& registry, entt::entity entity) {
	auto& sprite = registry.get<SpriteRendererComponent>(entity);
	if (!sprite.GetTexture()) sprite.SetTexture(Texture::defaultTex);
	sprite.Pointer() = VAO::Create();
	Ref<Material> material = Material::Create(sprite.GetTexture(), sprite.Color());
	Sprite::Create(sprite.Pointer(), sprite.Data(), sprite.Color(), material);
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
		size = glm::vec3(registry.get<SpriteRendererComponent>(entity).Size(), 0);
	size *= registry.get<TransformComponent>(entity).scale;
	auto& col = registry.get<CollisionComponent>(entity);
	col.Size({size.x, size.y});
	col.Pointer() = VAO::Create();
	col.GetMaterial() = Material::Create({0, 1, 0, 1});
	Sprite::Create(col.Pointer(), col.Data(), col.Size(), col.GetMaterial());
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
	auto& tag = entity.AddComponent<TagComponent>();
	tag.name = name.empty() ? "Unnamed Entity" : name;
	tag.tag = "";
	entity.AddComponent<TransformComponent>();
	return entity;
}

void Scene::OnUpdate(Time time) {
	// Native Script Component
	scene_registry.view<NativeScriptComponent>().each([=](auto entity, auto& script) {
		if (script.active && scene_registry.get<TagComponent>(entity).active) {
			if (!script.instance) {
				script.instance = script.InstantiateScript();
				script.instance->entity = Entity{ entity, &scene_registry };
				script.instance->OnCreate();
			}
			script.instance->OnUpdate(time);
		}
	});

	// Rigidbody & Collision
	auto rigidGroup = 
		scene_registry.group<RigidbodyComponent, CollisionComponent>(entt::get<TransformComponent>);
	for (auto entity : rigidGroup) {
		auto [transform, rigidbody, collision] = 
			rigidGroup.get<TransformComponent, RigidbodyComponent, CollisionComponent>(entity);
		rigidbody.position = transform.position;
	
		if (!rigidbody.active || !scene_registry.get<TagComponent>(entity).active) continue;

		rigidbody.position += rigidbody.velocity * time.deltaTime;
		glm::vec2 push_dpos = { 0, 0 };

		if (collision.active) {
			CollisionPacket packet = Collision::Check(entity, scene_registry);
			for (auto& box : packet.boxes) {
				glm::vec2 push = box.normal * box.depth;

				float massMultiplier = 1;
				if (box.rigidbody && !box.rigidbody->isStatic) {
					massMultiplier = box.rigidbody->mass / rigidbody.mass;
					push /= 2.0f;
					box.rigidbody->position -= glm::vec3(push, 0);
				}
				push *= massMultiplier;

				if (glm::abs(push_dpos.x) < glm::abs(push.x)) push_dpos.x = push.x;
				if (glm::abs(push_dpos.y) < glm::abs(push.y)) push_dpos.y = push.y;
			}
		}

		rigidbody.position += glm::vec3(push_dpos, 0);
		transform.position = rigidbody.position;
	}

	// Transform parent inheritance
	auto transformView = scene_registry.view<TransformComponent>();
	for (auto entity : transformView) {
		TagComponent* tag = scene_registry.try_get<TagComponent>(entity);
		if (!tag || !tag->active) continue;
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
		if (camera.active && camera.primary && scene_registry.get<TagComponent>(entity).active) {
			Transform t = { transform.position, transform.rotation, transform.scale };
			for (auto& it : Shader::LUT)
				Camera::Update(t, camera.cameraResolution, it.second);
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
		if (!audioSource.active || !scene_registry.get<TagComponent>(entity).active) continue;

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
		if (!listener.active || !listener.listening || !scene_registry.get<TagComponent>(entity).active) continue;
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
			
			if (!sprite.active || !scene_registry.get<TagComponent>(entity).active) continue;

			Transform transform = { transformC.position, transformC.rotation, transformC.scale };

			if (sprite.parallelTexture)
				sprite.TextureOffset(glm::vec2{ transform.position.x, transform.position.y } / sprite.UVRepeat());

			Ref<Material> material = Material::Create(sprite.GetTexture(), sprite.Color());

			// Sprite sheet resizing
			if (scene_registry.any_of<SpriteSheetComponent>(entity)) {
				auto& spriteSheet = scene_registry.get<SpriteSheetComponent>(entity);
				if (!spriteSheet.SpriteSheet()) continue;

				int sheetIndex = spriteSheet.DrawAtIndex();
				int sheetSizeP = spriteSheet.SizePerSprite();
				glm::vec2 sheetSize = spriteSheet.Size();

				if (sheetIndex < 0) sheetIndex = (int)(sheetSize.x / sheetSizeP) - 1;
				else sheetIndex %= (int)(sheetSize.x / sheetSizeP);

				spriteSheet.DrawAtIndex(sheetIndex);
				material.reset();
				material = Material::Create(spriteSheet.SpriteSheet(), sprite.Color());

				if (spriteSheet.UpdateRequired()) {
					float offset = (float)spriteSheet.DrawAtIndex() * spriteSheet.SizePerSprite();
					glm::vec2 start = glm::vec2{ offset, 0 } / spriteSheet.Size();
					glm::vec2 end = glm::vec2{ offset + spriteSheet.SizePerSprite(), 1 } / spriteSheet.Size();
					Sprite::Resize(sprite.Pointer(), sprite.Data(), sprite.Size(), start, end);
				}
			}
			// Animator resizing
			else if (scene_registry.any_of<AnimatorComponent>(entity)) {
				auto& animator = scene_registry.get<AnimatorComponent>(entity);
				if (animator.GetCurrentIndex() == INT_MAX) continue;
				auto& aniObject = animator.GetCurrentAnimation();

				aniObject._currentTime += time.deltaTime;
				if (aniObject._currentTime >= 1.0f / aniObject.FPS()) {
					aniObject.DrawAtIndex(aniObject.DrawAtIndex() + 1);
					aniObject._currentTime = 0;
				}

				int sheetIndex = aniObject.DrawAtIndex();
				int sheetSizeP = aniObject.SizePerSprite();
				glm::vec2 sheetSize = aniObject.Size();

				if (sheetIndex < 0) sheetIndex = (int)(sheetSize.x / sheetSizeP) - 1;
				else sheetIndex %= (int)(sheetSize.x / sheetSizeP);

				aniObject.DrawAtIndex(sheetIndex);
				material.reset();
				material = Material::Create(aniObject.AnimationSheet(), sprite.Color());

				if (aniObject.UpdateRequired() || animator.UpdateRequired()) {
					float offset = (float)aniObject.DrawAtIndex() * aniObject.SizePerSprite();
					glm::vec2 start = glm::vec2{ offset, 0 } / aniObject.Size();
					glm::vec2 end = glm::vec2{ offset + aniObject.SizePerSprite(), 1 } / aniObject.Size();
					Sprite::Resize(sprite.Pointer(), sprite.Data(), sprite.Size(), start, end);
				}
			}
			// Default resizing
			else if (sprite.UpdateRequired()) {
				Sprite::Resize(sprite.Pointer(), sprite.Data(), sprite.Size(), sprite.UVRepeat());
			}

			// Collsion box visualization
			CollisionComponent* collision = scene_registry.try_get<CollisionComponent>(entity);
			if (collision && collision->DrawBox()) {
				Transform col_t = transform;
				col_t.position += glm::vec3(collision->Origin(), 0);

				if (collision->UpdateRequired())
					Sprite::Resize(collision->Pointer(), collision->Data(), collision->Size(), 1);

				Renderer::DrawSprite(
					collision->Pointer(),
					col_t,
					collision->GetMaterial(),
					{ 0, 0 }
				);
			}

			// Draw
			Renderer::DrawSprite(
				sprite.Pointer(),
				transform,
				material,
				sprite.TextureOffset()
			);
		}

		// Script UI Rendering
		scene_registry.view<NativeScriptComponent>().each([=](auto entity, auto& script) {
			if (script.active && scene_registry.get<TagComponent>(entity).active) {
				if (script.instance)
					script.instance->OnDrawUI(time);
			}
		});
	}	
}

void Scene::Destroy() {
	auto allEntt = scene_registry.view<TagComponent>();
	scene_registry.destroy(allEntt.begin(), allEntt.end());
}