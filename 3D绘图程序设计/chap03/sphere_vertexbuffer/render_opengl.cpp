#include <windows.h>
// OpenGL 1.2+ & extensions
#include "glew.h"
#include "wglew.h"
// OpenGL 1.2-
#include <GL/gl.h>

#include "Gut.h"
#include "render_data.h"

static GLuint g_VertexBufferID = 0;
static GLuint g_IndexBufferID = 0;

bool InitResourceOpenGL(void)
{
	// `投影矩阵`
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(90.0f, 1.0f, 0.1f, 100.0f);
	// `设置视角转换矩阵`
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
	
	glMatrixMode(GL_MODELVIEW);	

	// `生成一块新的Vertex Buffer Object`
	glGenBuffers(1, &g_VertexBufferID);
	// `使用g_VertexBufferID所代表的Vertex Buffer Object`
	glBindBuffer(GL_ARRAY_BUFFER, g_VertexBufferID);
	// `分配Vertex Buffer的大小, 并同时把g_pSphereVertices的内容拷贝过进去.`
	glBufferData(GL_ARRAY_BUFFER, g_iNumSphereVertices * sizeof(Vertex_VC), g_pSphereVertices, GL_STATIC_COPY);

	// `生成一块新的Index Buffer Object`
	glGenBuffers(1, &g_IndexBufferID);
	// `使用g_IndexBufferID所代表的Index Buffer Object`
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IndexBufferID);
	// `分配Index Buffer的大小, 并同时把g_pSphereIndices的内容拷贝过去.`
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_iNumSphereIndices * 2, g_pSphereIndices, GL_STATIC_COPY);

	return true;
}

bool ReleaseResourceOpenGL(void)
{
	if ( g_VertexBufferID )
	{
		glDeleteBuffers(1, &g_VertexBufferID);
		g_VertexBufferID = 0;
	}

	if ( g_IndexBufferID )
	{
		glDeleteBuffers(1, &g_IndexBufferID);
		g_IndexBufferID = 0;
	}

	return true;
}

void ResizeWindowOpenGL(int width, int height)
{
	glViewport(0, 0, width, height);
	// `投影矩阵`
	float aspect = (float) height / (float) width;
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(60.0f, aspect, 0.1f, 100.0f);
	// `设置视角转换矩阵`
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
}

// `使用OpenGL来绘图`
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// `使用g\_VertexBufferID所代表的Vertex Buffer Object`
	glBindBuffer(GL_ARRAY_BUFFER, g_VertexBufferID);
	// `使用g\_IndexBufferID所代表的Index Buffer Object`
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IndexBufferID);

	// `设置要用数组的方式传入顶点位置和颜色`
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	// `VertexPointer/ColorPointer不再传入指针位置`
	// 而是传入内存相对的位移值
	int voffset = (int) &g_pSphereVertices[0].m_Position - (int) &g_pSphereVertices[0];
	int coffset = (int) &g_pSphereVertices[0].m_RGBA - (int) &g_pSphereVertices[0];
	// `最后一个值是在内存中的起始位移`
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VC), (GLvoid *)voffset); // voffset=0
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex_VC), (GLvoid *)coffset); // coffset=12

	// `计算出一个可以转换到镜头坐标系的矩阵`
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &view_matrix);

	glDrawElements(
		GL_TRIANGLES, // `指定所要画的基本图形种类`
		g_iNumSphereIndices, // `有几个索引值`
		GL_UNSIGNED_SHORT, // `索引值的类型`
		0 // `不再传入数组的指针, 而是传用索引值起始的位移.`
	);

	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
