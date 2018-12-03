#include <stdio.h>
#include <conio.h>

#include "Gut.h"
#include "GutInput.h"

#include "render_dx9.h"
#include "render_dx10.h"
#include "render_opengl.h"
#include "render_data.h"

void GetUserInput(void)
{
	GutMouseInfo mouse;
	GutReadMouse(&mouse);
	
	if ( mouse.button[0] )
	{
		// 如果左击, 就旋转对象
		float rz = mouse.x * 0.01f;
		float rx = mouse.y * -0.01f;

		Matrix4x4 rotation_matrix;
		rotation_matrix.RotateZ_Replace(rz);
		rotation_matrix.RotateX(rx);
	
		g_object_matrix = g_object_matrix * rotation_matrix;
	}

	if ( mouse.z )
	{
		// 如果滚动滚轮, 就使用滚轮来缩放对象
		float scale = 1.0f + mouse.z * 0.001f;
		g_object_matrix.Scale(scale, scale, scale);
	}
}

void main(int argc, char *argv[])
{
	// 默认使用DirectX 9来绘图
	char *device = "dx9";
	void (*render)(void) = RenderFrameDX9;
	bool (*init_resource)(void) = InitResourceDX9;
	bool (*release_resource)(void) = ReleaseResourceDX9;

#ifdef _ENABLE_DX10_
	printf("Press\n(1) for Direct3D9\n(2) for OpenGL\n(3) for Direct3D10\n");
#else
	printf("Press\n(1) for Direct3D9\n(2) for OpenGL\n");
#endif

	int c = getche();

	switch(c)
	{
	default:
	case '1':
		render = RenderFrameDX9;
		init_resource = InitResourceDX9;
		release_resource = ReleaseResourceDX9;
		break;
	case '2':
		device = "opengl";
		init_resource = InitResourceOpenGL;
		release_resource = ReleaseResourceOpenGL;
		render = RenderFrameOpenGL;
		break;
	case '3':
	#ifdef _ENABLE_DX10_
		device = "dx10";
		init_resource = InitResourceDX10;
		release_resource = ReleaseResourceDX10;
		render = RenderFrameDX10;
	#endif
		break;
	}

	// 在(100,100)的位置打开一个大小为(512x512)的窗口
	GutCreateWindow(100, 100, 512, 512, device);

	// 做OpenGL或DirectX初始化
	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}


	// 载入shader
	if ( !init_resource() )
	{
		release_resource();
		printf("Failed to load resources\n");
		exit(0);
	}

	// 初始化DirectX Input对象
	GutInputInit();
	// 先把矩阵设为单位矩阵
	g_object_matrix.Identity();

	// 主循环
	while( GutProcessMessage() )
	{
		// 读取鼠标, 并计算新的旋转及缩放矩阵
		GetUserInput();
		// 画图
		render();
	}
	
	// 释放shader
	release_resource();

	// 关闭OpenGL/DirectX绘图设备
	GutReleaseGraphicsDevice();
}
