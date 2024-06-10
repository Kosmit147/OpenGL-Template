#version 430 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec4 inColor;

out vec2 TexCoords;
out vec4 Color;

void main()
{
	TexCoords = inTexCoords;
	Color = inColor;
	gl_Position = inPosition;
}
