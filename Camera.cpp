#include "Camera.h"

Camera::Camera(glm::vec3 pos) {
	this->position = pos;
}

void Camera::Update(Shader& shader, int scr_w, int scr_h) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	float orthoX = scr_w * orthoSize;
	float orthoY = scr_h * orthoSize;

	projection = glm::ortho<float>(-orthoX / 2, orthoX / 2, -orthoY / 2, orthoY / 2, -1000, 1000);
	view = glm::lookAt(position, position + orientation, up);

	cameraMatrix = projection * view * Model();

	shader.Bind();
	glUniformMatrix4fv(glGetUniformLocation(shader.handle, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}