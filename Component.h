#ifndef COMPONENT_H
#define COMPONENT_H

#include "Magia.h"
#include "ScriptableEntity.h"

struct TagComponent {
	std::string tag;
};

struct TransformComponent {
	glm::vec3 position = {0, 0, 0 };
	glm::vec3 rotation = { 0, 0, 0 };
	glm::vec3 scale = { 1, 1, 1 };
};

struct SpriteRendererComponent {
	Ref<VAO> handle;
	Ref<Material> material;
	glm::vec2 size = {1, 1};
	glm::vec2 textureOffset = { 0, 0 };
	float UVrepeat = 1;
};

struct CameraComponent {
	glm::vec2 resolution = { 16, 9 };
	bool primary = true;
};

struct NativeScriptComponent {
	ScriptableEntity* instance = NULL;

	ScriptableEntity*(*InstantiateScript)();
	void(*DestroyScript)(NativeScriptComponent*);

	template<typename T>
	void Bind() {
		InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		DestroyScript = [](NativeScriptComponent* script) { delete script->instance; script->instance = NULL; };
	}
};

#endif