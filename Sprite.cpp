#include "Sprite.h"

std::vector<GLuint> Sprite::__indices = {
		0, 3, 1,
		1, 3, 2
};

std::vector<Vertex> Sprite::_GetVertices(float UVLimit) {
	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, 0}},
		{glm::vec3{-h_width, h_height, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, UVLimit}},
		{glm::vec3{h_width, h_height, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{UVLimit, UVLimit}},
		{glm::vec3{h_width, -h_height, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{UVLimit, 0}},
	};

	return vertices;
}

Sprite::Sprite() {

	textures = {
		Texture::Create("texture/sprite_default.png", 0),
	};

	std::vector<Vertex> vertices = _GetVertices(1);

	mesh = Mesh::Create(vertices, __indices, textures);
}

Sprite::Sprite(glm::vec2 size, float UVLimit) {
	this->size = size;

	textures = {
		Texture::Create("texture/sprite_default.png", 0),
	};

	std::vector<Vertex> vertices = _GetVertices(UVLimit);

	mesh = Mesh::Create(vertices, __indices, textures);
}

Sprite::Sprite(glm::vec2 size, Ref<Texture>& texture, float UVLimit) {

	this->size = size;

	std::vector<Vertex> vertices = _GetVertices(UVLimit);

	textures = { texture };

	mesh = Mesh::Create(vertices, __indices, textures);
}

Ref<Sprite> Sprite::Create() {
	return std::make_shared<Sprite>();
}

Ref<Sprite> Sprite::Create(glm::vec2 size, float UVLimit) {
	return std::make_shared<Sprite>(size);
}

Ref<Sprite> Sprite::Create(glm::vec2 size, Ref<Texture>& texture, float UVLimit) {
	return std::make_shared<Sprite>(size, texture, UVLimit);
}

void Sprite::Draw(Ref<Shader> shader) {
	mesh->transform = transform;
	mesh->Draw(shader);
}