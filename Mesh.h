#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <glm/glm.hpp>
#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Mesh {
	bool disposed = false;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Ref<Texture>> textures;

public:
	Transform transform;
	glm::vec2 texOffset = glm::vec2(0);
	VAO handle;

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Ref<Texture>>& texture);
	static Ref<Mesh> Create(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Ref<Texture>>& texture);
	void Draw(Ref<Shader>& shader);
	void Delete();
	~Mesh();
};

#endif 
