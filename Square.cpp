#include "Square.h"

Square::Square (int width, int height, std::vector<Texture>& textures, int UVLimit) {

	this->width = width;
	this->height = height;

	std::vector<Vertex> vertices = {
		{glm::vec3{-width / 2.0, -height / 2.0, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, 0}},
		{glm::vec3{-width / 2.0, height / 2.0, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{0, UVLimit}},
		{glm::vec3{width / 2.0, height / 2.0, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{UVLimit, UVLimit}},
		{glm::vec3{width / 2.0, -height / 2.0, 0}, glm::vec4{1, 1, 1, 1}, glm::vec2{UVLimit, 0}},
	};

	std::vector<GLuint> indices = {
		0, 3, 1,
		1, 3, 2
	};

	mesh.SetMesh(vertices, indices, textures);
}

void Square::Draw(Shader& shader) {
	mesh.transform = transform;
	mesh.Draw(shader);
}