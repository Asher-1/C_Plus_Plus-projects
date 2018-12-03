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
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	// 设置视角转换矩阵
	g_projection_matrix = GutMatrixOrthoRH_DirectX(g_fOrthoWidth, g_fOrthoHeight, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);

	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 画出正向和反向的三角形
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 载入贴图
	g_pTexture0 = GutLoadTexture_DX9("../../textures/brickwall_broken.tga");
	g_pTexture1 = GutLoadTexture_DX9("../../textures/spotlight_effect.tga");

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
	GutResetGraphicsDeviceDX9();
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	g_fOrthoWidth = g_fOrthoSize;
	g_fOrthoHeight = g_fOrthoSize;
	if ( aspect > 1.0f )
		g_fOrthoHeight *= aspect;
	else
		g_fOrthoWidth /= aspect;

	g_projection_matrix = GutMatrixOrthoRH_DirectX(g_fOrthoWidth, g_fOrthoHeight, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
	// 计算出一个可以转换到镜头坐标系的矩阵
	//device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
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

// `使用Direct3D9来绘图`
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 0);
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	// `开始下绘图指令`
	device->BeginScene(); 
	// `设置数据格式`
	device->SetFVF(D3DFVF_XYZ|D3DFVF_TEX1); 

	{
		Matrix4x4 IdentityMatrix; 
		IdentityMatrix.Identity();
		
		device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &IdentityMatrix);
		device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &IdentityMatrix);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &IdentityMatrix);
		device->SetTexture(0, g_pTexture0);

		if ( g_bStencil )
		{
			// `启动` Stencil Buffer Test
			device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			// `把 Stencil 参考值设置 1`
			device->SetRenderState(D3DRS_STENCILREF, 1);
			// `Stencil Test 判断条件设置成永远成立`
			device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
			// `直接把 Stencil 参考值填入 Stencil Buffer 里`
			device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		}
		else
		{
			// 关闭Stencil Buffer Test
			device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		}

		// `设置 Alpha Test 条件`
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		device->SetRenderState(D3DRS_ALPHAREF, 128);

		// `画出墙壁, 会同时更新Framebuffer上的颜色和Stencil Buffer.`
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_VT));
	}

	{
		if ( g_bStencil )
		{
			// `下面 3 行程序码会设置成只更新画面上 Stencil 值为 1 的像素`
			// `把 Stencil 参考值设置 1`
			device->SetRenderState(D3DRS_STENCILREF, 1);
			// `Stencil Test 判断条件设置成等于 Stencil 参考值才成立`
			device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
			// `Stencil Test 判断成立时，保留 Stencil Buffer 值，不去更新它。`
			device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		}

		device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
		// `镜头坐标系转换矩阵`
		Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
		device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &g_world_matrix);
		device->SetTexture(0, g_pTexture1);
		// `打开混合功能`
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		// `混合公式` = source_color * 1 + dest_color * 1
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		// `画出光柱`
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_VT));
		// `关闭Alpha Test功能`
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	// `把背景backbuffer的画面显示出来`
	device->Present( NULL, NULL, NULL, NULL );
}
