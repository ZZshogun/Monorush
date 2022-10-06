#ifndef SPRITE_SHEET_CLASS_H
#define SPRITE_SHEET_CLASS_H

#include "Core.h"
#include "Mesh.h"

class SpriteSheet {
	Material material;
	Ref<Mesh> sprites;
	glm::vec2 lastIndex = {-1, -1};
	glm::vec2 sizePerIMG = { 0, 0 };
	glm::vec2 IMGcount = { 0, 0 };

	std::vector<Vertex> _GetVertex(glm::vec2 index);

public:
	Transform transform;

	SpriteSheet(glm::vec2 sizePerIMG, glm::vec2 IMGcount, Material& material);
	static Ref<SpriteSheet> Create(glm::vec2 sizePerIMG, glm::vec2 IMGcount, Material& material);
	void Draw(glm::vec2 index);
};

#endif