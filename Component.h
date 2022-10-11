#ifndef COMPONENT_H
#define COMPONENT_H

#include "Magia.h"

class ScriptableEntity;

struct TagComponent {
	std::string tag;
};

struct TransformComponent {
	TransformComponent* parent = NULL;
	glm::vec3 position = {0, 0, 0 };
	glm::vec3 rotation = { 0, 0, 0 };
	glm::vec3 scale = { 1, 1, 1 };
};

struct SpriteRendererComponent {
	Ref<VAO> handle;
	Ref<Material> material;
	glm::vec2 size = {1, 1};
	bool parallelTexture = false;
	glm::vec2 textureOffset = { 0, 0 };
	float UVrepeat = 1;
};

struct CameraComponent {
	glm::vec2 resolution = { 16, 9 };
	bool primary = false;
};

struct CollisionComponent {
	bool collision = true;
	glm::vec2 origin = { 0, 0 };
	glm::vec2 size = { 1, 1 };
};

struct RigidbodyComponent {
	bool simulate = true;
	glm::vec3 position = { 0, 0, 0 };
	glm::vec3 velocity = { 0, 0, 0 };
};

struct NativeScriptComponent {
	ScriptableEntity* instance = NULL;

	ScriptableEntity*(*InstantiateScript)();
	void(*DestroyScript)(NativeScriptComponent*);

	template<typename T>
	void Bind() {
		InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		DestroyScript = [](NativeScriptComponent* script) { delete script->instance; script->instance = NULL; };
	
		InstantiateScript();
	}

	template<typename T>
	auto Get() {
		return (static_cast<T*>(instance));
	}
};

#endif