#include <stdio.h>
#include "Gut.h"

void RenderFrameDX9(void);
void RenderFrameOpenGL(void);

void main(int argc, char *argv[])
{
	// 默认使用DirectX 9来绘图
	char *device = "dx9";

	if ( argc > 1 )
	{
		// 如果命令列参数指定用OpenGL, 就改用OpenGL.
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

// 一个3D坐标点(x,y,z)的位置
float vertex[] = {0.0f, 0.0f, 0.0f};

// 使用DirectX 9的函数
void RenderFrameDX9(void)
{
	// 获得D3D设备.
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 关闭光照的计算
    device->SetRenderState( D3DRS_LIGHTING, FALSE );
	// 把画面清为黑色
    device->Clear( 
		0, NULL,　// 清除整个画面 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // 清除颜色和Z buffer 
		D3DCOLOR_ARGB(0, 0, 0, 0),  // 设置要把颜色清成黑色
		1.0f, // 设置要把Z值清为1, 也就是离镜头最远
		0 // 设置要把Stencil buffer清为0, 在这是否设置没有区别.
	);

	// 开始下绘图指令
	device->BeginScene(); 
	// 设置数据格式
	device->SetFVF(D3DFVF_XYZ); 
	// 画出一个点
	device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vertex, 12); 
	// 声明所有的绘图指令都下完了
	device->EndScene();		
	
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}

// 使用OpenGL的函数
void RenderFrameOpenGL(void)
{
	// 清除画面
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 画一个点
	glBegin(GL_POINTS);
	glVertex3fv(vertex);
	glEnd();

	// 把背景backbuffer的画面显示出来
	GutSwapBuffersOpenGL();
}
