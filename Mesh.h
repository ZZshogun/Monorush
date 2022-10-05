#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "Core.h"
#include "Transform.h"
#include "Texture.h"
#include "VAO.h"
#include "EBO.h"

struct Material {
	Ref<Shader> shader;
	Ref<Texture> texture;
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	static Material Create(
		Ref<Shader>& shader,
		Ref<Texture>& texture,
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	static Material Create(
		Ref<Shader>& shader,
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
};

class Mesh {
	bool disposed = false;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	Transform transform;
	Material material;
	VAO handle;

	glm::vec2 texOffset = glm::vec2(0);

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material);
	static Ref<Mesh> Create(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material);
	void Draw();
	void Delete();
	~Mesh();
};

#endif 
