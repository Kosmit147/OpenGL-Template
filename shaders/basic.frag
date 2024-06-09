#version 430 core

in vec4 Color;

out vec4 outColor;

uniform float time;

void main()
{
	outColor = vec4(sin(time) / 2.0 + 0.5, 1.0, cos(time) / 2.0 + 0.5, 1.0);
	outColor = mix(Color, outColor, 0.5);
}
