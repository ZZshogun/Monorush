#include "Texture.h"

Ref<Texture> Texture::defaultTex;

Texture::Texture(const char* file, GLuint slot) {
	this->unit = slot;

	int imgW, imgH, imgCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(file, &imgW, &imgH, &imgCh, 0);

	glGenTextures(1, &handle);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (imgCh) {
		case 1:	
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
			break;
		default:
			throw std::invalid_argument("Texture bit depth is invalid. (" + std::to_string(imgCh) + " bits)");
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	std::cout << "CREATE Texture " << handle << " " << file << "\n";

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Ref<Texture> Texture::Create() {
	return defaultTex;
}

Ref<Texture> Texture::Create(const char* file, GLuint slot) {
	return std::make_shared<Texture>(file, slot);
}

void Texture::TexUnit(Ref<Shader>& shader, const char* uniform, GLuint unit) {
	shader->Bind();
	int location = glGetUniformLocation(shader->handle, uniform);
	glUniform1i(location, unit);
}

void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	if (disposed) return;

	disposed = true;
	std::cout << "DELETE Texture " << handle << "\n";
	glDeleteTextures(1, &handle);
}

Texture::~Texture() {
	Delete();
}