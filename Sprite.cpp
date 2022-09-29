#include "Sprite.h"

std::vector<GLuint> Sprite::__indices = {
		0, 3, 1,
		1, 3, 2
};

std::vector<Vertex> Sprite::_GetVertices(int UVLimit) {
	std::vector<Vertex> vertices = {
		{glm::vec3{-width / 2.0, -height / 2.0, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, 0}},
		{glm::vec3{-width / 2.0, height / 2.0, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, UVLimit}},
		{glm::vec3{width / 2.0, height / 2.0, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{UVLimit, UVLimit}},
		{glm::vec3{width / 2.0, -height / 2.0, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{UVLimit, 0}},
	};

	return vertices;
}

Sprite::Sprite() {

	std::vector<Texture> textures = { Texture("texture/sprite_default.png", 0) };

	std::vector<Vertex> vertices = _GetVertices(1);

	mesh = new Mesh(vertices, __indices, textures);
}

Sprite::Sprite(int width, int height, std::vector<Texture>& textures, int UVLimit) {

	this->width = width;
	this->height = height;

	std::vector<Vertex> vertices = _GetVertices(UVLimit);

	mesh = new Mesh(vertices, __indices, textures);
}

void Sprite::Draw(Shader* shader) {
	mesh->transform = transform;
	mesh->Draw(shader);
}

Sprite::~Sprite() {
	delete mesh;
}