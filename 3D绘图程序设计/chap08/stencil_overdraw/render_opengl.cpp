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

bool InitResourceOpenGL(void)
{
	// 投影矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFOV, 1.0f, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
	glMatrixMode(GL_MODELVIEW);	

	glEnable(GL_DEPTH_TEST);

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
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFOV, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 world_matrix = g_Control.GetObjectMatrix();
	Matrix4x4 world_view_matrix = world_matrix * view_matrix;

	// `画出茶壶, 并更新 Stencil Buffer.`
	{
		// `画出茶壶`
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf( (float *) &g_projection_matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf( (float *) &world_view_matrix);

		// `启动 Stencil Test`
		glEnable(GL_STENCIL_TEST);
		// `设置 Stencil Test 条件, 让它永远成立.`
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		// `递增 Stencil Buffer 内容`
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		// `画出茶壶`
		g_Model_OpenGL.Render();
		// `不再去改变 Stencil Buffer 内容`
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	sModelMaterial_OpenGL material;
	material.Submit(NULL);

	// `让它盖满整个画面, 不需要隐藏面消除.`
	glDepthFunc(GL_ALWAYS);

	// `设置顶点数据格式`
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_V), &g_Quad[0].m_Position);

	// `用绿色标示只更新过1次的像素`
	{
		glStencilFunc(GL_EQUAL, 1, 0xff);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	// `用蓝色标示更新过2次的像素`
	{
		glStencilFunc(GL_EQUAL, 2, 0xff);
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	// `用红色来标示更新超过3次以上的像素`
	{
		glStencilFunc(GL_EQUAL, 3, 0xff);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	// `用红色来标示更新超过3次以上的像素`
	{
		// `如果3小于stencil buffer的值, 条件才成立. `
		// `也就是stencil buffer值要大于3的意思`
		glStencilFunc(GL_LESS, 3, 0xff); 
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	// `还原设置`
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDepthFunc(GL_LESS);
	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
