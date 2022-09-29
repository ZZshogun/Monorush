#include "Camera.h"

void Camera::Update(Shader* shader) {

	glm::mat4 projection = glm::mat4(1.0f);

	glm::mat4 view = glm::mat4(1.0f);

	float h_width = (float)width / 2;
	float h_height = (float)height / 2;

	projection = glm::ortho<float>(-h_width, h_width, -h_height, h_height, -1000, 1000);

	view = glm::translate(view, -transform.position);

	view = glm::rotate(view, glm::radians(-transform.rotation.x), glm::vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians(-transform.rotation.y), glm::vec3(0, 1, 0));
	view = glm::rotate(view, glm::radians(-transform.rotation.z), glm::vec3(0, 0, 1));

	shader->Bind();
	glUniformMatrix4fv(glGetUniformLocation(shader->handle, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->handle, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::Set(int width, int height) {
	this->width = width;
	this->height = height;
}