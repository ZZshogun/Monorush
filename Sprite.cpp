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

std::vector<Vertex> Sprite::GetVertices(glm::vec2 size, glm::vec2 UVrepeat) {
	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, glm::vec2{0, 0}},
		{glm::vec3{-h_width, h_height, 0}, glm::vec2{0, UVrepeat.y}},
		{glm::vec3{h_width, h_height, 0}, glm::vec2{UVrepeat.x, UVrepeat.y}},
		{glm::vec3{h_width, -h_height, 0}, glm::vec2{UVrepeat.x, 0}},
	};

	return vertices;
}

void Sprite::FlipVertices(std::vector<Vertex>& vertices, glm::bvec2 flip) {
	if (flip.x && flip.y) {
		glm::vec2 swap02 = vertices[0].normal;
		vertices[0].normal = vertices[2].normal;
		vertices[2].normal = swap02;
		glm::vec2 swap13 = vertices[1].normal;
		vertices[1].normal = vertices[3].normal;
		vertices[3].normal = swap13;
	}
	else if (flip.x) {
		glm::vec2 swap03 = vertices[0].normal;
		vertices[0].normal = vertices[3].normal;
		vertices[3].normal = swap03;
		glm::vec2 swap12 = vertices[1].normal;
		vertices[1].normal = vertices[2].normal;
		vertices[2].normal = swap12;
	}
	else if (flip.y) {
		glm::vec2 swap01 = vertices[1].normal;
		vertices[1].normal = vertices[0].normal;
		vertices[0].normal = swap01;
		glm::vec2 swap23 = vertices[3].normal;
		vertices[3].normal = vertices[2].normal;
		vertices[2].normal = swap23;
	}
}

void Sprite::Create(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, Ref<Material>& material, glm::vec2 UVrepeat) {
	std::vector<Vertex> vertices = GetVertices(size, UVrepeat);

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

void Sprite::Resize(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, glm::vec2 start, glm::vec2 end, glm::bvec2 flip) {
	handle->Bind();

	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, start},
		{glm::vec3{-h_width, h_height, 0}, glm::vec2{start.x, end.y}},
		{glm::vec3{h_width, h_height, 0}, end},
		{glm::vec3{h_width, -h_height, 0}, glm::vec2{end.x, start.y}},
	};

	if (flip.x || flip.y) FlipVertices(vertices, flip);

	data->Subdata(vertices);
	data->Bind();
	handle->LinkAttrib(*data, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle->LinkAttrib(*data, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle->Unbind();
	data->Unbind();
}

void Sprite::Resize(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, glm::vec2 UVrepeat, glm::bvec2 flip) {
	handle->Bind();

	float h_width = size.x / 2;
	float h_height = size.y / 2;
	std::vector<Vertex> vertices = {
		{glm::vec3{-h_width, -h_height, 0}, glm::vec2{0, 0}},
		{glm::vec3{-h_width, h_height, 0}, glm::vec2{0, UVrepeat.y}},
		{glm::vec3{h_width, h_height, 0}, glm::vec2{UVrepeat.x, UVrepeat.y}},
		{glm::vec3{h_width, -h_height, 0}, glm::vec2{UVrepeat.x, 0}},
	};

	if (flip.x || flip.y) FlipVertices(vertices, flip);

	data->Subdata(vertices);
	data->Bind();
	handle->LinkAttrib(*data, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle->LinkAttrib(*data, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle->Unbind();
	data->Unbind();
}