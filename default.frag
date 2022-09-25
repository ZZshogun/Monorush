#version 330 core

in vec3 fPos;
in vec4 fColor;
in vec2 fTexCoord;

uniform sampler2D tex0;

out vec4 glColor;

void main() {
	

	glColor = fColor * texture(tex0, fTexCoord);
}