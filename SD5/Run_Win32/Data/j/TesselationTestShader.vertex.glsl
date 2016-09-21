#version 410 core

uniform mat4 u_worldToClipMatrix;
uniform mat4 u_objectToWorldMatrix;

in vec3 a_position;
in vec4 a_color;
in vec2 a_texCoords;

out VS_OUT
{
	vec4 color;
} vs_out;

out vec4 passColor;

void main(void)
{
	vec4 pos = vec4(a_position, 1.0f);
	pos = pos * u_worldToClipMatrix * u_objectToWorldMatrix;

	vs_out.color = a_color;
	passColor = a_color;

	gl_Position = pos;
}


