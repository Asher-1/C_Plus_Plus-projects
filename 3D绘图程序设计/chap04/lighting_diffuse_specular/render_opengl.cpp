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
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_OpenGL(g_fFovW, 1.0f, 0.1f, 100.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &projection_matrix);
	// 把正向和反向的面都画出来
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
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
	// `打开光照功能`
	glEnable(GL_LIGHTING);
	// `打开/关闭auto normalize功能`
	if ( g_bAutoNormalize )
		glEnable(GL_NORMALIZE);
	else
		glDisable(GL_NORMALIZE);
	// `打开/关闭localviewer功能`
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, g_bLocalViewer ? 1 : 0 );
	// `设置环境光`
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float *)&g_vGlobal_AmbientLight);
	// `设置对象纹理的反光能力`
	glMaterialfv(GL_FRONT, GL_AMBIENT, (float *) &g_vMaterialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, (float *) &g_vMaterialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, (float *) &g_vMaterialSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, (float *) &g_vMaterialEmissive);
	glMaterialf (GL_FRONT, GL_SHININESS, g_fMaterialShininess);
	// `设置光源`
	for ( int i=0; i<g_iNumLights; i++ )
	{
		// `每个光源分别用GL_LIGHT0,GL_LIGHT1,GL_LIGHT2来表示`
		// `它们在定义时是依顺序排列,可以用GL_LIGHT0+n来获得光源ID`
		int LightID = GL_LIGHT0 + i;

		if ( g_Lights[i].m_bEnabled )
		{
			// 打开这盏光
			glEnable(LightID);
			
			switch(g_Lights[i].m_eType)
			{
			case LIGHT_DIRECTIONAL:
				// `光源方向`
				// `把一个(x,y,z,w)的向量传入GL_POSITION, 如果w值是0会被视为是方向光.`
				glLightfv(LightID, GL_POSITION, (float *)&(-g_Lights[i].m_vDirection) );
				break;
			case LIGHT_POINT:
				// `光源位置`
				glLightfv(LightID, GL_POSITION, (float *)&g_Lights[i].m_vPosition);
				// `随距离的衰减值` 
				// 1/(CONSTANT + LINEAR*d + QUADRATIC*d^2) 
				// `公式中的CONSTANT,LINEAR,QUADRATIC值`
				glLightf(LightID, GL_CONSTANT_ATTENUATION,	g_Lights[i].m_vAttenuation[0]);
				glLightf(LightID, GL_LINEAR_ATTENUATION,	g_Lights[i].m_vAttenuation[1]);
				glLightf(LightID, GL_QUADRATIC_ATTENUATION, g_Lights[i].m_vAttenuation[2]);
				break;
			case LIGHT_SPOT:
				// 光源位置
				glLightfv(LightID, GL_POSITION, (float *)&g_Lights[i].m_vPosition);
				// `随距离的衰减值`
				// 1/(CONSTANT + LINEAR*d + QUADRATIC*d^2) 
				// `公式中的CONSTANT, LINEAR, QUADRATIC值`
				glLightf(LightID, GL_CONSTANT_ATTENUATION,	g_Lights[i].m_vAttenuation[0]);
				glLightf(LightID, GL_LINEAR_ATTENUATION,	g_Lights[i].m_vAttenuation[1]);
				glLightf(LightID, GL_QUADRATIC_ATTENUATION, g_Lights[i].m_vAttenuation[2]);
				// `光柱的方向`
				glLightfv(LightID, GL_SPOT_DIRECTION, (float *)&g_Lights[i].m_vDirection);
				// `光柱圆锥的一半角度`
				glLightf(LightID, GL_SPOT_CUTOFF, g_Lights[i].m_fSpotlightCutoff * 0.5f);
				// `光柱圆锥的角度衰减`
				glLightf(LightID, GL_SPOT_EXPONENT, g_Lights[i].m_fSpotlightExponent);
				break;
			}
			// `光源的Ambient值`
			glLightfv(LightID, GL_AMBIENT, (float *)&g_Lights[i].m_vAmbientColor); 
			// `光源的Diffuse值`
			glLightfv(LightID, GL_DIFFUSE, (float *)&g_Lights[i].m_vDiffuseColor);
			// `光源的specular值`
			glLightfv(LightID, GL_SPECULAR, (float *)&g_Lights[i].m_vSpecularColor);
		}
		else
		{
			// `关闭这盏光`
			glDisable(LightID);
		}
	}
}

// 使用OpenGL来绘图
void RenderFrameOpenGL(void)
{
	// 清除画面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	// 设置要用数组的方式传入顶点位置和颜色
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex_V3N3), g_pGridVertices[0].m_Position);
	glNormalPointer(GL_FLOAT, sizeof(Vertex_V3N3), g_pGridVertices[0].m_Normal);
	
	// 计算出一个可以转换到镜头坐标系的矩阵
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// 使用镜头转换矩阵
	glLoadMatrixf( (float *) &view_matrix);
	// 设置光源, 所有光源会转换到镜头坐标系,所以要先使用镜头转换矩阵
	SetupLightingOpenGL();
	
	Matrix4x4 world_view_matrix = g_world_matrix * view_matrix;
	glLoadMatrixf( (float *) &world_view_matrix);
	// 画出格子
	glDrawElements (GL_TRIANGLE_STRIP, g_iNumGridIndices, GL_UNSIGNED_SHORT, g_pGridIndices);
	// 把背景backbuffer的画面显示出来
	GutSwapBuffersOpenGL();
}
