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

void GetUserInput(void)
{
	// 读取鼠标
	GutMouseInfo mouse;
	GutReadMouse(&mouse);
	// 获得画完前一个画面到现在所经历的时间
	g_fFrame_Time = g_Timer.Stop();
	g_Timer.Restart();

	float moving_speed = 2.0f * g_fFrame_Time;
	float rotation_speed = 1.0 * g_fFrame_Time;

	// 如果左击，就旋转镜头
	if ( mouse.button[0] ) 
	{
		g_fRotate_Y += mouse.x * rotation_speed;
		g_fRotate_X += mouse.y * rotation_speed;
	}
}

void frame_move(void)
{
	g_simulation_days += g_fFrame_Time * simulation_speed;
	
	// 把太阳放在世界坐标系原点
	g_sun_matrix.Identity();
	// 算出地球的位置
	g_earth_matrix = g_sun_matrix; // 把地球放到太阳的坐标系上
	g_earth_matrix.RotateY( 2.0f * PI * g_simulation_days / days_a_year); 
	g_earth_matrix.TranslateX( earth_to_sun_distance );
	// 算出月球的位置
	g_moon_matrix = g_earth_matrix; // 把月球放到地球的坐标系上
	g_moon_matrix.RotateY( 2.0f * PI * g_simulation_days / days_a_month );
	g_moon_matrix.TranslateX( moon_to_earth_distance );
}

void main(void)
{
	// 默认使用DirectX 9来绘图
	char *device = "dx9";
	void (*render)(void) = RenderFrameDX9;
	bool (*init_resource)(void) = InitResourceDX9;
	bool (*release_resource)(void) = ReleaseResourceDX9;
	void (*resize_func)(int width, int height) = ResizeWindowDX9;

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
		resize_func = ResizeWindowDX9;
		break;
	case '2':
		device = "opengl";
		init_resource = InitResourceOpenGL;
		release_resource = ReleaseResourceOpenGL;
		render = RenderFrameOpenGL;
		resize_func = ResizeWindowOpenGL;
		break;
	case '3':
		device = "dx10";
		init_resource = InitResourceDX10;
		release_resource = ReleaseResourceDX10;
		render = RenderFrameDX10;
		resize_func = ResizeWindowDX10;
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

	g_view_matrix.Identity();
	GutInputInit();

	// Sun
	float yellow[]={1.0f, 1.0f, 0.0f, 1.0f};
	CreateSphere(2.0f, &g_pSunVertices, &g_pSphereIndices, yellow);
	// Earch
	float blue[]={0.0f, 0.0f, 1.0f, 1.0f};
	CreateSphere(1.0f, &g_pEarthVertices, NULL, blue);
	// Moon
	float white[]={1.0f, 1.0f, 1.0f, 1.0f};
	CreateSphere(0.2f, &g_pMoonVertices, NULL);

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
		frame_move();
		render();
	}
	
	// 释放shader
	release_resource();

	// 关闭OpenGL/DirectX绘图设备
	GutReleaseGraphicsDevice();
}
