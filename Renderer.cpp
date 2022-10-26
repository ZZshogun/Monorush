#include "Renderer.h"


void Renderer::DrawSprite(
	Ref<VAO>& handle,
	Transform transform, 
	Ref<Material>& material, 
	glm::vec2 texOffset
) 
{	
	material->shader->Bind();
	handle->Bind();

	material->texture->TexUnit(material->shader, "tex0", 0);
	material->texture->Bind();

	glUniformMatrix4fv(glGetUniformLocation(material->shader->handle, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(transform.Model())
	);
	glUniform2f(glGetUniformLocation(material->shader->handle, "offset"), texOffset.x, texOffset.y);

	glUniform4f(glGetUniformLocation(material->shader->handle, "fColor"),
		material->color.r,
		material->color.g,
		material->color.b,
		material->color.a
	);

	glDrawElements(GL_TRIANGLES, (GLsizei)Sprite::_indices_square.size(), GL_UNSIGNED_INT, (void*)0);

	handle->Unbind();
}