#include <windows.h>

#include "glew.h" // OpenGL extension
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "GutWin32.h"
#include "GutModel_OpenGL.h"
#include "render_data.h"

static Matrix4x4 g_projection_matrix;
static Matrix4x4 g_mirror_view_matrix;

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
	glEnable(GL_NORMALIZE);
	glDisable(GL_CULL_FACE);

	CGutModel::SetTexturePath("../../textures/");

	g_Model_OpenGL.ConvertToOpenGLModel(&g_Model);

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	g_Model_OpenGL.Release();

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

static void RenderModelOpenGL(bool mirror, Vector4 *pPlane)
{
	Matrix4x4 view_matrix;

	if ( mirror )
	{
		Vector4 vEye = g_Control.GetCameraPosition();
		Vector4 vLookAt = g_Control.m_vLookAt;
		Vector4 vUp = g_Control.m_vUp;

		Vector4 mirror_eye = MirrorPoint(vEye, *pPlane);
		Vector4 mirror_lookat = MirrorPoint(vLookAt, *pPlane);
		Vector4 mirror_up = MirrorVector(vUp, *pPlane);

		Matrix4x4 temp_matrix = GutMatrixLookAtRH(mirror_eye, mirror_lookat, mirror_up);

		// 因为是镜射, 在转换到镜头坐标系后要做个左右对调的动作.
		Matrix4x4 mirror_x;
		mirror_x.Identity();
		mirror_x.Scale(-1.0f, 1.0f, 1.0f);

		view_matrix = temp_matrix * mirror_x;
		g_mirror_view_matrix = view_matrix;

		// 右左对调后, 3角形的顶点排列顺序会被反过来.
		glFrontFace(GL_CW);
	}
	else
	{
		view_matrix = g_Control.GetViewMatrix();
		glFrontFace(GL_CCW);
	}

	glMatrixMode(GL_MODELVIEW);
	Matrix4x4 world_view_matrix = g_Control.GetObjectMatrix() * view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);

	g_Model_OpenGL.Render();

	glFrontFace(GL_CCW);
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	Vector4 vPlane(0.0f, 0.0f, 1.0f, -g_mirror_z);

	// `清除画面`
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// `画出茶壶`
	RenderModelOpenGL(false, NULL);
	// `设置转换矩阵`
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();;
	Matrix4x4 world_view_matrix = view_matrix;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &world_view_matrix);

	sModelMaterial_OpenGL material;
	material.Submit(NULL);
	// `设置顶点数据格式`
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_V), &g_Quad_v[0].m_Position);

	// `画出镜子, 同时把镜子部分的stencil buffer设为1.`
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	// `把镜子部分的zbuffer清为1.0`
	{
		glStencilFunc(GL_EQUAL, 1, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		// `把z值的输出范围设置为1~1, 也就是z永远输出1.`
		glDepthRange(1.0f, 1.0f);
		glDepthFunc(GL_ALWAYS);
		// `只更新zbuffer,不需要更新颜色.`
		glColorMask(false, false, false, false);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		// `恢复更新颜色的功能`
		glColorMask(true, true, true, true);
		glDepthFunc(GL_LESS);
		// `把z值的范围还原为0~1`
		glDepthRange(0.0f, 1.0f);
	}
	// `画出镜子里的茶壶`
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		RenderModelOpenGL(true, &vPlane);
		glDisable(GL_STENCIL_TEST);
	}
	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
