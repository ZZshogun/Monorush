#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include "Shader.h"

class Texture {
	bool disposed = false;
public:
	GLuint handle;
	GLuint unit;
	Texture(const char* file, GLuint slot);
	~Texture();
	static Ref<Texture> Create(const char* file, GLuint slot);
	void TexUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
