#include <windows.h>
#include "glew.h"
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "GutTexture_OpenGL.h"

#include "render_data.h"

static Matrix4x4 g_view_matrix;
static GLuint g_quad_list[4];

GLuint g_TextureID = 0;

bool InitResourceOpenGL(void)
{
	const char *extension = (const char *)glGetString(GL_EXTENSIONS);
	printf("OpenGL Extension : \n%s\n", extension);

	// 计算出一个可以转换到镜头坐标系的矩阵
	g_view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// 设置投影矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, 1.0f, 0.1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);

	// 载入贴图
	g_TextureID = GutLoadTexture_OpenGL("../../textures/lena_rgba.tga");
	// 使用g_TextureID贴图对象
	glBindTexture( GL_TEXTURE_2D, g_TextureID );
	// 设置显示贴图时使用线性内插
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	// 设置显示贴图时使用线性外插
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	// 使用2D贴图功能
	glEnable(GL_TEXTURE_2D);

	GLint modes[] = {
		GL_REPEAT,// 左下角
		GL_MIRRORED_REPEAT, // 左上角
		GL_CLAMP_TO_EDGE, // 右下角
		GL_CLAMP_TO_BORDER, // 右上角
	};

	float border_color[4] = {0.5f, 0.5f, 0.5f, 0.5f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	Vertex_VT *vertex = NULL;

	for ( int i=0; i<4; i++ )
	{
		g_quad_list[i] = glGenLists(1);
		// `创建一笔新的Display List记录`
		glNewList(g_quad_list[i], GL_COMPILE);
			// `使用贴图`
			glBindTexture( GL_TEXTURE_2D, g_TextureID );
			// `使用贴图内插功能`
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// `设置贴图坐标寻址模式`
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, modes[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, modes[i]);
			// `画出看板`
			glBegin(GL_QUADS);
				vertex = &g_Quads[i][0];
				glTexCoord2f(vertex->m_Texcoord[0], vertex->m_Texcoord[1]);
				glVertex3fv(vertex->m_Position);

				vertex = &g_Quads[i][1];
				glTexCoord2f(vertex->m_Texcoord[0], vertex->m_Texcoord[1]);
				glVertex3fv(vertex->m_Position);

				vertex = &g_Quads[i][2];
				glTexCoord2f(vertex->m_Texcoord[0], vertex->m_Texcoord[1]);
				glVertex3fv(vertex->m_Position);

				vertex = &g_Quads[i][3];
				glTexCoord2f(vertex->m_Texcoord[0], vertex->m_Texcoord[1]);
				glVertex3fv(vertex->m_Position);
			glEnd();

		glEndList();
	}

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	if ( g_TextureID )
	{
		glDeleteTextures(1, &g_TextureID);
		g_TextureID = 0;
	}
	return true;
}

// callback function. 窗口大小改变时会被调用, 并传入新的窗口大小.
void ResizeWindowOpenGL(int width, int height)
{
	// 使用新的窗口大小做为新的绘图分辨率
	glViewport(0, 0, width, height);
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// `镜头坐标转换矩阵`
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// `转换矩阵`
	Matrix4x4 world_view_matrix = g_world_matrix * view_matrix;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &world_view_matrix);

	for ( int i=0; i<4; i++ )
	{
		// `重复Display List中所记录的动作`
		glCallList(g_quad_list[i]);
	}

	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
