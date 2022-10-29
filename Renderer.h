#ifndef RENDERER_H
#define RENDERER_H

#include "Magia.h"
#include "Component.h"

class Renderer {

	Renderer();

public:

	static void DrawSprite(
		Ref<VAO>& handle,
		TransformComponent transform,
		Ref<Material>& material,
		glm::vec2 texOffset = { 0, 0 }
	);

};

#endif
