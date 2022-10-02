#ifndef SPRITE_CLASS_H
#define SPRITE_CLASS_H

#include "Core.h"
#include "Mesh.h"

class Sprite {

	int width = 128;
	int height = 128;

	std::vector<Vertex> _GetVertices(int UVLimit);
	static std::vector<GLuint> __indices;


public:

	Ref<Mesh> mesh;
	std::vector<Ref<Texture>> textures;
	Transform transform;

	Sprite();
	Sprite(int width, int height, Ref<Texture>& texture, int UVLimit = 1);
	static Ref<Sprite> Create();
	static Ref<Sprite> Create(int width, int height, Ref<Texture>& texture, int UVLimit = 1);

	void Draw(Shader* shader);
};

#endif
