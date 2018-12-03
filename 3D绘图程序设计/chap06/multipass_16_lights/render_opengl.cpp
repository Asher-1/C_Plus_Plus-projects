#include <windows.h>

#include "glew.h" // OpenGL extension
#include <GL/gl.h>

#include "Gut.h"
#include "GutTexture_OpenGL.h"
#include "GutModel_OpenGL.h"

#include "render_data.h"

static Matrix4x4 g_projection_matrix;

CGutModel_OpenGL g_Model_OpenGL;

bool InitResourceOpenGL(void)
{
	// 投影矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, 1.0f, 0.1f, 1000.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);

	CGutModel::SetTexturePath("../../textures/");

	g_Model_OpenGL.ConvertToOpenGLModel(&g_Model);

	// 把正向和反向的面都画出来
	glDisable(GL_CULL_FACE);
	// 启动2D贴图功能
	glEnable(GL_TEXTURE_2D);
	// 启动zbuffer隐藏面消除
	glEnable(GL_DEPTH_TEST);

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
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
}

void SetupLightingOpenGL(Light *pLightArray, int num_lights)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &g_vAmbientLight[0]);

	int i;

	for ( i=0; i<num_lights; i++ )
	{
		int LightID = GL_LIGHT0 + i;

		Light *pLight = pLightArray + i;

		glEnable(LightID);
		
		glLightfv(LightID, GL_POSITION, &pLight->m_Position[0]);
		glLightfv(LightID, GL_DIFFUSE, &pLight->m_Diffuse[0]);
		glLightfv(LightID, GL_SPECULAR, &pLight->m_Specular[0]);

		glLightf(LightID, GL_CONSTANT_ATTENUATION,	0.0f);
		glLightf(LightID, GL_LINEAR_ATTENUATION,	2.0f);
		glLightf(LightID, GL_QUADRATIC_ATTENUATION, 0.0f);
	}

	for ( ; i<8; i++ )
	{
		glDisable(GL_LIGHT0 + i);
	}
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// `把隐藏面消除规则设为小于或等于` <=
	glDepthFunc(GL_LEQUAL);

	// `设置转换矩阵`
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 world_matrix = g_Control.GetObjectMatrix();
	Matrix4x4 world_view_matrix = world_matrix * view_matrix;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &world_view_matrix);

	// `前8盏光`
	if ( g_iSwitch & 0x01 )
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf( (float *) &view_matrix);
		SetupLightingOpenGL(&g_Lights[0], 8);

		glLoadMatrixf( (float *) &world_view_matrix);
		g_Model_OpenGL.Render();
	}

	// `后8盏光`
	if ( g_iSwitch & 0x02 )
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf( (float *) &view_matrix);
		SetupLightingOpenGL(&g_Lights[8], 8);

		glLoadMatrixf( (float *) &world_view_matrix);

		if ( g_iSwitch & 0x01 )
		{
			// 如果前8盏光有开, 要使用混合来把下8盏光叠加上去.
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			// ~SUBMIT_BLEND 会取消使用档案中的混合设置.
			g_Model_OpenGL.Render(~SUBMIT_BLEND);
		}
		else
		{
			g_Model_OpenGL.Render();
		}
	}

	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
