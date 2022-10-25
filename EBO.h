#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include "Core.h"

class EBO {
	bool disposed = false;
public:
	GLuint handle;
	EBO(std::vector<GLuint>& indices);
	static Ref<EBO> Create(std::vector<GLuint>& indices);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
