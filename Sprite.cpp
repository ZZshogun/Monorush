#include "Sprite.h"

Material::Material(Ref<Texture>& texture, glm::vec4 color, const char* shaderName) {
	this->color = color;
	this->texture = texture;
	this->shader = Shader::Get(shaderName);
}

Ref<Material> Material::Create(Ref<Texture>& texture, glm::vec4 color, const char* shaderName) {
	return std::make_shared<Material>(texture, color, shaderName);
}

Ref<Material> Material::Create(glm::vec4 color, const char* shaderName) {
	Ref<Texture> tex = Texture::Create();
	return std::make_shared<Material>(tex, color, shaderName);
}

// =========================================================================== //

std::vector<Vertex> Sprite::GetVertices(glm::vec2 size, float UVLimit) {
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

void Sprite::Create(Ref<VAO>& handle, glm::vec2 size, Ref<Material>& material, float UVLimit) {
	std::vector<Vertex> vertices = GetVertices(size, UVLimit);

	handle->Bind();

	VBO vbo(vertices);
	EBO ebo(_indices_square);

	handle->LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle->LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle->Unbind();
	vbo.Unbind();
	ebo.Unbind();

	vbo.Delete();
	ebo.Delete();
}

void Sprite::Resize(Ref<VAO>& handle, glm::vec2 size, glm::vec2 start, glm::vec2 end) {
	handle->Bind();

	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, start},
		{glm::vec3{-h_width, h_height, 0}, glm::vec2{start.x, end.y}},
		{glm::vec3{h_width, h_height, 0}, end},
		{glm::vec3{h_width, -h_height, 0}, glm::vec2{end.x, start.y}},
	};

	VBO vbo(vertices);
	handle->LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle->LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle->Unbind();
	vbo.Unbind();

	vbo.Delete();
}

void Sprite::Resize(Ref<VAO>& handle, glm::vec2 size, float UVrepeat) {
	handle->Bind();

	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, glm::vec2{0, 0}},
		{glm::vec3{-h_width, h_height, 0}, glm::vec2{0, UVrepeat}},
		{glm::vec3{h_width, h_height, 0}, glm::vec2{UVrepeat, UVrepeat}},
		{glm::vec3{h_width, -h_height, 0}, glm::vec2{UVrepeat, 0}},
	};

	VBO vbo(vertices);
	handle->LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle->LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle->Unbind();
	vbo.Unbind();

	vbo.Delete();
}