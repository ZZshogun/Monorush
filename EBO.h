#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

class EBO {
	bool disposed = false;
public:
	GLuint handle;
	EBO(std::vector<GLuint>& indices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
