#include <windows.h>

#include "glew.h" // OpenGL extension
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "GutWin32.h"
#include "GutModel_OpenGL.h"
#include "render_data.h"

static Matrix4x4 g_projection_matrix;

static CGutModel_OpenGL g_Model_OpenGL;
static Vector4 g_vFogColor(0.5f, 0.5f, 0.5f, 1.0f);

void InitState(float aspect)
{
	// 投影矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFOV, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
	glMatrixMode(GL_MODELVIEW);	

	glEnable(GL_DEPTH_TEST);
}

bool InitResourceOpenGL(void)
{
	InitState(1.0f);

	CGutModel::SetTexturePath("../../textures/");
	g_Model_OpenGL.ConvertToOpenGLModel(&g_Model);

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	return true;
}

// callback function. 窗口大小改变时会被调用, 并传入新的窗口大小.
void ResizeWindowOpenGL(int width, int height)
{
	// 使用新的窗口大小做为新的绘图分辨率
	glViewport(0, 0, width, height);
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	InitState(aspect);
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// 清除画面
	glClearColor(g_vFogColor[0], g_vFogColor[1], g_vFogColor[2], g_vFogColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// 计算出一个可以转换到镜头坐标系的矩阵
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 object_matrix = g_Control.GetObjectMatrix();
	Matrix4x4 world_view_matrix = object_matrix * view_matrix;
	// 设置投影转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
	// 设置镜头转换矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &world_view_matrix);

	// 设置雾
	glEnable(GL_FOG); // 打开雾的功能
	glFogfv(GL_FOG_COLOR, &g_vFogColor[0]); // 颜色
	
	switch(g_iFogMode)
	{
	case 0:
		glDisable(GL_FOG);
		break;
	case 1:
		// 雾随距离线性变浓
		glFogi(GL_FOG_MODE, GL_LINEAR);
		// 设置雾的范围
		glFogf(GL_FOG_START, 0.0f);
		glFogf(GL_FOG_END, 10.0f);
		// 计算公式为
		// (fog_end - distance_to_camera) / (fog_end - fog_start)
		break;
	case 2:
		// `使用指数函数来变化的雾 `
		glFogi(GL_FOG_MODE, GL_EXP);
		glFogf(GL_FOG_DENSITY, 0.5f);
		// 计算公式为
		// power(e, -(fog_density * distance_to_camera))
		break;
	case 3:
		// `使用指数函数来变化的雾 `
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogf(GL_FOG_DENSITY, 0.5f);
		// 计算公式为
		// power(e, -(fog_density * distance_to_camera)^2)
		break;
	}

	// 画出模型
	g_Model_OpenGL.Render();
	// 把背景backbuffer的画面显示出来
	GutSwapBuffersOpenGL();
}
