#include "Camera.h"

void Camera::Update(Shader& shader, int scr_w, int scr_h) {

	glm::mat4 projection = glm::mat4(1.0f);

	glm::mat4 view = glm::mat4(1.0f);

	float orthoX = scr_w * orthoSize;
	float orthoY = scr_h * orthoSize;

	projection = glm::ortho<float>(-orthoX / 2, orthoX / 2, -orthoY / 2, orthoY / 2, -1, 1);

	view = glm::translate(view, -transform.position);

	view = glm::rotate(view, glm::radians(-transform.rotation.x), glm::vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians(-transform.rotation.y), glm::vec3(0, 1, 0));
	view = glm::rotate(view, glm::radians(-transform.rotation.z), glm::vec3(0, 0, 1));

	shader.Bind();
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "view"), 1, GL_FALSE, glm::value_ptr(view));
}