
@OpenGL2.Vertex
//Uniforms
uniform mat4 entitymatrix;
uniform mat4 projectioncameramatrix;

//Attributes
attribute vec3 vertex_position;

//Outputs
varying vec4 ex_color;
varying float ex_selectionstate;

void main()
{
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	gl_Position = projectioncameramatrix * entitymatrix_ * vec4(vertex_position, 1.0);
	
	ex_color = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
	
	//If an object is selected, 10 is subtracted from the alpha color.
	//This is a bit of a hack that packs a per-object boolean into the alpha value.
	ex_selectionstate = 0.0;
	if (ex_color.a<-5.0)
	{
		ex_color.a += 10.0;
		ex_selectionstate = 1.0;
	}
}
@OpenGL2.Fragment
//Inputs
varying vec4 ex_color;
varying float ex_selectionstate;

void main(void)
{
	gl_FragColor = ex_color * (1.0-ex_selectionstate) + ex_selectionstate * (ex_color*0.5+vec4(0.5,0.0,0.0,0.0));
}
@OpenGLES2.Vertex
//Uniforms
uniform mediump mat4 entitymatrix;
uniform mediump mat4 projectioncameramatrix;

//Attributes
attribute mediump vec3 vertex_position;

//Outputs
varying mediump vec4 ex_color;

void main()
{
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	gl_Position = projectioncameramatrix * entitymatrix_ * vec4(vertex_position, 1.0);
	ex_color = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
}
@OpenGLES2.Fragment
//Inputs
varying mediump vec4 ex_color;

void main(void)
{
	gl_FragColor = ex_color;
}
@OpenGL4.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
uniform vec4 materialcolordiffuse;
uniform mat4 camerainversematrix;
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;

//Attributes
in vec3 vertex_position;
in vec4 vertex_color;
in vec2 vertex_texcoords0;
in vec3 vertex_normal;
in vec3 vertex_binormal;
in vec3 vertex_tangent;

//Outputs
out VertexData
{
	vec2 texcoord0;
	vec4 color;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
} vData;


void main()
{
	mat4 entitymatrix = entity.matrix[gl_InstanceID];

	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	gl_Position = entitymatrix_ * vec4(vertex_position,1.0);
	
	vData.texcoord0 = vertex_texcoords0;

	vData.color = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
	vData.color *= vec4(1.0-vertex_color.r,1.0-vertex_color.g,1.0-vertex_color.b,vertex_color.a) * materialcolordiffuse;

	mat3 nmat = mat3(camerainversematrix[0].xyz,camerainversematrix[1].xyz,camerainversematrix[2].xyz);//39
	nmat = nmat * mat3(entitymatrix[0].xyz,entitymatrix[1].xyz,entitymatrix[2].xyz);//40
	vData.normal = normalize(nmat * vertex_normal);	
	vData.binormal = normalize(nmat * vertex_binormal);
	vData.tangent = normalize(nmat * vertex_tangent);
}
@OpenGL4.Evaluation
#version 400

layout (triangles, equal_spacing, ccw) in;

uniform mat4 projectioncameramatrix;

uniform float verticalScale;
uniform float detailScale;
uniform float detailHeight;

in ControlData
{
	vec2 texcoord0;
	vec4 color;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
} cData[];

out VertexData
{
	vec2 texcoord0;
	vec4 color;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
} vData;

void main(void)
{
	vData.texcoord0 = cData[0].texcoord0 * gl_TessCoord.x + cData[1].texcoord0 * gl_TessCoord.y + cData[2].texcoord0 * gl_TessCoord.z;
	vData.color = cData[0].color * gl_TessCoord.x + cData[1].color * gl_TessCoord.y + cData[2].color * gl_TessCoord.z;
	vData.normal = cData[0].normal * gl_TessCoord.x + cData[1].normal * gl_TessCoord.y + cData[2].normal * gl_TessCoord.z;
	vData.binormal = cData[0].binormal * gl_TessCoord.x + cData[1].binormal * gl_TessCoord.y + cData[2].binormal * gl_TessCoord.z;
	vData.tangent = cData[0].tangent * gl_TessCoord.x + cData[1].tangent * gl_TessCoord.y + cData[2].tangent * gl_TessCoord.z;

	vec4 vPos = (gl_TessCoord.x * gl_in[0].gl_Position) +
		(gl_TessCoord.y * gl_in[1].gl_Position) +
		(gl_TessCoord.z * gl_in[2].gl_Position);

	gl_Position = projectioncameramatrix * vPos;
	
}
@OpenGL4.Control
#version 400

layout (vertices = 3) out;


in VertexData
{
	vec2 texcoord0;
	vec4 color;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
} vData[];

out ControlData
{
	vec2 texcoord0;
	vec4 color;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
} cData[];

void main(void)
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	cData[gl_InvocationID].texcoord0 = vData[gl_InvocationID].texcoord0;
	cData[gl_InvocationID].color = vData[gl_InvocationID].color;
	cData[gl_InvocationID].normal = vData[gl_InvocationID].normal;
	cData[gl_InvocationID].binormal = vData[gl_InvocationID].binormal;
	cData[gl_InvocationID].tangent = vData[gl_InvocationID].tangent;

	if (gl_InvocationID==0)
	{
		gl_TessLevelInner[0] = 2.0;

		gl_TessLevelOuter[0] = 2.0;
		gl_TessLevelOuter[1] = 2.0;
		gl_TessLevelOuter[2] = 2.0;
	}

}
@OpenGL4.Fragment
#version 400

//Uniforms
uniform sampler2D texture0;//diffuse map
uniform sampler2D texture1;//normal map
uniform mat4 camerainversematrix;

//Inputs
in VertexData
{
	vec2 texcoord0;
	vec4 color;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
} vData;

out vec4 fragData0;
out vec4 fragData1;
out vec4 fragData2;
out vec4 fragData3;

void main(void)
{
	vec4 outcolor = vData.color;

	//Blend with selection color if selected
	fragData0 = outcolor;
	
	int materialflags=1;
	fragData2 = vec4(0.0,0.0,0.0,materialflags/255.0);
}
