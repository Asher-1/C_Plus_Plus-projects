#include <stdio.h>
#include <conio.h>

#include "Gut.h"
#include "GutInput.h"
#include "GutTimer.h"

#include "render_dx10.h"
#include "render_data.h"

GutTimer g_Timer;

void GetUserInput(void)
{
	g_fTimeAdvance = g_Timer.Stop();
	g_fTimeElapsed += g_fTimeAdvance;

	g_Timer.Restart();
	GutReadKeyboard();

	g_Control.Update(g_fTimeAdvance, CGutUserControl::CONTROLLER_ROTATEOBJECT);
}

void main(void)
{
	// 默认使用DirectX 9来绘图
	char *device = "dx10";
	void (*render)(void) = RenderFrameDX10;
	bool (*init_resource)(void) = InitResourceDX10;
	bool (*release_resource)(void) = ReleaseResourceDX10;
	void (*resize_func)(int width, int height) = ResizeWindowDX10;

	GutResizeFunc(resize_func);
	// 在(100,100)的位置打开一个大小为(512x512)的窗口
	GutCreateWindow(100, 100, 512, 512, device);
	// D3D初始化
	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}

	GutInputInit();

	g_Control.SetCamera(Vector4(0.0f, 1.0f, 5.0f), Vector4(0.0f, 1.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f));

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
		render();
	}
	// 释放shader
	release_resource();
	// 关闭OpenGL/DirectX绘图设备
	GutReleaseGraphicsDevice();
}
