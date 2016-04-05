#version 330

uniform mat4 u_worldToClipMatrix;
uniform mat4 u_objectToWorldMatrix;

in vec3 a_position;
in vec4 a_color;
in vec2 a_texCoords;

out vec4 v_surfaceColor;
out vec2 v_texCoords;

void main()
{
	vec3 objectPosition = a_position;
	vec4 worldPosition = u_objectToWorldMatrix * vec4(objectPosition, 1.0);
	gl_Position =  u_worldToClipMatrix * worldPosition; //Convert to clip space

	v_texCoords = a_texCoords;
	v_surfaceColor = a_color;
}