#version 400

layout (vertices = 3) out;

in VertexData
{
	vec2 texcoord0;
	vec4 color;
} vData[];

out ControlData
{
	vec2 texcoord0;
	vec4 color;
} cData[];

void main(void)
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	cData[gl_InvocationID].texcoord0 = vData[gl_InvocationID].texcoord0;
	cData[gl_InvocationID].color = vData[gl_InvocationID].color;

	if (gl_InvocationID==0)
	{
		gl_TessLevelInner[0] = 1.0;

		gl_TessLevelOuter[0] = 1.0;
		gl_TessLevelOuter[1] = 1.0;
		gl_TessLevelOuter[2] = 1.0;
	}

}