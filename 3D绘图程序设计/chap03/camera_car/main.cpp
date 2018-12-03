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

void GetUserInput(void)
{
	// `读取鼠标`
	GutMouseInfo mouse;
	GutReadMouse(&mouse);
	// `读取键盘`
	char keyboard_state[256];
	GutReadKeyboard(keyboard_state);
	// `计算目前镜头法线`
	Vector4 facing = g_lookat - g_eye;
	// `确保它是个单位向量`
	facing.Normalize(); 
	// `获得画完前一个画面到现在所经历的时间`
	float time_diff = g_Timer.Stop();
	g_Timer.Restart();

	float moving_speed = 2.0f * time_diff;
	float rotation_speed = 1.0 * time_diff;
	float ry = 0.0f;

	// `如果左击, 就旋转镜头.`
	if ( mouse.button[0] ) 
		ry = mouse.x * rotation_speed;
	// `按下A或者方向键<-, 就左旋.`
	if ( keyboard_state[GUTKEY_A] || keyboard_state[GUTKEY_LEFT] )
		ry = -rotation_speed;
	// `按下D或者方向键->, 就右旋.`
	if ( keyboard_state[GUTKEY_D] || keyboard_state[GUTKEY_RIGHT] )
		ry = rotation_speed;
	
	if ( ry )
	{
		// `先获得一个旋转矩阵`
		Matrix4x4 rotate_matrix;
		rotate_matrix.RotateY_Replace(-ry);
		// `把原来的法线沿Y轴旋转ry度, 获得新的法线.`
		facing = facing * rotate_matrix;
	}

	// `按下W或方向键向上`
	if ( keyboard_state[GUTKEY_W] || keyboard_state[GUTKEY_UP] )
	{
		// `镜头向前移动`
		g_eye += facing * moving_speed; 
	}

	// `按下S或方向键向下`
	if ( keyboard_state[GUTKEY_S] || keyboard_state[GUTKEY_DOWN] )
	{
		// `镜头向后移动`
		g_eye -= facing * moving_speed; 
	}

	// `计算出镜头对准的点, 生成镜头转换矩阵时会用到.`
	g_lookat = g_eye + facing; 
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

	GutInputInit();
	g_object_matrix.Identity();

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
