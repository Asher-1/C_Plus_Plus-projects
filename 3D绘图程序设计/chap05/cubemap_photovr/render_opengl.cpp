#include <windows.h>

#include "glew.h" // OpenGL extension
#include <GL/gl.h>

#include "Gut.h"
#include "GutTexture_OpenGL.h"
#include "GutModel_OpenGL.h"

#include "render_data.h"

GLuint g_TextureID = 0;

bool InitResourceOpenGL(void)
{
	// 设置投影矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, 1.0f, 0.1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
	// 载入贴图
	const char *texture_array[] = 
	{
		"../../textures/uffizi_right.tga",
		"../../textures/uffizi_left.tga",
		"../../textures/uffizi_top.tga",
		"../../textures/uffizi_bottom.tga",
		"../../textures/uffizi_back.tga", // `右手坐标系上 Z+ 为镜头后方.`
		"../../textures/uffizi_front.tga" // `右手坐标系上 Z- 为镜头前方.`
	};

	g_TextureID = GutLoadCubemapTexture_OpenGL(texture_array);

	// 使用CUBEMAP贴图功能
	glEnable(GL_TEXTURE_CUBE_MAP);
	// 使用CUBEMAP贴图
	glBindTexture( GL_TEXTURE_CUBE_MAP, g_TextureID );

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
	// `把正向和反向的面都画出来`
	glDisable(GL_CULL_FACE);
	// `使用CUBEMAP贴图功能`
	glEnable(GL_TEXTURE_CUBE_MAP);
	// `使用CUBEMAP贴图`
	glBindTexture( GL_TEXTURE_CUBE_MAP, g_TextureID );
	// Trilinear filter
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	// `设置转换矩阵`
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 world_matrix = g_Control.GetObjectMatrix();
	Matrix4x4 world_view_matrix = world_matrix * view_matrix;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &world_view_matrix);
	// `设置Vertex Array`
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VN), g_pSphereVertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex_VN), g_pSphereVertices[0].m_Normal);
	// `画球`
	glDrawElements(
		GL_TRIANGLES, // `指定所要画的基本图形种类`
		g_iNumSphereIndices, // `有几个索引值`
		GL_UNSIGNED_SHORT, // `索引值的类型`
		g_pSphereIndices // `索引值数组`
	);
	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
