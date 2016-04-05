#include "Renderer.hpp"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "Engine\Graphics\GL\glext.h"

Renderer::Renderer(void)
{

}


Renderer::~Renderer(void)
{

}

// Globals "function pointer" variables
PFNGLGENBUFFERSPROC		glGenBuffers		= nullptr;
PFNGLBINDBUFFERPROC		glBindBuffer		= nullptr;
PFNGLBUFFERDATAPROC		glBufferData		= nullptr;
PFNGLGENERATEMIPMAPPROC	glGenerateMipmap	= nullptr;
PFNGLDELETEBUFFERSPROC	glDeleteBuffers     = nullptr;
PFNGLACTIVETEXTUREPROC  glActiveTexture     = nullptr;


void Renderer::myGLtranslatef(float x, float y, float z) {
	glTranslatef(x, y, z);
}

void Renderer::myGLrotatef(float angle, float x, float y, float z) {
	glRotatef(angle, x, y, z);
}

void Renderer::myGLVertex3f(float x, float y, float z) {
	glVertex3f(x, y, z);
}

void Renderer::myGLcolor3f(float r, float g, float b) {
	glColor3f(r, g, b);
}

void Renderer::myGLbegin(int mode) {
	if (mode == GL_LINES || mode == GL_QUADS || mode == GL_POINTS) { //1
		glBegin(mode);
	}
}

void Renderer::myGLpointSize(float size) {
	glPointSize(size);
}

void Renderer::myGLPopAttrib() {
	glPopAttrib();
}

void Renderer::myGLend() {
	glEnd();
}

void Renderer::myGLPushMatrix() {
	glPushMatrix();
}

void Renderer::myGLPopMatrix() {
	glPopMatrix();
}

void Renderer::myGLloadIdentity() {
	glLoadIdentity();
}

void Renderer::myGLDisable(int mode) {
	if (mode == GL_TEXTURE_2D || mode == GL_CULL_FACE || mode == GL_DEPTH_TEST || mode == GL_BLEND) { //0x0DE1
		glDisable(mode);
	}	
}

void Renderer::myGLEnable(int flag) {
	if (flag == GL_LINE_SMOOTH || flag == GL_TEXTURE_2D || flag == GL_BLEND || flag == GL_DEPTH_TEST) {
		glEnable(flag);
	}
}

void Renderer::myGLglColor4f(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void Renderer::myGLpushAttrib(int attrib) {
	if (attrib == GL_CURRENT_BIT || attrib == GL_ENABLE_BIT) {
		glPushAttrib(attrib);
	}
}

void Renderer::myGLLineWidth(float lineWidth) {
	glLineWidth(lineWidth);
}

void Renderer::myGLfrontFace(int attrib) { 
	if (attrib == GL_CCW) {
		glFrontFace(GL_CCW);
	}
}

void Renderer::myGLtexCoord2f(float firstCoord, float secondCoord) {
	glTexCoord2f(firstCoord, secondCoord);
}

void Renderer::myGLEnableClientState( int state) {
	if (state == GL_VERTEX_ARRAY || state == GL_COLOR_ARRAY || state == GL_TEXTURE_COORD_ARRAY) {
		glEnableClientState(state);
	}
}

void Renderer::myGLDisableClientState(int state) {
	if (state == GL_VERTEX_ARRAY || state == GL_COLOR_ARRAY || state == GL_TEXTURE_COORD_ARRAY) {
		glDisableClientState(state);
	}
}

void Renderer::myGLbindBuffer(int buffer, int vboID) {
	if (buffer == GL_ARRAY_BUFFER) {
		glBindBuffer(buffer, vboID);
	}
}

void Renderer::myGLcullFace(int face) {
	glCullFace(face);
}

void Renderer::myGLdepthFunc(int func) {
	glDepthFunc(func);
}

void Renderer::myGLdepthMask(bool mode) {
	glDepthMask(mode);
}

void Renderer::myGLDrawArrays(int mode, int first, int count) {
	if (mode == GL_QUADS) {
		glDrawArrays( GL_QUADS, first, count );
	}
}

void Renderer::myGLVertexPointer(int size, int type, int stride, void* pointer) {
	glVertexPointer(size, type, stride, pointer);
}

void Renderer::myGLColorPointer(int size, int type, int stride, void* pointer) {
	glColorPointer(size, type, stride, pointer);
}

void Renderer::myGLTexCoordPointer(int size, int type, int stride, void* pointer) {
	glTexCoordPointer(size, type, stride, pointer);
}

void Renderer::myGLBlendFunc(int src, int dst) {
	if (src == GL_SRC_ALPHA && dst == GL_ONE_MINUS_SRC_ALPHA) {
		glBlendFunc(src, dst);
	}
}

void Renderer::myGLgenBuffers(unsigned int n, unsigned int* buffer) {
	glGenBuffers(n, buffer);
}

void Renderer::myGLHint(int hint, int flag) {
	glHint(hint, flag);
}

void Renderer::myGLbufferData(int bufferTarget, int size, void* data, int usage) {
	glBufferData(bufferTarget, size, data, usage);
}

void Renderer::myGluPerspective( float fovY, float aspect, float zNear, float zFa ) {
	gluPerspective(fovY, aspect, zNear, zFa);
}


void Renderer::myGLdeleteBuffers(int buf, unsigned int vboID) {
	glDeleteBuffers(buf, &vboID);
}

void Renderer::myGLGenTextures(int mode, unsigned int* textureID) {
	glGenTextures( mode, (GLuint*) textureID);
}

void Renderer::myGLPixelStorei(int align, int mode) {
	glPixelStorei(align, mode);
}

void Renderer::myGLbindTexture(int type, int texID) {
	glBindTexture( type, texID );
}

void Renderer::myGLtexParameteri( int target, int pname, int param) {
	glTexParameteri( target, pname, param );
}

void Renderer::myGLgenerateMipmap(int mode) {
	glGenerateMipmap(mode); 
}


void Renderer::myGLMatrixMode(int mode) {
	glMatrixMode(mode);
}

void Renderer::myGLtexImage2D(int textype, int rootmipmap, int internalFormat, int sizeX, int sizeY, int borderSize, int bufferFormat, int componentType, unsigned char* imageData) {
	glTexImage2D(textype, rootmipmap, internalFormat, sizeX, sizeY, borderSize, bufferFormat, componentType, imageData);
}


// Call some function like this during startup, after creating window & OpenGL rendering context
void Renderer::InitializeAdvancedOpenGLFunctions()
{
	glGenBuffers		= (PFNGLGENBUFFERSPROC)		wglGetProcAddress( "glGenBuffers" );
	glBindBuffer		= (PFNGLBINDBUFFERPROC)		wglGetProcAddress( "glBindBuffer" );
	glBufferData		= (PFNGLBUFFERDATAPROC)		wglGetProcAddress( "glBufferData" );
	glGenerateMipmap	= (PFNGLGENERATEMIPMAPPROC)	wglGetProcAddress( "glGenerateMipmap" );
	glDeleteBuffers     = (PFNGLDELETEBUFFERSPROC)	wglGetProcAddress("glDeleteBuffers");
	glActiveTexture     = (PFNGLACTIVETEXTUREPROC)  wglGetProcAddress("glActiveTexture");
} 

void Renderer::myGLOrtho(double param1, const float SCREEN_WIDTH, double param3, const float SCREEN_HEIGHT, int param5, int param6)
{
	glOrtho(param1, SCREEN_WIDTH, param3, SCREEN_HEIGHT, param5, param6);
}

void Renderer::myGLAlphaFunc(unsigned int func, float ref)
{
	glAlphaFunc(func, ref);
}
