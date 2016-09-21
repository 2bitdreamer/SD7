#version 410 core

// define the number of CPs in the output patch
layout (vertices = 3) out;

// attributes of the input CPs
in VS_OUT
{
	vec4 color;
} ts_in[];

out TCS_OUT 
{
	vec4 color;
} tcs_out[];

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcs_out[gl_InvocationID].color = ts_in[gl_InvocationID].color;
   
    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = 8;
    gl_TessLevelOuter[1] = 8;
    gl_TessLevelOuter[2] = 8;
    gl_TessLevelInner[0] = 8;
}