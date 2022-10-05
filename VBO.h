#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "Core.h"

struct Vertex {
	glm::vec3 vertice;
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
