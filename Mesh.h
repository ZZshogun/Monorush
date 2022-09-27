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

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	void SetMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	void Draw(Shader& shader);
	~Mesh();
	void Delete();
};

#endif 
