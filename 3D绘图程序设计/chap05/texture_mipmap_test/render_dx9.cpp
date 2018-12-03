#include <d3dx9.h>

#include "Gut.h"
#include "GutWin32.h"

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
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 画出正向和反向的三角形
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// 生成贴图对象
	if ( D3D_OK!=device->CreateTexture(g_iWidth, g_iHeight, 
		0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTexture, NULL) )
		return false;

	int mipmap_level = 0;
	w = g_iWidth;
	h = g_iHeight;

	while(true)
	{
		D3DLOCKED_RECT locked_rect;
		g_pTexture->LockRect(mipmap_level, &locked_rect, NULL, 0);
		
		unsigned char *target = (unsigned char *)locked_rect.pBits;
		unsigned char *source = g_pImages[mipmap_level];
		
		for ( int y=0; y<h; y++ )
		{
			for ( int x=0; x<w; x++ )
			{
				target[0] = source[2];
				target[1] = source[1];
				target[2] = source[0];
				target[3] = source[3];

				source +=4;
				target+=4;
			}
		}
		
		g_pTexture->UnlockRect(mipmap_level);
		
		if ( w==1 && h==1 )
			break;

		if ( w>1 ) w/=2;
		if ( h>1 ) h/=2;
		mipmap_level++;
	}

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
	// 设置矩阵
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 object_matrix = g_Control.GetObjectMatrix();
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &object_matrix);
	// 设置数据格式
	device->SetFVF(D3DFVF_XYZ|D3DFVF_TEX1); 
	// 使用贴图
	device->SetTexture(0, g_pTexture);

	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	switch(g_iFilterMode)
	{
	case 1: // linear filtering
		// 贴图被放大时, 取最接近4个点来内插.
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		// 贴图被缩小时, 取最接近4个点来平均
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		// 不使用Mipmap
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		break;
	case 2: // bilinear filtering
		// 贴图被放大时, 取最接近4个点来内插.
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		// 贴图被缩小时, 取最接近4个点来平均
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		// 取最接近大小的图层
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		break;
	case 3: // trilinear filtering
		// 贴图被放大时, 取最接近4个点来内插.
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		// 贴图被缩小时, 取最接近4个点来平均
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		// 取最接近大小的两个图层, 然后再内插.
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		break;
	case 4: // anisotropic filtering
		// 会根据缩放的长宽比例来取用贴图, 不固定是取哪几个图.
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);
		// 取最接近大小的两个图层, 然后再内插.
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		break;
	}
	// 画出矩形
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_VT));
	// 声明所有的绘图指令都下完了
	device->EndScene(); 
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}
