#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "Shader.h"
#include "Transform.h"

class Camera {
public:

	Transform transform;

	int width = 1280;
	int height = 720;

	float zoomLevel = 1;

	void Update(Shader* shader);
	void Set(int width, int height);
};


#endif
