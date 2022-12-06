#include "../header/Scene.h"

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

void Scene::OnUpdate(Ref<LayerState>& layerState, Time& time) {
	if (!layerState) return;

	// Rigidbody
	auto rigidGroup =
		scene_registry.group(entt::get<RigidbodyComponent, CollisionComponent, TransformComponent, TagComponent>);
	for (auto entity : rigidGroup) {
		auto [transform, rigidbody, collision, tag] =
			rigidGroup.get<TransformComponent, RigidbodyComponent, CollisionComponent, TagComponent>(entity);
		rigidbody.position = transform.position;

		if (!rigidbody.active || !tag.active) continue;

		rigidbody.position += rigidbody.velocity * time.deltaTime;
		transform.position = rigidbody.position;
		Collision::Update(entity, scene_registry);

		glm::vec2 push_dpos = { 0, 0 };
		if (collision.active) {
			CollisionPacket& packet = Collision::Check(entity, scene_registry);
			for (auto& box : packet.boxes) {
				glm::vec2 push = box.normal * box.depth;
				auto box_rb = box.entity.TryGetComponent<RigidbodyComponent>();

				if (box_rb && !box_rb->isStatic) {
					float totalMass = rigidbody.mass + box_rb->mass;
					box_rb->position -= glm::vec3(push * (rigidbody.mass / totalMass), 0);
					push *= (box_rb->mass / totalMass);
				}

				if (glm::abs(push_dpos.x) < glm::abs(push.x)) push_dpos.x = push.x;
				if (glm::abs(push_dpos.y) < glm::abs(push.y)) push_dpos.y = push.y;
			}
		}

		rigidbody.position += glm::vec3(push_dpos, 0);
		transform.position = rigidbody.position;
	}

	// Native Script Component
	scene_registry.view<NativeScriptComponent>().each([=](auto entity, auto& script) {
		if (script.active && scene_registry.get<TagComponent>(entity).active) {
			if (!script.instance) {
				script.instance = script.InstantiateScript();
				script.initialized = false;
			}
			if (!script.initialized) {
				script.initialized = true;
				script.instance->entity = Entity{ entity, &scene_registry };
				script.instance->OnCreate();
			}
			if (!Collision::Check(entity, scene_registry).empty)
				script.instance->OnCollision(Collision::Check(entity, scene_registry), time);
			if (scene_registry.valid(entity)) script.instance->OnUpdate(time);
		}
		});

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
	entt::entity camera;
	auto cameraGroup = scene_registry.group(entt::get<CameraComponent, TransformComponent>);
	for (auto entity : cameraGroup) {
		auto [transform, cameraC] =
			cameraGroup.get<TransformComponent, CameraComponent>(entity);
		if (cameraC.active && cameraC.primary && scene_registry.get<TagComponent>(entity).active) {

			cameraC.resolution = layerState->resolution;
			for (auto& it : Shader::LUT) {
				glm::mat4 projection = glm::mat4(1.0f);
				glm::mat4 view = glm::mat4(1.0f);

				float aspectRatio = cameraC.resolution.x / cameraC.resolution.y * cameraC.orthographicSize;
				projection = glm::ortho<float>(
					-aspectRatio,
					aspectRatio,
					-cameraC.orthographicSize,
					cameraC.orthographicSize,
					-1000,
					1000
				);

				view = glm::translate(view, -transform.position);
				view = glm::rotate(view, glm::radians(-transform.rotation.x), glm::vec3(1, 0, 0));
				view = glm::rotate(view, glm::radians(-transform.rotation.y), glm::vec3(0, 1, 0));
				view = glm::rotate(view, glm::radians(-transform.rotation.z), glm::vec3(0, 0, 1));

				if (it.first == "unlit" || it.first == "unlit-edgefade") {
					it.second->UniformMat4("proj", projection);
					it.second->UniformMat4("view", view);
					if (it.first == "unlit-edgefade") {
						it.second->UniformFloat("intensity", 0.1f);
						it.second->UniformVec2("viewport", cameraC.resolution);

						glm::ivec2 fullViewport = { 1280, 720 };
						glfwGetWindowSize(layerState->window, &fullViewport.x, &fullViewport.y);
						it.second->UniformVec2("viewportOffset", (glm::vec2(fullViewport) - cameraC.resolution) / 2.0f);
					}
				}
			}
		}
		foundCamera = true;
		camera = entity;
		break;
	}
	
	// Audio Sources
	auto audioGroup = 
		scene_registry.group(entt::get<AudioSourceComponent, TransformComponent>);
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
		scene_registry.group(entt::get<AudioListenerComponent, TransformComponent>);
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

		auto& cameraTransform = scene_registry.get<TransformComponent>(camera);
		auto& cameraComponent = scene_registry.get<CameraComponent>(camera);

		// SpriteRenderer
		auto renderGroup = 
			scene_registry.group(entt::get<SpriteRendererComponent, TransformComponent, TagComponent>);
		// Render reordering
		renderGroup.sort<SpriteRendererComponent>(
			[](const SpriteRendererComponent& a, const SpriteRendererComponent& b) {
				return a.order < b.order;
			}
		);
		for (auto entity : renderGroup) {
			auto [sprite, transform, tag] =
				renderGroup.get<SpriteRendererComponent, TransformComponent, TagComponent>(entity);
			
			if (!sprite.active || !tag.active) continue;

			glm::vec3 cameraPos = cameraTransform.position;
			glm::vec3 cameraViewDist = glm::vec3(
				glm::vec2{ cameraComponent.aspectRatio(), 1 } * cameraComponent.orthographicSize * 2.0f +
				sprite.Size() * glm::abs(glm::vec2(transform.scale)), 0);

			if (!Math::InBox2(transform.position, cameraPos, cameraViewDist))
				continue;

			if (sprite.parallelTexture)
				sprite.textureOffset = glm::vec2(transform.position) / sprite.Size() * sprite.UVRepeat();

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
					Sprite::Resize(sprite.Pointer(), sprite.Data(), sprite.Size(), start, end, glm::bvec2{ sprite.flipX, sprite.flipY });
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
					Sprite::Resize(sprite.Pointer(), sprite.Data(), sprite.Size(), start, end, glm::bvec2{ sprite.flipX, sprite.flipY });
				}
			}
			// Default resizing
			else if (sprite.UpdateRequired()) {
				if (sprite.ScreenSpace()) sprite.Size({ 24, 24 });
				Sprite::Resize(sprite.Pointer(), sprite.Data(), sprite.Size(), sprite.UVRepeat(), glm::bvec2{ sprite.flipX, sprite.flipY });
			}

			// Collsion box visualization
			CollisionComponent* collision = scene_registry.try_get<CollisionComponent>(entity);
			if (collision && collision->DrawBox()) {
				TransformComponent col_t = transform;
				col_t.position += glm::vec3(collision->Origin(), 0);

				if (collision->UpdateRequired())
					Sprite::Resize(collision->Pointer(), collision->Data(), collision->Size());

				Renderer::DrawSprite(
					collision->Pointer(),
					col_t,
					collision->GetMaterial(),
					{ 0, 0 }
				);
			}

			// Draw
			material->shader = Shader::LUT[sprite.shader];
			Renderer::DrawSprite(
				sprite.Pointer(),
				transform,
				material,
				sprite.textureOffset
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