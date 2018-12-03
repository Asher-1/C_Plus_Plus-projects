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

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// `设置要用数组的方式传入顶点位置和颜色`
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// `计算出一个可以转换到镜头坐标系的矩阵`
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);

	Vector4 border(-15.0f, 0.0f, -15.0f);
	Vector4 grid_position = border;

	const int grids_x = 30;
	const int grids_z = 30;

	for ( int x=0; x<grids_x; x++ )
	{
		int grid_x = x & 0x07;
		grid_position[2] = border[2];

		for ( int z=0; z<grids_z; z++ )
		{
			int grid_z = z & 0x07;
			char c = g_map[grid_x][grid_z];

			// `设置转换矩阵`
			Matrix4x4 object_matrix; object_matrix.Identity();
			
			if ( c==0 )
			{
				object_matrix = view_matrix;
				object_matrix.Translate(grid_position);
				glLoadMatrixf( (float *) &object_matrix);
				// `设置GPU要去哪读取顶点坐标数据`
				glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), &g_road_vertices[0].m_Position);
				// `设置GPU要去哪读取顶点颜色数据`
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), &g_road_vertices[0].m_RGBA);
				// `画出地板`
				glDrawElements(
					GL_TRIANGLE_STRIP, // `指定所要画的基本图形种类`
					4, // `有几个索引值`
					GL_UNSIGNED_SHORT, // `索引值的类型`
					g_road_trianglestrip_indices // `索引值数组`
				);
			}
			else
			{
				// `设置金字塔的高度`
				object_matrix.Scale_Replace(1.0f, (float) c, 1.0f);
				object_matrix[3] = grid_position;
				Matrix4x4 world_view_matrix = object_matrix * view_matrix;
				glLoadMatrixf( (float *) &world_view_matrix);
				// `设置GPU要去哪读取顶点坐标数据`
				glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), &g_pyramid_vertices[0].m_Position);
				// `设置GPU要去哪读取顶点颜色数据`
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), &g_pyramid_vertices[0].m_RGBA);
				// `画出金字塔`
				glDrawElements(
					GL_TRIANGLE_FAN, // `指定所要画的基本图形种类`
					6, // `有几个索引值`
					GL_UNSIGNED_SHORT, // `索引值的类型`
					g_pyramid_trianglefan_indices // `索引值数组`
				);
			}

			grid_position[2] += 1.0f;
		}

		grid_position[0] += 1.0f;
	}

	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
