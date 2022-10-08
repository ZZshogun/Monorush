#ifndef SPRITE_SHEET_CLASS_H
#define SPRITE_SHEET_CLASS_H

#include "Core.h"
#include "Sprite.h"

class SpriteSheet {
	Ref<Material> material;
	Ref<Sprite> sprites;
	glm::vec2 lastIndex = {-1, -1};
	glm::vec2 sizePerIMG = { 0, 0 };
	glm::vec2 IMGcount = { 0, 0 };

public:
	Transform transform;

	SpriteSheet(glm::vec2 sizePerIMG, glm::vec2 IMGcount, Ref<Material>& material);
	static Ref<SpriteSheet> Create(glm::vec2 sizePerIMG, glm::vec2 IMGcount, Ref<Material>& material);
	glm::vec2 size();
};

#endif