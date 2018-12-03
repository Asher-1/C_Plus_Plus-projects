#include <windows.h>

#include "glew.h" // OpenGL extension
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "GutWin32.h"
#include "render_data.h"

static Matrix4x4 g_view_matrix;
static Matrix4x4 g_projection_matrix;

const int NumObjects = 2;
GLuint g_OcclusionQueryObjects[NumObjects];

bool InitResourceOpenGL(void)
{
	// 计算出一个可以转换到镜头坐标系的矩阵
	g_view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// 投影矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(90.0f, 1.0f, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
	glMatrixMode(GL_MODELVIEW);	

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glGenQueries(2, g_OcclusionQueryObjects);

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	glDeleteQueries(2, g_OcclusionQueryObjects);

	return true;
}

// callback function. 窗口大小改变时会被调用, 并传入新的窗口大小.
void ResizeWindowOpenGL(int width, int height)
{
	// 使用新的窗口大小做为新的绘图分辨率
	glViewport(0, 0, width, height);
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(60.0f, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
}

GLuint g_FrameCount = 0;

static void RenderSolarSystemOpenGL(void)
{
	int index = g_FrameCount % 2;
	GLuint id = g_OcclusionQueryObjects[index];

	// 设置要用数组的方式传入顶点位置和颜色
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	Matrix4x4 world_view_matrix;

	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);

	// 太阳
	world_view_matrix = g_sun_matrix * g_view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), g_pSunVertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), g_pSunVertices[0].m_RGBA);
	glDrawElements(GL_TRIANGLES, g_iNumSphereIndices, GL_UNSIGNED_SHORT, g_pSphereIndices);
	// 地球
	// 检查范围开始
	glBeginQuery(GL_SAMPLES_PASSED, id);
		world_view_matrix = g_earth_matrix * g_view_matrix;
		glLoadMatrixf( (float *) &world_view_matrix);
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), g_pEarthVertices);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), g_pEarthVertices[0].m_RGBA);
		glDrawElements(GL_TRIANGLES, g_iNumSphereIndices, GL_UNSIGNED_SHORT, g_pSphereIndices);
	// 检查范围结束
	glEndQuery(GL_SAMPLES_PASSED);
	// 月球
	world_view_matrix = g_moon_matrix * g_view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), g_pMoonVertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), g_pMoonVertices[0].m_RGBA);
	glDrawElements(GL_TRIANGLES, g_iNumSphereIndices, GL_UNSIGNED_SHORT, g_pSphereIndices);

	if ( g_FrameCount )
	{
		id = g_OcclusionQueryObjects[(index + 1) % 2];
		GLint samples_passed = 0;
		GLint result = GL_FALSE;
		int num_loops = 0;

		while(result==GL_FALSE)
		{
			glGetQueryObjectiv(id, GL_QUERY_RESULT_AVAILABLE, &result);
			// 结果可能还没出来, 要再查询一次
			num_loops++;
		}
		// 查看有几个点被更新// 查看有几个点被更新
		glGetQueryObjectiv(id, GL_QUERY_RESULT, &samples_passed);
		printf("Earth %s\t\r", samples_passed ? "visible" : "disappear");
	}

	g_FrameCount++;
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// 获得窗口大小
	int w, h;
	GutGetWindowSize(w, h);
	// 清除画面
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// object * view matrix
	Matrix4x4 object_matrix = g_Control.GetObjectMatrix();
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	g_view_matrix = object_matrix * view_matrix;
	// projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&g_projection_matrix[0][0]);
	// render objects
	RenderSolarSystemOpenGL();
	// 把背景backbuffer的画面显示出来
	GutSwapBuffersOpenGL();
}
