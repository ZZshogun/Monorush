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
	std::vector<Texture> textures;

public:
	Transform transform;
	VAO handle;

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& texture);
	static std::unique_ptr<Mesh> Create(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& texture);
	void Draw(Shader* shader);
	void Delete();
	~Mesh();
};

#endif 
