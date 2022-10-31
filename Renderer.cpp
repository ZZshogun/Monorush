#include "Renderer.h"

void Renderer::DrawSprite(Ref<VAO>& handle, TransformComponent transform, Ref<Material>& material, glm::vec2 texOffset) {	
	material->shader->Bind();
	handle->Bind();

	material->texture->TexUnit(material->shader, "tex0", 0);
	material->texture->Bind();

	material->shader->UniformMat4("model", transform.Model());
	material->shader->UniformVec2("offset", texOffset);
	material->shader->UniformVec4("fColor", material->color);

	glDrawElements(GL_TRIANGLES, (GLsizei)Sprite::_indices_square.size(), GL_UNSIGNED_INT, (void*)0);

	handle->Unbind();
}