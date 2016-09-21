#version 410 core

uniform mat4 u_objectToWorldMatrix;
uniform mat4 u_worldToClipMatrix;

in vec3 a_position;
in vec4 a_color;

out VS_OUT
{
	vec4 color;
} vs_out;

out vec4 passColor;

void main(void)
{
	vec3 objectPosition = a_position;
	vec4 worldPosition = u_objectToWorldMatrix * vec4(objectPosition, 1.0);
	gl_Position = u_worldToClipMatrix * worldPosition; //Convert to clip space
	vs_out.color = a_color;
	passColor = a_color;
}