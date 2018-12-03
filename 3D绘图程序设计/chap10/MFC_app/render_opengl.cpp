#include <windows.h>
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "render_data.h"

static Matrix4x4 g_view_matrix;

bool InitResourceOpenGL(void)
{
	// 计算出一个可以转换到镜头坐标系的矩阵
	g_view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// 投影矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(90.0f, 1.0f, 1.0f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
	glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_FLAT); // 关闭内插
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 使用昼边线模式
	// glEnable(GL_CULL_FACE); // 忽略背对镜头的面
	return true;
}

bool ReleaseResourceOpenGL(void)
{
	// 没做任何事
	return true;
}

void ResizeWindowOpenGL(int w, int h)
{
	glViewport(0, 0, w, h);
	float aspect = (float) h / (float) w;
	// 投影矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(90.0f, aspect, 1.0f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
}

// `使用OpenGL来绘图`
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// `设置好GPU要去哪读取顶点坐标数据`
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), &g_vertices[0].m_Position);
	// `设置好GPU要去哪读取顶点颜色数据`
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), &g_vertices[0].m_RGBA);
	// `设置要改变GL_MODELVIEW矩阵`
	glMatrixMode(GL_MODELVIEW);	

	static float angle = 0.0f;
	angle += 0.01f;

	// `设置转换矩阵`
	Matrix4x4 world_view_matrix = g_view_matrix;
	// `乘上旋转矩阵`
	world_view_matrix.RotateZ(angle); 
	glLoadMatrixf( (float *) &world_view_matrix);
	// `画出金字塔的8条边线`
	glDrawElements(
		GL_TRIANGLES, // `指定所要画的基本图形种类`
		18, // `有几个索引值`
		GL_UNSIGNED_SHORT, // `索引值的类型`
		g_indices // `索引值数组`
	);

	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
