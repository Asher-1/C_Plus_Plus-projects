#include <stdio.h>
#include "Gut.h"
#include "Vector4.h"

void RenderFrameDX9(void);
void RenderFrameOpenGL(void);

void main(int argc, char *argv[])
{
	// 默认使用DirectX 9来绘图
	char *device = "dx9";

	if ( argc > 1 )
	{
		// 如果命令行参数指定用OpenGL, 就改用OpenGL.
		if ( stricmp(argv[1], "opengl")==0 )
		{
			device = "opengl";
		}
	}

	// 在(100,100)的位置打开一个大小为(512x512)的窗口
	GutCreateWindow(100, 100, 512, 512, device);

	// 做OpenGL或DirectX初始化
	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}

	// 主循环
	while( GutProcessMessage() )
	{
		if ( !strcmp(device, "dx9") )
		{
			RenderFrameDX9();
		}
		else
		{
			RenderFrameOpenGL();
		}
	}

	// 关闭OpenGL/DirectX绘图设备
	GutReleaseGraphicsDevice();
}

// `金字塔形的8条边线`
Vector4 g_vertices[] = 
{
	Vector4(-1.0f, 1.0f,-1.0f),
	Vector4(-1.0f,-1.0f,-1.0f),

	Vector4(-1.0f,-1.0f,-1.0f),
	Vector4( 1.0f,-1.0f,-1.0f),

	Vector4( 1.0f,-1.0f,-1.0f),
	Vector4( 1.0f, 1.0f,-1.0f),

	Vector4( 1.0f, 1.0f,-1.0f),
	Vector4(-1.0f, 1.0f,-1.0f),

	Vector4( 0.0f, 0.0f, 1.0f),
	Vector4(-1.0f, 1.0f,-1.0f),

	Vector4( 0.0f, 0.0f, 1.0f),
	Vector4(-1.0f,-1.0f,-1.0f),

	Vector4( 0.0f, 0.0f, 1.0f),
	Vector4( 1.0f,-1.0f,-1.0f),

	Vector4( 0.0f, 0.0f, 1.0f),
	Vector4( 1.0f, 1.0f,-1.0f),
};

// `镜头位置`
Vector4 g_eye(0.0f,0.0f,5.0f); 
// `镜头对准的点`
Vector4 g_lookat(0.0f, 0.0f, 0.0f); 
// `镜头正上方的方向`
Vector4 g_up(0.0f, 1.0f,0.0f); 

// `使用DirectX9来绘图`
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
    device->SetRenderState( D3DRS_LIGHTING, FALSE );
    device->Clear(
		0, NULL, // `清除整个画面`
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // `清除颜色和Z Buffer`
		D3DCOLOR_ARGB(0, 0, 0, 0), // `设置要把颜色清成黑色`
		1.0f, // `设置要把Z值清为1, 也就是离镜头最远.`
		0 // `设置要把Stencil buffer清为0, 在这是否设置没有差别.`
	);
	
	// `计算出一个可以转换到镜头坐标系的矩阵`
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// `计算出一个使用正交投影的矩阵`
	Matrix4x4 orthogonal_matrix = GutMatrixOrthoRH_DirectX(5.0f, 5.0f, 0.1f, 100.0f);
	// `把这两个矩阵相乘`
	Matrix4x4 view_orthogonal_matrix = view_matrix * orthogonal_matrix;
	// `把空间中的坐标点转换到屏幕坐标系上`
	Vector4 vertices[16];
	for ( int i=0; i<16; i++ )
	{
		vertices[i] = g_vertices[i] * view_orthogonal_matrix;
		vertices[i] /= vertices[i].GetW();
	}

	device->BeginScene(); // `开始下绘图指令`
	device->SetFVF(D3DFVF_XYZ); // `设置数据格式`
	// `画出金字塔的8条边线`
	device->DrawPrimitiveUP(D3DPT_LINELIST, 8, vertices, sizeof(Vector4)); 
	device->EndScene(); // `声明所有的绘图指令都下完了`
	
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}

// `使用OpenGL来绘图`
void RenderFrameOpenGL(void)
{
	// `清除画面`
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// `计算出一个可以转换到镜头坐标系的矩阵`
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// `计算出一个使用正交投影的矩阵`
	Matrix4x4 orthogonal_matrix = GutMatrixOrthoRH_OpenGL(5.0f, 5.0f, 0.1f, 100.0f);
	// `把这两个矩阵相乘`
	Matrix4x4 view_orthogonal_matrix = view_matrix * orthogonal_matrix;
	// `把空间中的坐标点转换到屏幕坐标系上`
	Vector4 vertices[16];
	for ( int i=0; i<16; i++ )
	{
		vertices[i] = g_vertices[i] * view_orthogonal_matrix;
		vertices[i] /= vertices[i].GetW();
	}

	// `画出金字塔的8条边线`
	glBegin(GL_LINES);
	for ( int l=0; l<8; l++ )
	{
		glVertex3fv((float *) &vertices[l*2]);
		glVertex3fv((float *) &vertices[l*2+1]);
	}
	glEnd();

	// `把背景backbuffer的画面显示出来`
	GutSwapBuffersOpenGL();
}
