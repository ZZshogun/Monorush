#include "Mesh.h"

Mesh::Mesh() {
	std::cout << "CREATE Mesh(Empty) " << handle.handle << "\n";
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {
	SetMesh(vertices, indices, textures);
	std::cout << "CREATE Mesh " << handle.handle << "\n";
}

void Mesh::SetMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	handle.Bind();

	VBO vbo(vertices);
	EBO ebo(indices);

	handle.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle.LinkAttrib(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	handle.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(7 * sizeof(float)));

	handle.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	vbo.Delete();
	ebo.Delete();
}

void Mesh::Draw(Shader& shader) {
	shader.Bind();
	handle.Bind();

	for (int i = 0; i < textures.size(); i++) {
		textures[i].TexUnit(shader, ("tex" + std::to_string(i)).c_str(), i);
		textures[i].Bind();
	}

	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, GL_FALSE, glm::value_ptr(transform.Model()));

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete() {
	if (disposed) return;

	for (int i = 0; i < textures.size(); i++) textures[i].Delete();
	handle.Delete();

	disposed = true;
	std::cout << "DELETE Mesh " << handle.handle << "\n";
}

Mesh::~Mesh() {
	Delete();
}