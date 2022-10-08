#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include "VBO.h"

class VAO {
	bool disposed = false;

public:
	GLuint handle;
	VAO();
	static Ref<VAO> Create();
	void Delete();
	void Bind();
	void Unbind();
	void LinkAttrib(VBO& vbo, GLuint layout, int size, GLenum type, int stride, void* pointer);
	~VAO();
};

#endif