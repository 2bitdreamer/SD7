#version 410 core

layout (triangles, equal_spacing, ccw) in;

in TCS_OUT 
{
	vec4 color;
} tes_in[];

out vec4 passColor;

void main()
{
	vec4 pos0 = gl_in[0].gl_Position;
	vec4 pos1 = gl_in[1].gl_Position;
	vec4 pos2 = gl_in[2].gl_Position;
	vec3 w = gl_TessCoord;

	vec4 final_pos = w.x * pos0
		+ w.y * pos1 
		+ w.z * pos2;

	vec4 final_color = tes_in[0].color * w.x
		+ tes_in[1].color * w.y
		+ tes_in[2].color * w.z;


	gl_Position = final_pos;
	passColor = final_color;
}