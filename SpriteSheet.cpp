#include "SpriteSheet.h"

std::vector<Vertex> SpriteSheet::_GetVertex(glm::vec2 index) {

	glm::vec2 h_size = sizePerIMG / 2.0f;

	glm::vec2 idxAsPix = index / IMGcount;
	glm::vec2 idxAsPix1 = (index + glm::vec2{1, 1}) / IMGcount;

	std::vector<Vertex> vertices = {
		{glm::vec3{-h_size.x, -h_size.y, 0}, idxAsPix},
		{glm::vec3{-h_size.x, h_size.y, 0}, glm::vec2{idxAsPix.x, idxAsPix1.y}},
		{glm::vec3{h_size.x, h_size.y, 0}, idxAsPix1},
		{glm::vec3{h_size.x, -h_size.y, 0}, glm::vec2{idxAsPix1.x, idxAsPix.y}},
	};
	return vertices;
}

SpriteSheet::SpriteSheet(glm::vec2 sizePerIMG, glm::vec2 IMGcount, Material& material) {
	this->sizePerIMG = sizePerIMG;
	this->IMGcount = IMGcount;
	this->material = material;
}

void SpriteSheet::Draw(glm::vec2 index) {
	index = glm::clamp(index, glm::vec2{ 0, 0 }, IMGcount);
	if (index != lastIndex) {
		std::vector<GLuint> indices = {
			0, 3, 1,
			1, 3, 2
		};
		std::vector<Vertex> vertices = _GetVertex(index);
		sprites.reset();
		sprites = Mesh::Create(vertices, indices, material);
		lastIndex = index;
	}
	sprites->transform = transform;
	sprites->Draw();
}

Ref<SpriteSheet> SpriteSheet::Create(glm::vec2 sizePerIMG, glm::vec2 IMGcount, Material& material) {
	return std::make_shared<SpriteSheet>(sizePerIMG, IMGcount, material);
}