
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 camMatrix;
uniform mat4 model;

out vec3 fPos;
out vec4 fColor;
out vec2 fTexCoord;

void main() {

	fColor = aColor;
	fTexCoord = aTexCoord;

	fPos = vec3(model * vec4(aPos, 1));

	gl_Position = camMatrix * vec4(fPos, 1);

}