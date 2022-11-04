#include "../header/VBO.h"

VBO::VBO(std::vector<Vertex>& vertices) {
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
}

Ref<VBO> VBO::Create(std::vector<Vertex>& vertices) {
	return std::make_shared<VBO>(vertices);
}

void VBO::Subdata(std::vector<Vertex>& vertices) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());
	Unbind();
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	if (disposed) return;

	disposed = true;
	glDeleteBuffers(1, &handle);
}

VBO::~VBO() {
	Delete();
}