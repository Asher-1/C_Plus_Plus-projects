#include <windows.h>
// Standard OpenGL header
#include <GL/gl.h>

#include "Gut.h"
#include "render_data.h"

static Matrix4x4 g_view_matrix;

bool InitResourceOpenGL(void)
{
	// 使用zbuffer test
	glEnable(GL_DEPTH_TEST);
	// 计算出一个可以转换到镜头坐标系的矩阵
	g_view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// 设置投影矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, 1.0f, 0.1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);

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
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, aspect, 0.1f, 100.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
}

void SetupLightingOpenGL(void)
{
	// `设置环境光`
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float *)&g_vGlobal_AmbientLight);
	// `使用two side lighting功能`
	glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	// `设置对象纹理正面的反光能力`
	glMaterialfv(GL_FRONT, GL_AMBIENT, &g_vMaterialAmbient[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &g_vMaterialDiffuse[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, &g_vMaterialSpecular[0]);
	glMaterialfv(GL_FRONT, GL_EMISSION, &g_vMaterialEmissive[0]);
	glMaterialf (GL_FRONT, GL_SHININESS, g_fMaterialShininess);
	// `设置对象纹理背面的反光能力`
	glMaterialfv(GL_BACK, GL_AMBIENT, &g_vMaterialAmbient_Back[0]);
	glMaterialfv(GL_BACK, GL_DIFFUSE, &g_vMaterialDiffuse_Back[0]);
	glMaterialfv(GL_BACK, GL_SPECULAR, &g_vMaterialSpecular_Back[0]);
	glMaterialfv(GL_BACK, GL_EMISSION, &g_vMaterialEmissive_Back[0]);
	glMaterialf (GL_BACK, GL_SHININESS, g_fMaterialShininess_Back);
	// `打开光照功能`
	glEnable(GL_LIGHTING);
	// `打开第0号光`
	glEnable(GL_LIGHT0);
	// `把一个(x,y,z,w)的向量传入GL_POSITION, 如果w值是0会被视为是方向`
	glLightfv(GL_LIGHT0, GL_POSITION, (float *)&(-g_vLightDirection) );
	// `光源的Ambient值`
	glLightfv(GL_LIGHT0, GL_AMBIENT, &g_vLightAmbient[0]); 
	// `光源的Diffuse值`
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &g_vLightDiffuse[0]);
	// `光源的Specular值`
	glLightfv(GL_LIGHT0, GL_SPECULAR, &g_vLightSpecular[0]);
}

// `使用OpenGL来绘图`
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// `把正向和反向的面都画出来`
	glDisable(GL_CULL_FACE);
	// `设置光源`
	glLoadMatrixf( (float *) &g_view_matrix);
	SetupLightingOpenGL();
	// `设置要用数组的方式传入顶点位置和法线`
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_VN), &g_Quad[0].m_Position);
	glNormalPointer(GL_FLOAT, sizeof(Vertex_VN), &g_Quad[0].m_Normal);
	// `计算并代入转换矩阵`
	Matrix4x4 world_view_matrix = g_world_matrix * g_view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);
	// `画出一片矩形`
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
