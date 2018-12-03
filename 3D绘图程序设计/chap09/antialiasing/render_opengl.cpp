#include <windows.h>

#include "glew.h" // OpenGL extension
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "GutWin32.h"
#include "GutModel_OpenGL.h"
#include "render_data.h"

static GLuint g_texture = 0;
static GLuint g_framebuffer = 0;
static GLuint g_depthbuffer = 0;

static int g_iFrameBufferWidth = 1024;
static int g_iFrameBufferHeight = 1024;

void InitState(float aspect)
{
	glDisable(GL_DEPTH_TEST);
}

bool InitResourceOpenGL(void)
{
	glDisable(GL_DEPTH_TEST);
	
	int width, height;
	GutGetWindowSize(width, height);
	
	g_iFrameBufferWidth = width * 2;
	g_iFrameBufferHeight = height * 2;

	// 打开一个framebuffer object
	glGenFramebuffersEXT(1, &g_framebuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_framebuffer);

	// 分配一块贴图空间给framebuffer object绘图使用 
	{
		glGenTextures(1, &g_texture);
		glBindTexture(GL_TEXTURE_2D, g_texture);
		// 设置filter
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// 声明贴图大小及格式
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  g_iFrameBufferWidth, g_iFrameBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		// framebuffer的RGBA绘图
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, g_texture, 0);
	}

	// 分配zbuffer给framebuffer object使用
	{
		glGenRenderbuffersEXT(1, &g_depthbuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, g_depthbuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, g_iFrameBufferWidth, g_iFrameBufferHeight);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, g_depthbuffer);
	}

	// 检查framebuffer object是否分配成功
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if ( status!=GL_FRAMEBUFFER_COMPLETE_EXT )
	{
		return false;
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	for ( int i=0; i<4; i++ )
	{
		g_Quad[i].m_Tex[1] = 1.0f - g_Quad[i].m_Tex[1];
	}

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	if ( g_framebuffer )
	{
		glDeleteFramebuffersEXT(1, &g_framebuffer);
		g_framebuffer = 0;
	}

	if ( g_texture )
	{
		glDeleteTextures(1, &g_texture);
		g_texture = 0;
	}

	if ( g_depthbuffer )
	{
		glDeleteRenderbuffersEXT(1, &g_depthbuffer);
		g_depthbuffer = 0;
	}

	return true;
}

// callback function. 窗口大小改变时会被调用, 并传入新的窗口大小.
void ResizeWindowOpenGL(int width, int height)
{
	// 使用新的窗口大小做为新的绘图分辨率
	glViewport(0, 0, width, height);
}

void RenderModel(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&g_world_matrix[0][0]);
	// 画出模型
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vector4), g_Triangle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void Antialiasing_None(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// `设置投影转换矩阵`
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// `画出模型`
	RenderModel();
	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}

static void Antialiasing_MultiSampling(void)
{
	glEnable(GL_MULTISAMPLE_ARB);
	Antialiasing_None();
	glDisable(GL_MULTISAMPLE_ARB);
}

static void Antialiasing_SuperSampling(void)
{
	// `设置投影转换矩阵`
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	{
		// `使用` g_framebuffer framebuffer object	
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_framebuffer);
		glViewport(0, 0, g_iFrameBufferWidth, g_iFrameBufferHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		// `画出模型`
		RenderModel();
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	{
		// `使用` g_framebuffer framebuffer object	
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		int w, h;
		GutGetWindowSize(w, h);
		glViewport(0, 0, w, h);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);

		sModelMaterial_OpenGL material;
		material.m_bCullFace = false;
		material.m_Textures[0] = g_texture;
		material.Submit(NULL);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VT), g_Quad[0].m_Position);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_VT), g_Quad[0].m_Tex);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}
	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}

void Antialiasing_SmoothLine(void)
{
	// `使用 OpenGL line smooth 功能`
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Antialiasing_None();
	
	glDisable(GL_BLEND);
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	if ( g_bWireframe )
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	switch(g_iMode)
	{
	case 1:
		Antialiasing_None();
		break;
	case 2:
		Antialiasing_MultiSampling();
		break;
	case 3:
		Antialiasing_SuperSampling();
		break;
	case 4:
		Antialiasing_SmoothLine();
		break;
	}
}
