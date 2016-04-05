#pragma once
#include "Engine/Math/Vec3.hpp"

#define WGL_CURRENT_BIT                    0x00000001

/* TextureMagFilter */
#define WGL_NEAREST                        0x2600
#define WGL_LINEAR                         0x2601

/* TextureParameterName */
#define WGL_TEXTURE_MAG_FILTER             0x2800
#define WGL_TEXTURE_MIN_FILTER             0x2801
#define WGL_TEXTURE_WRAP_S                 0x2802
#define WGL_TEXTURE_WRAP_T                 0x2803

/* TextureMinFilter */
#define WGL_NEAREST_MIPMAP_NEAREST         0x2700
#define WGL_LINEAR_MIPMAP_NEAREST          0x2701
#define WGL_NEAREST_MIPMAP_LINEAR          0x2702
#define WGL_LINEAR_MIPMAP_LINEAR           0x2703

/*POLYGON STUFF */
#define WGL_POINTS                         0x0000
#define WGL_LINES                          0x0001
#define WGL_QUADS                          0x0007
#define WGL_POLYGON                        0x0009

/* DataType */
#define WGL_BYTE                           0x1400
#define WGL_UNSIGNED_BYTE                  0x1401
#define WGL_SHORT                          0x1402
#define WGL_INT                            0x1404
#define WGL_UNSIGNED_INT                   0x1405
#define WGL_FLOAT                          0x1406

/* PixelFormat */
#define WGL_RGB                            0x1907
#define WGL_RGBA                           0x1908

/* Tex stuff */
#define WGL_TEXTURE_2D                     0x0DE1
#define WGL_UNPACK_ALIGNMENT               0x0CF5

/* TextureWrapMode */
#define WGL_CLAMP                          0x2900
#define WGL_REPEAT                         0x2901
#define WGL_TEXTURE_MAX_LEVEL              0x813D
#define WGL_ARRAY_BUFFER                   0x8892
/* Array */
#define WGL_VERTEX_ARRAY                   0x8074
#define WGL_COLOR_ARRAY                    0x8076
#define WGL_TEXTURE_COORD_ARRAY            0x8078

/* draw modes */
#define WGL_STATIC_DRAW                    0x88E4

/* depth data */

#define WGL_DEPTH_TEST                     0x0B71
#define WGL_ALPHA_TEST                     0x0BC0

/* Boolean */
#define WGL_TRUE                           1
#define WGL_FALSE                          0

#define WGL_CULL_FACE                      0x0B44
#define WGL_LEQUAL                         0x0203
#define WGL_GREATER                        0x0204

/* FrontFaceDirection */
#define WGL_CW                             0x0900
#define WGL_CCW                            0x0901

#define WGL_FRONT                          0x0404
#define WGL_BACK                           0x0405
#define WGL_FRONT_AND_BACK                 0x0408

#define WGL_PROJECTION                     0x1701
#define WGL_MODELVIEW                      0x1700


class Renderer
{
public:
	Renderer(void);
	~Renderer(void);
	void myGLBlendFunc(int src, int dst);
	void myGLColorPointer(int size, int type, int stride, void* pointer);
	void myGLVertexPointer(int size, int type, int stride, void* pointer);
	void myGLDrawArrays(int mode, int first, int count);
	void myGLbindBuffer(int buffer, int vboID);
	void myGLDisableClientState(int state);
	void myGLEnableClientState( int state);
	void myGLtexCoord2f(float firstCoord, float secondCoord);
	void myGLfrontFace(int attrib);
	void myGLpushAttrib(int attrib);
	void myGLglColor4f(float r, float g, float b, float a);
	void myGLEnable(int flag);
	void myGLDisable(int mode);
	void myGLPopMatrix();
	void myGLPushMatrix();
	void myGLend();
	void myGLbegin(int mode);
	void myGLcolor3f(float r, float g, float b);
	void myGLVertex3f(float x, float y, float z);
	void myGLrotatef(float angle, float x, float y, float z);
	void myGLtranslatef(float x, float y, float z);
	void myGLHint(int hint, int flag);
	void myGLloadIdentity();
	void myGLTexCoordPointer(int size, int type, int stride, void* pointer);
	void myGLLineWidth(float lineWidth);
	void myGLgenBuffers(unsigned int n, unsigned int* buffer);
	void InitializeAdvancedOpenGLFunctions();
	void myGLbufferData(int bufferTarget, int size, void* data, int usage);
	void myGluPerspective( float fovY, float aspect, float zNear, float zFa );
	void myGLPixelStorei( int align, int mode );
	void myGLdeleteBuffers(int buf, unsigned int vboID);
	void myGLGenTextures(int mode, unsigned int* textureID);
	void myGLbindTexture(int type, int texID);
	void myGLtexParameteri( int target, int pname, int param);
	void myGLgenerateMipmap(int mode);
	void myGLPopAttrib();
	void myGLpointSize(float size);
	void myGLdepthMask(bool mode);
	void myGLdepthFunc(int func);
	void myGLcullFace(int face);
	void myGLMatrixMode(int mode);
	void myGLOrtho(double param1, const float SCREEN_WIDTH, double param3, const float SCREEN_HEIGHT, int param5, int param6);
	void myGLtexImage2D(int textype, int rootmipmap, int internalFormat, int sizeX, int sizeY, int borderSize, int bufferFormat, int componentType, unsigned char* imageData);
	void myGLAlphaFunc(unsigned int func, float ref);
};

