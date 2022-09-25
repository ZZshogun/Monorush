#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "Shader.h"
#include "Transform.h"

class Camera : public Transform {
public:
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 forward = glm::vec3(0, 1, 1);
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	float orthoSize = 2;

	Camera(glm::vec3 pos);
	void Update(Shader& shader, int scr_w, int scr_h);
};


#endif
