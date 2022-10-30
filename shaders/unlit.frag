#version 330 core

in vec2 fTexCoord;

uniform vec4 fColor;
uniform sampler2D tex0;

out vec4 glColor;

void main() {
	glColor = fColor * texture(tex0, fTexCoord);
}