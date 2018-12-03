#include <windows.h>

#include "glew.h" // OpenGL extension
#include <GL/gl.h>

#include "Gut.h"
#include "GutTexture_OpenGL.h"

#include "render_data.h"

static Matrix4x4 g_proj_matrix;

GLuint g_Texture0_ID = 0;
GLuint g_Texture1_ID = 0;

bool InitResourceOpenGL(void)
{

	// 投影矩阵
	g_proj_matrix = GutMatrixOrthoRH_OpenGL(g_fOrthoWidth, g_fOrthoHeight, 0.1f, 100.0f);
	//g_proj_matrix = GutMatrixPerspectiveRH_OpenGL(60.0f, 1.0f, 0.1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_proj_matrix);

	Vector4 a(0.0f, 0.0f, -0.1f, 1.0f);
	Vector4 b(0.0f, 0.0f, -100.0f, 1.0f);
	
	Vector4 ta = a * g_proj_matrix;
	Vector4 tb = b * g_proj_matrix;

	// 载入贴图
	g_Texture0_ID = GutLoadTexture_OpenGL("../../textures/brickwall.tga");
	// 使用Trilinear Filter
	glBindTexture(GL_TEXTURE_2D, g_Texture0_ID);
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	// 载入贴图
	g_Texture1_ID = GutLoadTexture_OpenGL("../../textures/spotlight_effect.tga");
	// 使用Trilinear Filter
	glBindTexture(GL_TEXTURE_2D, g_Texture1_ID);
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	glBindTexture(GL_TEXTURE_2D, g_Texture1_ID);

	// 把正向和反向的面都画出来
	glDisable(GL_CULL_FACE);
	// 启动2D贴图功能
	glEnable(GL_TEXTURE_2D);
	// 启动zbuffer隐藏面消除
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	if ( g_Texture0_ID )
	{
		// 释放贴图资源
		glDeleteTextures(1, &g_Texture0_ID);
		g_Texture0_ID = 0;
	}

	if ( g_Texture1_ID )
	{
		// 释放贴图资源
		glDeleteTextures(1, &g_Texture1_ID);
		g_Texture1_ID = 0;
	}

	return true;
}

// callback function. 窗口大小改变时会被调用, 并传入新的窗口大小.
void ResizeWindowOpenGL(int width, int height)
{
	// 使用新的窗口大小做为新的绘图分辨率
	glViewport(0, 0, width, height);
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	g_fOrthoWidth = g_fOrthoSize;
	g_fOrthoHeight = g_fOrthoSize;

	if ( aspect > 1.0f )
		g_fOrthoHeight *= aspect;
	else
		g_fOrthoWidth /= aspect;

	g_proj_matrix = GutMatrixOrthoRH_OpenGL(g_fOrthoWidth, g_fOrthoHeight, 0.1f, 100.0f);
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 设置要用数组的方式传入顶点位置和颜色
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// 在此不需要用到zbuffer
	glDisable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	{
		// 不再用单一颜色来清除画面,
		// 把整个画面用砖墙图片盖过去

		// 把modelview, projection矩阵设成单位矩阵
		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// 使用贴图
		glBindTexture(GL_TEXTURE_2D, g_Texture0_ID);
		// 画出矩形, 同时会清除ZBuffer
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VT), &g_FullScreenQuad[0].m_Position);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_VT), &g_FullScreenQuad[0].m_Texcoord);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// 把存放在stack中的projection取回来
		glPopMatrix();
	}

	{
		// 打开混合功能
		glEnable(GL_BLEND);
		// source_blend_factor = 1
		// dest_blend_factor = 1
		// 混合公式= source_color * 1 + dest_color * 1
		glBlendFunc(GL_ONE, GL_ONE);
		// 使用贴图
		glBindTexture(GL_TEXTURE_2D, g_Texture1_ID);
		// 代入转换矩阵
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf( (float *) &g_proj_matrix);
		Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
		Matrix4x4 world_view_matrix = g_world_matrix * view_matrix;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf( (float *) &world_view_matrix);
		// 画出矩形
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VT), &g_Quad[0].m_Position);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex_VT), &g_Quad[0].m_Texcoord);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		// 关闭混合功能		
		glDisable(GL_BLEND);
	}

	// 把背景backbuffer的画面显示出来
	GutSwapBuffersOpenGL();
}
