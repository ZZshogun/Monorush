#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(glm::vec2 sizePerIMG, glm::vec2 IMGcount, Ref<Material>& material) {
	this->sizePerIMG = sizePerIMG;
	this->IMGcount = IMGcount;
	this->material = material;

	//this->sprites = Sprite::Create(IMGcount, material);
}

Ref<SpriteSheet> SpriteSheet::Create(glm::vec2 sizePerIMG, glm::vec2 IMGcount, Ref<Material>& material) {
	return std::make_shared<SpriteSheet>(sizePerIMG, IMGcount, material);
}

glm::vec2 SpriteSheet::size() {
	return IMGcount;
}