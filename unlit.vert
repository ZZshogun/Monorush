
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec2 offset;

out vec4 fColor;
out vec2 fTexCoord;

void main() {

	fColor = color;
	fTexCoord = aTexCoord + offset;

	gl_Position = proj * view * model * vec4(aPos, 1);

}