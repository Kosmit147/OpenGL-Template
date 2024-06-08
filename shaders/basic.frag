#version 430 core

out vec4 outColor;

uniform float time;

void main()
{
	outColor = vec4(sin(time) / 2.0 + 0.5, 1.0, cos(time) / 2.0 + 0.5, 1.0);
}
