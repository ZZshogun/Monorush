#ifndef COMPONENT_H
#define COMPONENT_H

#include "Magia.h"

class ScriptableEntity;

struct TagComponent {
	std::string tag;
	bool active = true;
};

struct TransformComponent {
	TransformComponent* parent = NULL;
	glm::vec3 position = {0, 0, 0 };
	glm::vec3 rotation = { 0, 0, 0 };
	glm::vec3 scale = { 1, 1, 1 };
};

struct SpriteRendererComponent {
	bool active = true;
	int order = 0;
	bool parallelTexture = false;

public:
	void SetTexture(Ref<Texture>& texture) { this->texture = texture; update = true; }
	void Albedo(glm::vec4 color) {
		if (albedo == color) return;
		albedo = color;
		update = true;
	}
	void Size(glm::vec2 size) { 
		if (this->size == size) return;
		this->size = size; 
		update = true; 
	}
	void TextureOffset(glm::vec2 offset) { 
		if (textureOffset == offset) return;
		textureOffset = offset; update = true; 
	}
	void UVRepeat(float count) { 
		if (UVrepeat == count) return;
		UVrepeat = count; 
		update = true; 
	}
	
	bool UpdateRequired() { return update; }
	Ref<Texture>& GetTexture() { return texture; }
	glm::vec4 Albedo() { return albedo; }
	glm::vec2 Size() { return size; }
	glm::vec2 TextureOffset() { return textureOffset; }
	float UVRepeat() { return UVrepeat; }
	Ref<VAO>& Pointer() { return handle; }
	Ref<VBO>& Data() { return data; }

private:
	bool update = false;
	Ref<VAO> handle;
	Ref<VBO> data;
	Ref<Texture> texture;
	glm::vec4 albedo = { 1, 1, 1, 1 };
	glm::vec2 size = { 1, 1 };
	glm::vec2 textureOffset = { 0, 0 };
	float UVrepeat = 1;
};

struct CameraComponent {
	bool active = true;
	glm::vec2 resolution = { 16, 9 };
	bool primary = false;
};

struct AudioListenerComponent {
	bool active = true;
	bool listening = true;
};

struct AudioSourceComponent {
	bool active = true;
	ALuint source{};

	float gain = 1;
	float pitch = 1;
	bool loop = false;

	float rolloff = 1.15f;
	float reference = 3;
	float maxDistance = AL_MAX_DISTANCE;

	void Play(Ref<AudioBuffer>& sound) {
		AudioSource::Play(source, sound);
	}
	void Pause() {
		AudioSource::Pause(source);
	}
	void Resume() {
		AudioSource::Resume(source);
	}
	void Stop() {
		AudioSource::Stop(source);
	}
	void Restart(Ref<AudioBuffer>& sound) {
		Stop();
		Play(sound);
	}
};

struct CollisionComponent {
	bool active = true;

public:
	void DrawBox(bool status) {
		if (drawBox == status) return;
		drawBox = status;
		update = true;
	}
	void Origin(glm::vec2 position) {
		if (origin == position) return;
		origin = position;
		update = true;
	}
	void Size(glm::vec2 size) {
		if (this->size == size) return;
		this->size = size;
		update = true;
	}

	bool UpdateRequired() { return update; }
	bool DrawBox() { return drawBox; }
	glm::vec2 Origin() { return origin; }
	glm::vec2 Size() { return size; }
	Ref<VAO>& Pointer() { return handle; }
	Ref<VBO>& Data() { return data; }
	Ref<Material>& GetMaterial() { return material; }

private:
	bool update = false;
	bool drawBox = false;
	Ref<VAO> handle;
	Ref<VBO> data;
	Ref<Material> material;
	glm::vec2 origin = { 0, 0 };
	glm::vec2 size = { 1, 1 };
};

struct RigidbodyComponent {
	bool active = true;
	bool isStatic = false;
	float mass = 1;
	glm::vec3 position = { 0, 0, 0 };
	glm::vec3 velocity = { 0, 0, 0 };
};

struct SpriteSheetComponent {
	bool active = true;

public:
	void SetSpriteSheet(Ref<Texture>& textureSheet) { sheet = textureSheet; update = true; }
	void Size(glm::vec2 size) { 
		if (this->size == size) return;
		this->size = size; 
		update = true; 
	}
	void SizePerSprite(int unit) { 
		if (this->sizePerSprite == unit) return;
		this->sizePerSprite = unit; 
		update = true; 
	}
	void DrawAtIndex(int index) { 
		if (drawIndex == index) return;
		drawIndex = index; 
		update = true; 
	}

	bool UpdateRequired() { return update; }
	Ref<Texture>& SpriteSheet() { return sheet; }
	int DrawAtIndex() { return drawIndex; }
	int SizePerSprite() { return sizePerSprite; }
	glm::vec2 Size() { return size; }

private:
	bool update = false;
	glm::vec2 size = { 1, 1 };
	int sizePerSprite = 1;
	int drawIndex = 0;
	Ref<Texture> sheet;
};

struct AnimatorComponent {
	bool active = true;

	struct AnimatorObject {
		float _currentTime = 0;

	public:
		void AnimationSheet(Ref<Texture>& animationSheet) { animation = animationSheet; update = true; }
		void Size(glm::vec2 size) { 
			if (this->size == size) return;
			this->size = size; 
			update = true; 
		}
		void SizePerSprite(int sizePerSprite) { 
			if (this->sizePerSprite == sizePerSprite) return;
			this->sizePerSprite = sizePerSprite; 
			update = true; 
		}
		void FPS(int fps) { 
			if (this->fps == fps) return;
			this->fps = fps; 
			update = true; 
		}
		void DrawAtIndex(int index) { 
			if (drawIndex == index) return;
			drawIndex = index; 
			update = true; 
		}

		bool UpdateRequired() { return update; }
		Ref<Texture>& AnimationSheet() { return animation; }
		glm::vec2 Size() { return size; }
		int SizePerSprite() { return sizePerSprite; }
		int FPS() { return fps; }
		int DrawAtIndex() { return drawIndex; }

	private:
		bool update = false;
		Ref<Texture> animation;
		glm::vec2 size = { 1, 1 };
		int sizePerSprite = 1;
		int fps = 5;
		int drawIndex = 0;
	};

public:
	void SetAnimationIndex(int index) { 
		if (current_id == index) return;
		current_id = index; 
		update = true; 
	}

	void AddAnimation(int id, AnimatorObject& object) {
		if (current_id == INT_MAX) current_id = id;
		animation_map[id] = object;
	}

	void AddAnimation(int id, Ref<Texture>& texture, glm::vec2 size, int fps, int sizePerSprite = 1) {
		AnimatorObject ani;
		ani.AnimationSheet(texture);
		ani.Size(size);
		ani.FPS(fps);
		ani.SizePerSprite(sizePerSprite);
		ani.DrawAtIndex(0);
		AddAnimation(id, ani);
	}

	AnimatorObject& GetAnimation(int id) { return animation_map[id]; }
	AnimatorObject& GetCurrentAnimation() { return animation_map[current_id]; }
	int GetCurrentIndex() { return current_id; }

	bool UpdateRequired() { return update; }

private:
	bool update = false;
	std::map<int, AnimatorObject> animation_map;
	int current_id = INT_MAX;
};

struct NativeScriptComponent {
	bool active = true;
	ScriptableEntity* instance = NULL;

	ScriptableEntity*(*InstantiateScript)() = NULL;
	void(*DestroyScript)(NativeScriptComponent*) = NULL;

	template<typename T>
	void Bind() {
		InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		DestroyScript = [](NativeScriptComponent* script) { delete script->instance; script->instance = NULL; };
	
		InstantiateScript();
	}
};

#endif