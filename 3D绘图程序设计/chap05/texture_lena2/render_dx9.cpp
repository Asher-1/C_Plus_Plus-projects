#include <d3dx9.h>

#include "Gut.h"
#include "GutWin32.h"
#include "GutTGA.h"

#include "render_data.h"

static LPDIRECT3DTEXTURE9 g_pTexture = NULL;

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	int w, h;
	GutGetWindowSize(w, h);
	float aspect = (float) h / (float) w;
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, aspect, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// 镜头坐标系的转换矩阵
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 画出正向和反向的三角形
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// 载入贴图
	TGAImg tga_loader;
	if( IMG_OK!=tga_loader.Load("../../textures/lena_rgba.tga") )
		return false;
	// 生成贴图对象
	if ( D3D_OK!=device->CreateTexture(tga_loader.GetWidth(), tga_loader.GetHeight(), 
		1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTexture, NULL) )
		return false;
	int image_size = tga_loader.GetWidth() * tga_loader.GetHeight() * 4;
	// 拷贝贴图数据
	D3DLOCKED_RECT locked_rect;
	g_pTexture->LockRect(0, &locked_rect, NULL, 0);
	unsigned char *target = (unsigned char *)locked_rect.pBits;
	unsigned char *source = tga_loader.GetImg();
	for ( int y=0; y<tga_loader.GetHeight(); y++ )
	{
		for ( int x=0; x<tga_loader.GetWidth(); x++ )
		{
			// D3DFMT_A8R8G8B8的内存排列顺序是BGRA
			// 要对用RGBA排列的内存做转换
			target[0] = source[2];
			target[1] = source[1];
			target[2] = source[0];
			target[3] = source[3];

			target+=4;
			source+=4;
		}
	}
	g_pTexture->UnlockRect(0);

	return true;
}

bool ReleaseResourceDX9(void)
{
	SAFE_RELEASE(g_pTexture);

	return true;
}

void ResizeWindowDX9(int width, int height)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// Reset Device
	GutResetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	int w, h;
	GutGetWindowSize(w, h);
	float aspect = (float) h / (float) w;
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, aspect, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// 镜头坐标系的转换矩阵
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 画出正向和反向的三角形
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

// 使用DirectX 9来绘图
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 清除画面
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	// 开始下绘图指令
	device->BeginScene(); 
	// 
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &g_world_matrix);
	// 设置数据格式
	device->SetFVF(D3DFVF_XYZ|D3DFVF_TEX1); 
	// 使用贴图
	device->SetTexture(0, g_pTexture);
	// 画出格子
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_VT));
	// 声明所有的绘图指令都下完了
	device->EndScene(); 
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}
