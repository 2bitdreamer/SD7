#version 330

uniform mat4 u_worldToClipMatrix;
uniform mat4 u_objectToWorldMatrix;

in vec2 a_texCoords;
in vec4 a_color;
in vec3 a_position;
in vec3 a_normal;
in vec3 a_tangent;
in vec3 a_bitangent;

out vec4 v_surfaceColor;
out vec3 v_worldPosition;
out vec3 v_normal;
out vec2 v_texCoords;
out vec3 v_tangent;
out vec3 v_bitangent;

void main()
{
	vec3 objectPosition = a_position;
	vec4 worldPosition = u_objectToWorldMatrix * vec4(objectPosition, 1.0);
	gl_Position =  u_worldToClipMatrix * worldPosition; //Convert to clip space
	v_texCoords = a_texCoords;
	v_worldPosition = worldPosition.xyz;
	v_surfaceColor = a_color;
	v_tangent = a_tangent;
	v_bitangent = a_bitangent;
	v_normal = a_normal;

}