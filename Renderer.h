#ifndef RENDERER_H
#define RENDERER_H

#include "Core.h"
#include "Sprite.h"
#include "Transform.h"

class Renderer {

	Renderer();

public:

	static void DrawSprite(
		Ref<VAO>& handle,
		glm::vec2 size,
		Transform transform,
		Ref<Material>& material,
		glm::vec2 texOffset = { 0, 0 },
		float UVrepeat = 1
	);

};

#endif
