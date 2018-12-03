#include <d3dx9.h>

#include "Gut.h"
#include "GutWin32.h"
#include "GutTexture_DX9.h"

#include "render_data.h"

static LPDIRECT3DTEXTURE9 g_pTexture0 = NULL;
static LPDIRECT3DTEXTURE9 g_pTexture1 = NULL;

static Matrix4x4 g_projection_matrix;

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	int w, h;
	GutGetWindowSize(w, h);
	float aspect = (float) h / (float) w;
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, aspect, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 画出正向和反向的三角形
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// 载入贴图
	g_pTexture0 = GutLoadTexture_DX9("../../textures/brickwall.tga");
	g_pTexture1 = GutLoadTexture_DX9("../../textures/lena.tga");

	// trilinear
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// trilinear
	device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return true;
}

bool ReleaseResourceDX9(void)
{
	SAFE_RELEASE(g_pTexture0);
	SAFE_RELEASE(g_pTexture1);

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
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, aspect, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 画出正向和反向的三角形
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// trilinear
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// trilinear
	device->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

// 使用DirectX 9来绘图
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 开始下绘图指令
	device->BeginScene(); 
	// 设置数据格式
	device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1); 

	{
		Matrix4x4 IdentityMatrix; 
		IdentityMatrix.Identity();
		
		device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &IdentityMatrix);
		device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &IdentityMatrix);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &IdentityMatrix);
		device->SetTexture(0, g_pTexture0);
		// ZBuffer测试条件设为永远成立
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		// 画出矩形, 同时会清除ZBuffer
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_FullScreenQuad, sizeof(Vertex_VCT));
		// ZBuffer测试条件设为小于
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	}

	{
		device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
		// 设置转换矩阵
		Matrix4x4 view_matrix = g_Control.GetViewMatrix();
		Matrix4x4 world_matrix = g_Control.GetObjectMatrix();
		device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_matrix);
		//
		device->SetTexture(0, g_pTexture1);
		// 打开混合功能
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		// source_blend_factor = source_color.a
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		// dest_blend_factor = (1 - source_color.a)
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		// 混合公式 = source_color.rgba * (source_color.a) + dest_color.rgba * (1 - source_color.a)
		// direct3D默认会直接输出贴图alpha值, 把它改成和顶点alpha值相乘.
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		// 画出矩形
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_VCT));
		// 关闭Alpha Test功能
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	// 声明所有的绘图指令都下完了
	device->EndScene(); 
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}
