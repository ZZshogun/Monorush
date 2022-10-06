#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "Core.h"

std::string Read_from_file(const char* filepath);

class Shader {
	bool disposed = false;

	static std::map<std::string, Ref<Shader>> LUT;
	Shader();

public:
	int handle;

	Shader(const char* vertexCode, const char* fragmentCode);
	void Bind();
	void Delete();
	~Shader();

	static void CompileAll();
	static Ref<Shader> Create(const char* vertexCode, const char* fragmentCode);
	static Ref<Shader>& Get(std::string shader);
	static std::map<std::string, Ref<Shader>>::iterator Begin();
	static std::map<std::string, Ref<Shader>>::iterator End();
};

#endif
