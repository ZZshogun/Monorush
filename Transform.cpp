#include "Transform.h"

glm::mat4 Transform::Model() const {

	glm::mat4 model = glm::mat4(1);

	model = glm::scale(model, scale);

	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));

	model = glm::translate(model, position);

	return model;
}