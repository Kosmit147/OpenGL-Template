#version 430 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColor;

out vec4 Color;

void main()
{
	Color = inColor;
	gl_Position = inPosition;
}
