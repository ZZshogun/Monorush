#version 330 core

in vec2 fTexCoord;

uniform vec4 fColor;
uniform sampler2D tex0;

uniform vec2 viewport;
uniform float intensity;

out vec4 glColor;

void main() {
    float ratio = viewport.x / viewport.y;
    vec2 uv = gl_FragCoord.xy / viewport.xy;

	vec4 color = fColor * texture(tex0, fTexCoord);

    color *= (smoothstep(0., intensity, uv.x)) * (1. - smoothstep(1. - intensity, 1., uv.x));
    color *= (smoothstep(0., intensity * ratio, uv.y)) * (1. - smoothstep(1. - intensity * ratio, 1., uv.y));

    gl_FragColor = color;
}