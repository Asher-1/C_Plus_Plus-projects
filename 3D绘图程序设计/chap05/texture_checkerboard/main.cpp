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
	// 读取键盘
	char keyboard_state[256];
	GutReadKeyboard(keyboard_state);
	// 获得画完前一个画面到现在所经历的时间
	float time_diff = g_Timer.Stop();
	g_Timer.Restart();

	float moving_speed = 2.0f * time_diff;
	float rotation_speed = 1.0 * time_diff;

	// 要累积下对X和Y轴的旋转角度
	static float ry = 0.0f;
	static float rx = 0.0f;

	// 如果左击，就旋转镜头
	if ( mouse.button[0] ) 
	{
		ry -= mouse.x * rotation_speed;
		rx -= mouse.y * rotation_speed;
	}

	Matrix4x4 rotate_matrix;
	rotate_matrix.RotateY_Replace(ry);
	rotate_matrix.RotateX(rx);

	// 在右手坐标系里, 镜面的法线是转换矩阵的-Z轴
	Vector4 camera_facing = -rotate_matrix[2];
	// 获得镜面右边的方向
	Vector4 camera_right = rotate_matrix[0];

	// 按下W或方向键向上
	if ( keyboard_state[GUTKEY_W] || keyboard_state[GUTKEY_UP] )
	{
		g_eye += camera_facing * moving_speed;
	}
	// 按下S或方向键向下
	if ( keyboard_state[GUTKEY_S] || keyboard_state[GUTKEY_DOWN] )
	{
		g_eye -= camera_facing * moving_speed;
	}
	// 按下A或方向键向左
	if ( keyboard_state[GUTKEY_A] || keyboard_state[GUTKEY_LEFT] )
	{
		g_eye -= camera_right * moving_speed;
	}
	// 按下D或方向键向右
	if ( keyboard_state[GUTKEY_D] || keyboard_state[GUTKEY_RIGHT] )
	{
		g_eye += camera_right * moving_speed;
	}
	// 计算出镜头对准的点, 生成镜头转换矩阵时会用到.
	g_lookat = g_eye + camera_facing;
	// 因为是对2个轴转动, 需要更新镜头朝上的轴
	g_up = Vector3CrossProduct(camera_right, camera_facing);
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

	printf("\nSelected %s device for rendering.\n", device);

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
	g_world_matrix.Identity();

	GutInputInit();

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
		// 画出矩形
		render();
	}
	// 释放shader
	release_resource();
	// 关闭OpenGL/DirectX绘图设备
	GutReleaseGraphicsDevice();
}
