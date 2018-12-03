#include "Gut.h"
#include "render_data.h"
#include "GutModel_DX9.h"
#include "GutWin32.h"

static int g_iFrameBufferWidth = 1024;
static int g_iFrameBufferHeight = 1024;

LPDIRECT3DTEXTURE9 g_pTexture = NULL;
LPDIRECT3DSURFACE9 g_pFrameBuffer = NULL;
LPDIRECT3DSURFACE9 g_pDepthStencil = NULL;

LPDIRECT3DSURFACE9 g_pMainFrameBuffer = NULL;
LPDIRECT3DSURFACE9 g_pMainDepthBuffer = NULL;

void InitStateDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 使用trilinear
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	device->GetRenderTarget(0, &g_pMainFrameBuffer);
	device->GetDepthStencilSurface(&g_pMainDepthBuffer);

	int width, height;
	GutGetWindowSize(width, height);
	
	g_iFrameBufferWidth = width * 2;
	g_iFrameBufferHeight = height * 2;

	device->CreateTexture(g_iFrameBufferWidth, g_iFrameBufferHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_pTexture, NULL);
	device->CreateDepthStencilSurface(g_iFrameBufferWidth, g_iFrameBufferHeight, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &g_pDepthStencil, NULL);
	g_pTexture->GetSurfaceLevel(0, &g_pFrameBuffer);
}

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	InitStateDX9();

	return true;
}

bool ReleaseResourceDX9(void)
{
	SAFE_RELEASE(g_pFrameBuffer);
	SAFE_RELEASE(g_pMainFrameBuffer);
	SAFE_RELEASE(g_pMainDepthBuffer);

	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pTexture);

	return true;
}

void ResizeWindowDX9(int width, int height)
{
	SAFE_RELEASE(g_pFrameBuffer);
	SAFE_RELEASE(g_pMainFrameBuffer);
	SAFE_RELEASE(g_pMainDepthBuffer);

	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pTexture);

	// Reset Device
	GutResetGraphicsDeviceDX9();
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	InitStateDX9();
}

static void Antialiasing_None(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->BeginScene(); 

	// `设置转换矩阵`
	Matrix4x4 ident_matrix; ident_matrix.Identity();

	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &ident_matrix);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &ident_matrix);
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &g_world_matrix);

	device->SetRenderState(D3DRS_ZENABLE, FALSE);

	// `清除画面`
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_RGBA(0,0,255,255), 1.0f, 0);
	// `画出3角形`
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);
	device->SetFVF(D3DFVF_XYZ);
	device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, g_Triangle, sizeof(Vector4));
	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}

static void Antialiasing_MultiSampling(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `打开multisampling功能`
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	// `画图的方法没有什么不同`
	Antialiasing_None();
	// `关闭multisampling功能`
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
}

static void Antialiasing_SuperSampling(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->BeginScene(); 

	Matrix4x4 ident_matrix; ident_matrix.Identity();

	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &ident_matrix);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &ident_matrix);

	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	device->SetRenderState(D3DRS_ZENABLE, FALSE);

	// `先把画面画在4倍大的Framebuffer上`
	{
		// `设置绘图结果要画在动态贴图上`
		device->SetRenderTarget(0, g_pFrameBuffer);
		device->SetDepthStencilSurface(g_pDepthStencil);
		// `清除画面`
		device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_RGBA(0,0,255,255), 1.0f, 0);
		// `设置转换矩阵`
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &g_world_matrix);
		// `画一片3角形`
		device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);
		device->SetFVF(D3DFVF_XYZ);
		device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, g_Triangle, sizeof(Vector4));
	}

	// `再把画面缩小放到原始大小的画面上`
	{
		// `改用原来的Framebuffer`
		device->SetRenderTarget(0, g_pMainFrameBuffer);
		device->SetDepthStencilSurface(g_pMainDepthBuffer);
		// `设置要贴图的内插`
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		// `使用动态贴图`
		device->SetTexture(0, g_pTexture);
		device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		// `设置转换矩阵`
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &ident_matrix);
		// `把贴图套到整个画面上`
		device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_VT));
	}

	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}

// 使用DirectX 9来绘图
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	if ( g_bWireframe )
	{
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	switch(g_iMode)
	{
	default:
	case 1:
		Antialiasing_None();
		break;
	case 2:
		Antialiasing_MultiSampling();
		break;
	case 3:
		Antialiasing_SuperSampling();
		break;
	}
}