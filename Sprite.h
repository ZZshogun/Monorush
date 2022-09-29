#ifndef SPRITE_CLASS_H
#define SPRITE_CLASS_H

#include "Mesh.h"

class Sprite {

	int width = 512;
	int height = 512;

	std::vector<Vertex> _GetVertices(int UVLimit);
	static std::vector<GLuint> __indices;


public:

	Mesh* mesh;
	Transform transform;

	Sprite();
	Sprite(int width, int height, std::vector<Texture>& textures, int UVLimit = 1);

	void Draw(Shader* shader);
	~Sprite();
};

#endif
