#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "Core.h"
#include "Sprite.h"

enum CameraBackgroundMode {
	SolidColor,
	TexBackground,
};

class Camera {
	Ref<Sprite> spriteBackground;
	CameraBackgroundMode currentMode = SolidColor;

public:
	Transform transform;
	Ref<Shader> cameraShader;

	glm::vec4 solidColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec2 resolution = { 16, 9 };

	void Draw();
	void Update(Ref<Shader>& shader);
	void Set(glm::vec2 resolution);

	void SetBackgroundMode(CameraBackgroundMode mode);
	void SetBackground(Ref<Texture>& texture, float repeatUV = 1);
	void OffsetBackground(glm::vec2 addition);
};


#endif
