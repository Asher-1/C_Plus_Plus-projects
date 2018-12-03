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
	// 设置投影矩阵
	Matrix4x4 projection_matrix = GutMatrixOrthoRH_OpenGL(g_fOrthoWidth, g_fOrthoHeight, 0.1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);

	// 把正向和反向的面都画出来
	glDisable(GL_CULL_FACE);

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
	g_fOrthoWidth = g_fOrthoSize;
	g_fOrthoHeight = g_fOrthoSize;
	if ( aspect > 1.0f )
		g_fOrthoHeight *= aspect;
	else
		g_fOrthoWidth /= aspect;

	Matrix4x4 projection_matrix = GutMatrixOrthoRH_OpenGL(g_fOrthoWidth, g_fOrthoHeight, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
}

static void SetupLighting(void)
{
	// 打开光照功能
	glEnable(GL_LIGHTING);
	// `设置环境光`
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float *)&g_vAmbientLight);
	// `设置对象纹理的反光能力`
	Vector4 vFullIntensity(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 vZeroIntensity(0.0f, 0.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, (float *) &vFullIntensity);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (float *) &vFullIntensity);
	glMaterialfv(GL_FRONT, GL_SPECULAR, (float *) &vFullIntensity);
	glMaterialfv(GL_FRONT, GL_EMISSION, (float *) &vZeroIntensity);
	glMaterialf (GL_FRONT, GL_SHININESS, g_fMaterialShininess);

	int i;
	for ( i=0; i<g_iNumActivatedLights; i++ )
	{
		int LightID = GL_LIGHT0 + i;

		glEnable(LightID);

		glLightfv(LightID, GL_POSITION, (float *) &g_ActivatedLights[i].m_Position);
		glLightfv(LightID, GL_DIFFUSE, (float *) &g_ActivatedLights[i].m_Diffuse);
		glLightfv(LightID, GL_SPECULAR, (float *) &g_ActivatedLights[i].m_Specular);

		glLightf(LightID, GL_CONSTANT_ATTENUATION,	1.0f);
		glLightf(LightID, GL_LINEAR_ATTENUATION,	0.0f);
		glLightf(LightID, GL_QUADRATIC_ATTENUATION, 0.0f);
	}

	for ( ; i<MAX_ACTIVATED_LIGHTS; i++ )
	{
		glDisable(GL_LIGHT0+i);
	}
}

// `使用OpenGL来绘图`
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// `用点来画出光源位置`	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &g_view_matrix);
	// `暂时关闭光照功能`
	glDisable(GL_LIGHTING);
	// `设置画点时使用5x5个像素来画一个点`
	glPointSize(5.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VCN), &g_LightPosition[0].m_Position);
	glColorPointer (4, GL_FLOAT, sizeof(Vertex_VCN), &g_LightPosition[0].m_Color);
	glDrawArrays(GL_POINTS, 0, NUM_LIGHTS);

	SetupLighting();

	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VCN), &g_Quad[0].m_Position);
	glNormalPointer(GL_FLOAT, sizeof(Vertex_VCN), &g_Quad[0].m_Normal);
	// `计算出一个可以转换到镜头坐标系的矩阵`
	Matrix4x4 world_view_matrix = g_world_matrix * g_view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);
	// `画一片矩形来看光照效果`
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
