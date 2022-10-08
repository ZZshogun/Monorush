#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core.h"
#include "Sprite.h"
#include "Camera.h"

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
	bool active = true;
	glm::vec2 resolution = { 16, 9 };
};

#endif