#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include "MagiaLog.h"

std::string Read_from_file(const char* filepath);

class Shader {
	bool disposed = false;
public:
	int handle;

	Shader(const char* vertexCode, const char* fragmentCode);
	~Shader();
	void Bind();
	void Delete();
};

#endif
