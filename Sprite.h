#ifndef SPRITE_CLASS_H
#define SPRITE_CLASS_H

#include "Core.h"
#include "Mesh.h"

class Sprite {

	glm::vec2 size = { 1, 1 };

	std::vector<Vertex> _GetVertices(float UVLimit);
	static std::vector<GLuint> __indices;


public:

	Ref<Mesh> mesh;
	std::vector<Ref<Texture>> textures;
	Transform transform;

	Sprite();
	Sprite(glm::vec2 size, float UVLimit = 1);
	Sprite(glm::vec2 size, Ref<Texture>& texture, float UVLimit = 1);
	static Ref<Sprite> Create();
	static Ref<Sprite> Create(glm::vec2 size, float UVLimit = 1);
	static Ref<Sprite> Create(glm::vec2 size, Ref<Texture>& texture, float UVLimit = 1);
	void SetSpriteSize(glm::vec2 size);

	void Draw(Ref<Shader> shader);
};

#endif
