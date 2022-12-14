#include "../header/EBO.h"

EBO::EBO(std::vector<GLuint>& indices) {
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);
}

Ref<EBO> EBO::Create(std::vector<GLuint>& indices) {
	return std::make_shared<EBO>(indices);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	if (disposed) return;

	disposed = true;
	glDeleteBuffers(1, &handle);
}