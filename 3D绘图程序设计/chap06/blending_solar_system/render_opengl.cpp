#include <windows.h>
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "render_data.h"

bool InitResourceOpenGL(void)
{
	// 投影矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(90.0f, 1.0f, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
	glMatrixMode(GL_MODELVIEW);	

	glEnable(GL_CULL_FACE);

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	// 没做任何事
	return true;
}

// callback function. 窗口大小改变时会被调用, 并传入新的窗口大小.
void ResizeWindowOpenGL(int width, int height)
{
	// 使用新的窗口大小做为新的绘图分辨率
	glViewport(0, 0, width, height);
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(60.0f, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// 清除画面
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// 设置要用数组的方式传入顶点位置和颜色
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	// 计算出一个可以转换到镜头坐标系的矩阵
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 world_view_matrix;

	glMatrixMode(GL_MODELVIEW);

	if ( g_bZWrite )
	{
		// 更新zbuffer
		glDepthMask(GL_TRUE);
	}
	else
	{
		// 不更新zbuffer
		glDepthMask(GL_FALSE);
	}
	
	glEnable(GL_BLEND);
	// 混合模式
	switch(g_iBlendMode)
	{
	case 0:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 1:
		glBlendFunc(GL_ONE, GL_ONE);
		break;
	}

	for ( int i=0; i<2; i++ )
	{
		switch( g_iOrder[i] )
		{
		case 0:
			// 太阳
			world_view_matrix = g_sun_matrix * g_rot_matrix * view_matrix;
			glLoadMatrixf( (float *) &world_view_matrix);
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), g_pSunVertices);
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), g_pSunVertices[0].m_RGBA);
			glDrawElements(GL_TRIANGLES, g_iNumSphereIndices, GL_UNSIGNED_SHORT, g_pSphereIndices);
			break;
		case 1:
			// 地球
			world_view_matrix = g_earth_matrix * g_rot_matrix * view_matrix;
			glLoadMatrixf( (float *) &world_view_matrix);
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), g_pEarthVertices);
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), g_pEarthVertices[0].m_RGBA);
			glDrawElements(GL_TRIANGLES, g_iNumSphereIndices, GL_UNSIGNED_SHORT, g_pSphereIndices);
			break;
		default:
			break;
		}
	}

	// 把背景backbuffer的画面显示出来
	GutSwapBuffersOpenGL();
}
