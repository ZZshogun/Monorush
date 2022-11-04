#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "Core.h"

std::string Read_from_file(const char* filepath);

class Shader {
	struct GLVariable {
		GLint location = -1;
		GLenum type = GL_INT;
		std::string name = std::string();
	};

	bool disposed = false;

	std::unordered_map<std::string, GLVariable> attribLocation;
	std::unordered_map<std::string, GLVariable> uniformLocation;

	Shader();

public:
	static std::unordered_map<std::string, Ref<Shader>> LUT;
	static bool log;
	int handle;

	Shader(const char* vertexCode, const char* fragmentCode);
	void Delete();
	~Shader();
	void Bind();
	GLVariable& GetAttribLocation(std::string attrib);
	GLVariable& GetUniformLocation(std::string uniform);

	void UniformSampler2D(std::string uniform, unsigned int value);
	void UniformFloat(std::string uniform, float value);
	void UniformVec2(std::string uniform, glm::vec2 vec);
	void UniformVec3(std::string uniform, glm::vec3 vec);
	void UniformVec4(std::string uniform, glm::vec4 vec);
	void UniformMat4(std::string uniform, glm::mat4 mat);

	static void Init();
	static Ref<Shader> Create(const char* vertexCode, const char* fragmentCode);
};

#endif
