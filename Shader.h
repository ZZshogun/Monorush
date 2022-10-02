#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>

std::string Read_from_file(const char* filepath);

class Shader {
	bool disposed = false;


public:
	int handle;

	Shader();
	Shader(const char* vertexCode, const char* fragmentCode);
	static std::unique_ptr<Shader> Create(const char* vertexCode, const char* fragmentCode);
	void Bind();
	void Delete();
	~Shader();
};

#endif
