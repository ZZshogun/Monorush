#include "Sprite.h"

Material::Material(Ref<Texture>& texture, glm::vec4 color, const char* shaderName) {
	this->color = color;
	this->texture = texture;
	this->shader = Shader::LUT[shaderName];
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

void Sprite::Create(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, Ref<Material>& material, float UVLimit) {
	std::vector<Vertex> vertices = GetVertices(size, UVLimit);

	handle->Bind();

	if (data) data.reset();
	data = VBO::Create(vertices);
	EBO ebo(_indices_square);

	handle->LinkAttrib(*data, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle->LinkAttrib(*data, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle->Unbind();
	data->Unbind();
	ebo.Unbind();

	ebo.Delete();
}

void Sprite::Resize(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, glm::vec2 start, glm::vec2 end) {
	handle->Bind();

	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, start},
		{glm::vec3{-h_width, h_height, 0}, glm::vec2{start.x, end.y}},
		{glm::vec3{h_width, h_height, 0}, end},
		{glm::vec3{h_width, -h_height, 0}, glm::vec2{end.x, start.y}},
	};

	data->Subdata(vertices);
	data->Bind();
	handle->LinkAttrib(*data, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle->LinkAttrib(*data, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle->Unbind();
	data->Unbind();
}

void Sprite::Resize(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, float UVrepeat) {
	handle->Bind();

	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, glm::vec2{0, 0}},
		{glm::vec3{-h_width, h_height, 0}, glm::vec2{0, UVrepeat}},
		{glm::vec3{h_width, h_height, 0}, glm::vec2{UVrepeat, UVrepeat}},
		{glm::vec3{h_width, -h_height, 0}, glm::vec2{UVrepeat, 0}},
	};

	data->Subdata(vertices);
	data->Bind();
	handle->LinkAttrib(*data, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle->LinkAttrib(*data, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle->Unbind();
	data->Unbind();
}