#include <stdio.h>
#include <conio.h>

#include "Gut.h"
#include "GutInput.h"
#include "GutTimer.h"

#include "render_dx9.h"
#include "render_dx10.h"
#include "render_opengl.h"
#include "render_data.h"

GutTimer g_Timer;
float g_fFrame_Time = 0.0f;
float g_fGame_Time = 0.0f;

void GetUserInput(void)
{
	g_fFrame_Time = g_Timer.Stop();
	g_fGame_Time += g_fFrame_Time;
	g_Timer.Restart();
	GutReadKeyboard();
	g_Control.Update(g_fFrame_Time, CGutUserControl::CONTROLLER_ROTATEOBJECT);
	//g_Control.Update(g_fFrame_Time, CGutUserControl::CONTROLLER_FPSCAMERA);
}

static float g_light_pos_x = 0.0f;

void FrameMove(void)
{
	g_Light.m_Position[0] = g_light_pos_x + FastMath::Sin(g_fGame_Time * 4.0f);
}

void main(void)
{
	// 默认使用DirectX 9来绘图
	char *device = "dx9";
	void (*render)(void) = RenderFrameDX9;
	bool (*init_resource)(void) = InitResourceDX9;
	bool (*release_resource)(void) = ReleaseResourceDX9;
	void (*resize_func)(int width, int height) = ResizeWindowDX9;

	printf("Press\n(1) for Direct3D9\n(2) for OpenGL\n");
	int c = getche();

	switch(c)
	{
	default:
	case '1':
		render = RenderFrameDX9;
		init_resource = InitResourceDX9;
		release_resource = ReleaseResourceDX9;
		resize_func = ResizeWindowDX9;
		break;
	case '2':
		device = "opengl";
		init_resource = InitResourceOpenGL;
		release_resource = ReleaseResourceOpenGL;
		render = RenderFrameOpenGL;
		resize_func = ResizeWindowOpenGL;
		break;
	}

	GutResizeFunc( resize_func );

	// 在(100,100)的位置打开一个大小为(512x512)的窗口
	GutCreateWindow(100, 100, 512, 512, device);

	// 做OpenGL或DirectX初始化
	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}

	// 设置方向光
	g_Light.m_Position.Set(-3.0f, 0.0f, 5.0f);
	g_Light.m_Direction.Set(0.0f, 1.0f, 1.0f, 0.0f);
	g_Light.m_Direction.Normalize();
	g_Light.m_Diffuse.Set(0.6f, 0.6f, 0.6f, 1.0f);
	//g_Light.m_Specular.Set(1.0f, 1.0f, 1.0f, 1.0f);
	g_Light.m_Specular.Set(0.0f, 0.0f, 0.0f, 1.0f);

	g_light_pos_x = g_Light.m_Position[0];

	GutInputInit();

	g_Control.SetCamera(Vector4(0.0f, 0.0f, 10.0f), Vector4(0.0f), Vector4(0.0f, 1.0f, 0.0f) );

	g_Model.Load_ASCII("..\\..\\models\\teapot.gma");

	// 载入shader
	if ( !init_resource() )
	{
		release_resource();
		printf("Failed to load resources\n");
		exit(0);
	}

	// 主循环
	while( GutProcessMessage() )
	{
		GetUserInput();
		FrameMove();
		render();
	}
	
	// 释放shader
	release_resource();

	// 关闭OpenGL/DirectX绘图设备
	GutReleaseGraphicsDevice();
}
