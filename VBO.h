#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

struct Vertex {
	glm::vec3 vertice;
	glm::vec4 color;
	glm::vec2 normal;
};

class VBO {
	bool disposed = false;


public:
	GLuint handle;
	VBO(std::vector<Vertex>& vertices);
	void Bind();
	void Unbind();
	void Delete();
	~VBO();
};

#endif
