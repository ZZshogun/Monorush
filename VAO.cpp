#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &handle);
}

Ref<VAO> VAO::Create() {
	return std::make_shared<VAO>();
}

VAO::~VAO() {
	Delete();
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, int size, GLenum type, int stride, void* pointer) {
	vbo.Bind();
	glVertexAttribPointer(layout, size, type, GL_FALSE, stride, pointer);
	glEnableVertexAttribArray(layout);
	vbo.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(handle);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	if (disposed) return;

	std::cout << "DELETE VAO " << handle << "\n";
	disposed = true;
	glDeleteVertexArrays(1, &handle);
}