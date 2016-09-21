#version 410 core

uniform float u_time;
uniform sampler2D u_diffuseMap;
in vec4 passColor;

// Output
out vec4 outColor;

void main(void)
{
	// outColor = passColor;
	outColor = passColor;
}