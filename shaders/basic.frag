#version 430 core

in vec2 TexCoords;
in vec4 Color;

out vec4 outColor;

uniform sampler2D sampler;
uniform float time;

void main()
{
	vec4 texColor = texture(sampler, TexCoords);
	outColor = vec4(sin(time) / 2.0 + 0.5, 1.0, cos(time) / 2.0 + 0.5, 1.0);
	outColor = mix(Color, outColor, 0.5);
	outColor = mix(outColor, texColor, 0.5);
}
