#include <windows.h>

#include "glew.h" // OpenGL extension
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "GutModel_OpenGL.h"
#include "GutWin32.h"
#include "render_data.h"

static Matrix4x4 g_view_matrix;
static Matrix4x4 g_projection_matrix;
static CGutModel_OpenGL g_Models_OpenGL[3];

bool InitResourceOpenGL(void)
{
	// 投影矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(70.0f, 1.0f, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
	glMatrixMode(GL_MODELVIEW);	

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	for ( int i=0; i<3; i++ )
	{
		g_Models_OpenGL[i].ConvertToOpenGLModel(&g_Models[i]);
	}

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
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(70.0f, aspect, 0.1f, 100.0f);
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
		glLightf(LightID, GL_LINEAR_ATTENUATION,	1.0f);
		glLightf(LightID, GL_QUADRATIC_ATTENUATION, 0.0f);
	}

	for ( ; i<8; i++ )
	{
		glDisable(GL_LIGHT0 + i);
	}
}

static void RenderSolarSystemOpenGL(void)
{
	// 设置要用数组的方式传入顶点位置和颜色
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	Matrix4x4 world_view_matrix;

	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);

	// 太阳
	world_view_matrix = g_sun_matrix * g_view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);
	g_Models_OpenGL[0].Render();
	// 地球
	world_view_matrix = g_earth_matrix * g_view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);
	g_Models_OpenGL[1].Render();
	// 月球
	world_view_matrix = g_moon_matrix * g_view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);
	g_Models_OpenGL[2].Render();
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// `获得窗口大小`
	int w, h;
	GutGetWindowSize(w, h);
	// `清除画面`
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Vector4 camera_lookat(0.0f, 0.0f, 0.0f);
	Matrix4x4 ortho_proj = GutMatrixOrthoRH_OpenGL(20.0f, 20.0f, 0.1f, 100.0f);

	{
		// `前视图`
		glViewport(0, h/2, w/2, h/2);
		// view matrix
		Vector4 camera_pos(0.0f, -20.0f, 0.0f);
		Vector4 camera_up(0.0f, 0.0f, 1.0f);
		g_view_matrix = GutMatrixLookAtRH(camera_pos, camera_lookat, camera_up);
		// projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&ortho_proj[0][0]);
		// render objects
		RenderSolarSystemOpenGL();
	}
	{
		// `上视图`
		glViewport(w/2, h/2, w/2, h/2);
		// view matrix
		Vector4 camera_pos(0.0f, 0.0f, 20.0f);
		Vector4 camera_up(0.0f, 1.0f, 0.0f);
		g_view_matrix = GutMatrixLookAtRH(camera_pos, camera_lookat, camera_up);
		// projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&ortho_proj[0][0]);
		// render objects
		RenderSolarSystemOpenGL();
	}
	{
		// `右视图`
		glViewport(0, 0, w/2, h/2);
		// view matrix
		Vector4 camera_pos(20.0f, 0.0f, 0.0f);
		Vector4 camera_up(0.0f, 0.0f, 1.0f);
		g_view_matrix = GutMatrixLookAtRH(camera_pos, camera_lookat, camera_up);
		// projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&ortho_proj[0][0]);
		// render objects
		RenderSolarSystemOpenGL();
	}
	{
		// `使用者视角`
		glViewport(w/2, 0, w/2, h/2);
		// object * view matrix
		Matrix4x4 object_matrix = g_Control.GetObjectMatrix();
		Matrix4x4 view_matrix = g_Control.GetViewMatrix();
		g_view_matrix = object_matrix * view_matrix;
		// projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&g_projection_matrix[0][0]);
		// render objects
		RenderSolarSystemOpenGL();
	}
	{
		// `画出viewport的边界`
		glViewport(0, 0, w, h);

		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

		glEnableClientState(GL_VERTEX_ARRAY);
		// projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// worldview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// `画边界`
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), g_Border);
		glDrawArrays(GL_LINES, 0, 4);

		glPopClientAttrib();
	}
	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
