#version 410 core

uniform mat4 gModel;
uniform mat4 gView;
uniform mat4 gProj;

in vec3 inPosition;
in vec4 inColor;

out VS_OUT 
{
	vec4 color;
} vs_out;

out vec4 passColor;

void main( void )
{
	vec4 pos = vec4( inPosition, 1.0f );
	pos = pos * gModel * gView * gProj;

	vs_out.color = inColor;
	passColor = inColor;

	gl_Position = pos;
}