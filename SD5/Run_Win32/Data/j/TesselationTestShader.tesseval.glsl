#version 400

layout (triangles, equal_spacing, ccw) in;

uniform mat4 u_worldToClipMatrix;

in ControlData
{
	vec2 texcoord0;
	vec4 color;
} cData[];

out vec4 v_surfaceColor;
out vec2 v_texCoords; 

void main(void)
{

	v_surfaceColor = cData[0].color * gl_TessCoord.x + cData[1].color * gl_TessCoord.y + cData[2].color * gl_TessCoord.z;
	v_texCoords = cData[0].texcoord0 * gl_TessCoord.x + cData[1].texcoord0 * gl_TessCoord.y + cData[2].texcoord0 * gl_TessCoord.z;

	vec4 vPos = (gl_TessCoord.x * gl_in[0].gl_Position) +
		(gl_TessCoord.y * gl_in[1].gl_Position) +
		(gl_TessCoord.z * gl_in[2].gl_Position);
		
		gl_Position = u_worldToClipMatrix * vPos;
	
}