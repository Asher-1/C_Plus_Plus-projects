#include <windows.h>
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "render_data.h"

bool InitResourceOpenGL(void)
{
	// 计算出一个可以转换到镜头坐标系的矩阵
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	g_Simulation.SetViewMatrix(view_matrix);
	// 设置投影矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, 1.0f, 0.1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);

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
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// 清除画面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// 把正向和反向的面都画出来
	glDisable(GL_CULL_FACE);
	// 设置要用数组的方式传入顶点位置和颜色
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VCN), &g_pGridVertices[0].m_Position);
	glColorPointer (4, GL_FLOAT, sizeof(Vertex_VCN), &g_pGridVertices[0].m_Color);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &g_Simulation.m_WorldViewMatrix);
	// 画出格子
	glDrawElements (GL_TRIANGLE_STRIP, g_iNumGridIndices, GL_UNSIGNED_SHORT, g_pGridIndices);
	// 把背景backbuffer的画面显示出来
	GutSwapBuffersOpenGL();
}
