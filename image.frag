#version 330 core

in vec2 fTexCoord;

uniform sampler2D tex0;
uniform vec4 fColor;

out vec4 glColor;

void main() {
	glColor = fColor * texture(tex0, fTexCoord);
}