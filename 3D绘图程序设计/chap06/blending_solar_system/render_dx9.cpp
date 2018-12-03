#include "Gut.h"
#include "render_data.h"
#include <d3dx9.h>
#include <d3dx9math.h>

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(90.0f, 1.0f, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 改变三角形正面的法线
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	return true;
}

bool ReleaseResourceDX9(void)
{
	return true;
}

void ResizeWindowDX9(int width, int height)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	D3DPRESENT_PARAMETERS d3dpresent;
    
	ZeroMemory( &d3dpresent, sizeof(d3dpresent) );
    d3dpresent.Windowed = TRUE; // 使用窗口模式
    d3dpresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpresent.BackBufferFormat = D3DFMT_UNKNOWN; // 使用窗口模式可以不去设置
	d3dpresent.BackBufferCount = 1; // 提供一块backbuffer
	d3dpresent.EnableAutoDepthStencil = TRUE; // 自动打开DepthStencil Buffer
	d3dpresent.AutoDepthStencilFormat = D3DFMT_D24S8; // DepthStencil Buffer模式
	
	device->Reset(&d3dpresent);

	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(90.0f, aspect, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 改变三角形正面的法线
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

// 使用DirectX 9来绘图
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 清除画面
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 50, 50, 50), 1.0f, 0);
	
	// 开始下绘图指令
	device->BeginScene(); 

	// 计算出一个可以转换到镜头坐标系的矩阵
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);

	// 设置数据格式
	// D3DFVF_XYZ = 使用3个浮点数来记录位置
	// D3DFVF_DIFFUSE = 使用32bits整数类型来记录BGRA颜色
	device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE); 

	device->SetRenderState(D3DRS_ZWRITEENABLE, g_bZWrite ? TRUE : FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
	switch(g_iBlendMode)
	{
	case 0:
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case 1:
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	}

	Matrix4x4 world_view_matrix;

	for ( int i=0; i<2; i++ )
	{
		switch( g_iOrder[i] )
		{
		case 0:
			// 太阳
			world_view_matrix = g_sun_matrix * g_rot_matrix;
			device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_view_matrix);
			device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, g_iNumSphereVertices, g_iNumSphereTriangles, 
			g_pSphereIndices, D3DFMT_INDEX16, g_pSunVertices, sizeof(Vertex_VC) );
			break;
		case 1:
			// 地球
			world_view_matrix = g_earth_matrix * g_rot_matrix;
			device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_view_matrix);
			device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, g_iNumSphereVertices, g_iNumSphereTriangles, 
			g_pSphereIndices, D3DFMT_INDEX16, g_pEarthVertices, sizeof(Vertex_VC) );
			break;
		}
	}

	// 声明所有的绘图指令都下完了
	device->EndScene(); 
	
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}
