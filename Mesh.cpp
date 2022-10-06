#include "Mesh.h"

Material Material::Create(Ref<Shader>& shader, Ref<Texture>& texture, glm::vec4 color) {
	Material mat;
	mat.shader = shader;
	mat.texture = texture;
	mat.color = color;
	return mat;
}

Material Material::Create(Ref<Shader>& shader, glm::vec4 color) {
	Ref<Texture> tex = Texture::Create();
	return Create(shader, tex, color);
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material) {
	this->vertices = vertices;
	this->indices = indices;
	this->material = material;

	handle.Bind();

	VBO vbo(vertices);
	EBO ebo(indices);

	handle.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	handle.LinkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

	handle.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	vbo.Delete();
	ebo.Delete();

	//std::cout << "CREATE Mesh " << handle.handle << "\n";
}

Ref<Mesh> Mesh::Create(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material) {
	return std::make_shared<Mesh>(vertices, indices, material);
}

void Mesh::Draw() {
 	material.shader->Bind();
	handle.Bind();

	material.texture->TexUnit(material.shader, "tex0", 0);
	material.texture->Bind();

	glUniformMatrix4fv(glGetUniformLocation(material.shader->handle, "model"), 
		1, 
		GL_FALSE, 
		glm::value_ptr(transform.Model())
	);
	glUniform2f(glGetUniformLocation(material.shader->handle, "offset"), texOffset.x, texOffset.y);

	glUniform4f(glGetUniformLocation(material.shader->handle, "color"),
		material.color.r,
		material.color.g,
		material.color.b,
		material.color.a
	);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete() {
	if (disposed) return;

	disposed = true;
	//std::cout << "DELETE Mesh " << handle.handle << "\n";
}

Mesh::~Mesh() {
	Delete();
}