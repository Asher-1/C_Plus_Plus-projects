#include <windows.h>

#include "glew.h" // OpenGL extension
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "GutWin32.h"
#include "GutModel_OpenGL.h"

#include "render_data.h"

static Matrix4x4 g_view_matrix;
static Matrix4x4 g_projection_matrix;

static CGutModel_OpenGL g_models[2];

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

	g_models[0].ConvertToOpenGLModel(&g_sun_model);
	g_models[1].ConvertToOpenGLModel(&g_earth_model);

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
	g_projection_matrix = GutMatrixPerspectiveRH_OpenGL(60.0f, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_projection_matrix);
}

static void RenderSolarSystemOpenGL(bool bSelection)
{
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 world_matrix = g_Control.GetObjectMatrix();
	Matrix4x4 wv_matrix = world_matrix * view_matrix;
	Matrix4x4 world_view_matrix;

	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);

	// 太阳
	world_view_matrix = g_sun_matrix * wv_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);

	if ( bSelection ) glPushName(1); // 设置名称代码为1
	g_models[0].Render();
	if ( bSelection ) glPopName();

	// 地球
	world_view_matrix = g_earth_matrix * wv_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);

	if ( bSelection ) glPushName(2); // 设置名称代码为2
	g_models[1].Render();
	if ( bSelection ) glPopName();
}

#define BUFSIZE 1024
GLuint selectBuf[BUFSIZE];

// `使用OpenGL来绘图`
void RenderFrameOpenGL(void)
{
	// `获得窗口大小`
	int w, h;
	GutGetWindowSize(w, h);
	// `清除画面`
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// `在屏幕上画出行星运动`
	RenderSolarSystemOpenGL(false);

	/* 
		`启动Selection Buffer, 硬件会试着去重画一次3D对象,` 
		`但不会更新画面, 只会记录下落在画面中某个范围内所有3D对象.`
	*/
	int x,y,buttons[3];
	GutGetMouseState(x, y, buttons);
	if ( buttons[0] )
	{
		GLint viewport[4];

		glSelectBuffer(BUFSIZE,selectBuf);
		glRenderMode(GL_SELECT);
		glInitNames();

		glGetIntegerv(GL_VIEWPORT,viewport);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		// `设置Selection Buffer的检查范围`
		gluPickMatrix(x, viewport[3]-y, 3, 3, viewport);
		float fRatio = (viewport[2]+0.0) / viewport[3];
		glMultMatrixf( (float *)&g_projection_matrix );
		// `在Selection Buffer中画出3D对象`
		RenderSolarSystemOpenGL(true);
		int hits = glRenderMode(GL_RENDER);
		
		GLuint *ptr = selectBuf;
		GLuint Zmin = 0xffffffff;
		GLuint SelectedObject = 0;
		// `找出检查范围中, 距离镜头最近的物体.`
		for ( int i=0; i<hits; i++, ptr+=4 )
		{
			if ( ptr[1] < Zmin )
			{
				Zmin = ptr[1];
				SelectedObject = ptr[3];
			}
		}

		switch(SelectedObject)
		{
		default:
		case 0:
			printf("Nothing\r");
			break;
		case 1:
			printf("Sun    \r");
			break;
		case 2:
			printf("Earth  \r");
			break;
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
