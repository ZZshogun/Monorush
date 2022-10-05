#include "Sprite.h"

std::vector<GLuint> Sprite::__indices = {
	0, 3, 1,
	1, 3, 2
};

std::vector<Vertex> Sprite::_GetVertices(float UVLimit) {
	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, glm::vec2{0, 0}},
		{glm::vec3{-h_width, h_height, 0}, glm::vec2{0, UVLimit}},
		{glm::vec3{h_width, h_height, 0}, glm::vec2{UVLimit, UVLimit}},
		{glm::vec3{h_width, -h_height, 0}, glm::vec2{UVLimit, 0}},
	};

	return vertices;
}

Sprite::Sprite(glm::vec2 size, Material& material, float UVLimit) {

	this->size = size;

	std::vector<Vertex> vertices = _GetVertices(UVLimit);

	mesh = Mesh::Create(vertices, __indices, material);
}

Ref<Sprite> Sprite::Create(glm::vec2 size, Material& material, float UVLimit) {
	return std::make_shared<Sprite>(size, material, UVLimit);
}

void Sprite::Draw() {
	mesh->transform = transform;
	mesh->Draw();
}