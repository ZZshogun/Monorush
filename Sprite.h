#ifndef SPRITE_CLASS_H
#define SPRITE_CLASS_H

#include "Mesh.h"

class Sprite {

	int width = 128;
	int height = 128;

	std::vector<Vertex> _GetVertices(int UVLimit);
	static std::vector<GLuint> __indices;


public:

	Mesh* mesh;
	Texture* texture;
	Transform transform;

	Sprite();
	Sprite(int width, int height, const char* textures, int UVLimit = 1);
	static std::unique_ptr<Sprite> Create();
	static std::unique_ptr<Sprite> Create(int width, int height, const char* texture, int UVLimit = 1);

	void Draw(Shader* shader);
	~Sprite();
};

#endif
