#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "Shader.h"
#include "Transform.h"

class Camera {
public:

	Transform transform;

	float orthoSize = 2;

	void Update(Shader& shader, int scr_w, int scr_h);
};


#endif
