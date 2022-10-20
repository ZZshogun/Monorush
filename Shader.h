#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "Core.h"

std::string Read_from_file(const char* filepath);

class Shader {
	bool disposed = false;

	Shader();

public:
	static std::map<std::string, Ref<Shader>> LUT;
	static bool log;
	int handle;

	Shader(const char* vertexCode, const char* fragmentCode);
	void Bind();
	void Delete();
	~Shader();

	static void Init();
	static Ref<Shader> Create(const char* vertexCode, const char* fragmentCode);
};

#endif
