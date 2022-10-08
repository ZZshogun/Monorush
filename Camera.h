#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "Core.h"
#include "Shader.h"
#include "Transform.h"

class Camera {

	Camera();
public:
	static void Update(Transform& transform, glm::vec2& resolution, Ref<Shader>& shader);
};


#endif
