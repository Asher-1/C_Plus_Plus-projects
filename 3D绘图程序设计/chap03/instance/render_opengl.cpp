#include <windows.h>
#include <GL/gl.h>

#include "Gut.h"
#include "render_data.h"

static Matrix4x4 g_view_matrix;

bool InitResourceOpenGL(void)
{
	// `计算出一个可以转换到镜头坐标系的矩阵`
	g_view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// `投影矩阵`
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(60.0f, 1.0f, 1.0f, 100.0f);
	// `设置视角转换矩阵`
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	// `没做任何事`
	return true;
}

// `使用OpenGL来绘图`
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// `设置好GPU要去哪读取顶点数据`
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(4, GL_FLOAT, sizeof(Vector4), g_vertices);
	// `设置要改变GL_MODELVIEW矩阵`
	glMatrixMode(GL_MODELVIEW);	

	// `4个金字塔的位置`
	Vector4 pos[4] = 
	{
		Vector4(-1.0f, -1.0f, 0.0f),
		Vector4( 1.0f, -1.0f, 0.0f),
		Vector4(-1.0f,  1.0f, 0.0f),
		Vector4( 1.0f,  1.0f, 0.0f),
	};

	Matrix4x4 world_matrix;
	Matrix4x4 world_view_matrix;

	for ( int i=0; i<4; i++ )
	{
		// `得到位移矩阵`
		world_matrix.Translate_Replace(pos[i]); 
		world_view_matrix = world_matrix * g_view_matrix;
		// `设置转换矩阵'
		glLoadMatrixf( (float *) &world_view_matrix);
		// `画出金字塔的8条边线`
		glDrawArrays(GL_LINES, 0, 16);
	}

	// `把背景backbuffer显示出来`
	GutSwapBuffersOpenGL();
}
