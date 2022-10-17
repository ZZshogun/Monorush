#version 330 core

in vec2 fTexCoord;

uniform vec4 fColor;
uniform sampler2D tex0;

out vec4 glColor;

void main() {
	vec4 sampled = vec4(1, 1, 1, texture(tex0, fTexCoord).r);
	glColor = fColor * sampled;
}