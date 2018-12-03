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
float g_fElapsed_Time = 0.0f;

void GetUserInput(void)
{
	g_fFrame_Time = g_Timer.Stop();
	g_fElapsed_Time += g_fFrame_Time;
	g_Timer.Restart();
	GutReadKeyboard();
	g_Control.Update(g_fFrame_Time, CGutUserControl::CONTROLLER_ROTATEOBJECT);
}

void KeyDown_1(void)
{
	g_iMode = 1;
	printf("Scroll\n");
}

void KeyDown_2(void)
{
	g_iMode = 2;
	printf("Rotate\n");
}

void KeyDown_3(void)
{
	g_iMode = 3;
	printf("Scale\n");
}

void KeyDown_4(void)
{
	g_iMode = 4;
	printf("Restore\n");
}

void CalculateTextureMatrix(void)
{
	const float fScrollingSpeed = 1.0f;
	const float fRotateSpeed = FastMath::DegreeToRadian(360.0f);
	const float fScaleSpeed = 1.0f;

	Matrix4x4 offset_matrix;
	Matrix4x4 restore_matrix;
	Matrix4x4 texture_matrix;

	offset_matrix.Identity();
	offset_matrix[3].Set(-0.5f, -0.5f, 0.0f, 1.0f);
	restore_matrix.Identity();
	restore_matrix[3].Set(0.5f, 0.5f, 0.0f, 1.0f);

	switch(g_iMode)
	{
	case 1: // `创建贴图位移矩阵`
		g_texture_matrix.Identity();
		g_texture_matrix.Translate_Replace(fScrollingSpeed * g_fElapsed_Time, 0.0f, 0.0f);
		break;
	case 2: // `创建贴图旋转矩阵`
		texture_matrix.RotateZ_Replace(fRotateSpeed * g_fElapsed_Time);
		// 先转换到-0.5~0.5范围, 再旋转, 再转换回0~1范围
		g_texture_matrix = offset_matrix * texture_matrix * restore_matrix;
		break;
	case 3: // `创建贴图缩放矩阵`
		{
			const float max_scale = 3.0f;
			float scale = fmod(g_fElapsed_Time, fScaleSpeed) * max_scale;
			texture_matrix.Scale_Replace(scale, scale, scale);
			// 先转换到-0.5~0.5范围, 再缩放, 再转换回0~1范围
			g_texture_matrix = offset_matrix * texture_matrix * restore_matrix;
		}
		break;
	default:
		g_texture_matrix.Identity();
		break;
	}
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
#ifdef _ENABLE_DX10_
	case '3':
		device = "dx10";
		init_resource = InitResourceDX10;
		release_resource = ReleaseResourceDX10;
		render = RenderFrameDX10;
		resize_func = ResizeWindowDX10;
		break;
#endif // _ENABLE_DX10_
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

	GutInputInit();

	GutRegisterKeyDown(GUTKEY_1, KeyDown_1);
	GutRegisterKeyDown(GUTKEY_2, KeyDown_2);
	GutRegisterKeyDown(GUTKEY_3, KeyDown_3);
	GutRegisterKeyDown(GUTKEY_4, KeyDown_4);

	g_Control.SetCamera(Vector4(0.0f, 0.0f, 5.0f), Vector4(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f) );

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
		// 读取键盘鼠标
		GetUserInput();
		// 计算贴图坐标转换矩阵
		CalculateTextureMatrix();
		// 画出矩形
		render();
	}
	// 释放shader
	release_resource();
	// 关闭OpenGL/DirectX绘图设备
	GutReleaseGraphicsDevice();
}
