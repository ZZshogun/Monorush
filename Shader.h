#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "Core.h"

std::string Read_from_file(const char* filepath);

class Shader {
	bool disposed = false;

	Shader();

public:
	int handle;

	Shader(const char* vertexCode, const char* fragmentCode);
	static Ref<Shader> Create(const char* vertexCode, const char* fragmentCode);
	void Bind();
	void Delete();
	~Shader();
};

#endif
