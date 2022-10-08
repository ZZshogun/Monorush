#include "Camera.h"

void Camera::Update(Transform& transform, glm::vec2& resolution, Ref<Shader>& shader) {

	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::vec2 res = resolution / 2.0f;
	projection = glm::ortho<float>(
		-res.x,
		res.x,
		-res.y,
		res.y,
		-1000,
		1000
	);

	view = glm::translate(view, -transform.position);
	view = glm::rotate(view, glm::radians(-transform.rotation.x), glm::vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians(-transform.rotation.y), glm::vec3(0, 1, 0));
	view = glm::rotate(view, glm::radians(-transform.rotation.z), glm::vec3(0, 0, 1));

	shader->Bind();
	glUniformMatrix4fv(glGetUniformLocation(shader->handle, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->handle, "view"), 1, GL_FALSE, glm::value_ptr(view));
}