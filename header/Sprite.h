#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "Core.h"
#include "Texture.h"
#include "VAO.h"
#include "EBO.h"

struct Material {
	Ref<Shader> shader;
	Ref<Texture> texture;
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	Material(
		Ref<Texture>& texture, 
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
		const char* shaderName = "unlit"
	);

	static Ref<Material> Create(
		Ref<Texture>& texture,
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		const char* shaderName = "unlit"
	);
	static Ref<Material> Create(
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		const char* shaderName = "unlit"
	);
};

class Sprite {

	static void FlipVertices(std::vector<Vertex>& vertices, glm::bvec2 flip);

public:
	inline static std::vector<GLuint> _indices_square = {
		0, 3, 1,
		1, 3, 2
	};

	static std::vector<Vertex> GetVertices(glm::vec2 size, glm::vec2 UVrepeat);
	static void Create(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, Ref<Material>& material, glm::vec2 UVrepeat = {1, 1});
	static void Resize(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, glm::vec2 start, glm::vec2 end, glm::bvec2 flip = { false, false });
	static void Resize(Ref<VAO>& handle, Ref<VBO>& data, glm::vec2 size, glm::vec2 UVrepeat = { 1, 1 }, glm::bvec2 flip = { false, false });
};

#endif 
