#ifndef SQUARE_CLASS_H
#define SQUARE_CLASS_H

#include "Mesh.h"

class Square {

	int width;
	int height;

public:

	Mesh mesh;
	Transform transform;

	Square(int width, int height, std::vector<Texture>& textures, int UVLimit = 1);

	void Draw(Shader& shader);
};

#endif
