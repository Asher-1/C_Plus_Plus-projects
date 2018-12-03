#include "Gut.h"
#include "render_data.h"

bool InitResourceDX9(void)
{
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `计算出一个可以转换到镜头坐标系的矩阵`
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(60.0f, 1.0f, 1.0f, 100.0f);
	// `设置视角转换矩阵`
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// `设置镜头转换矩阵`
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
	// `关闭光照`
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return true;
}

bool ReleaseResourceDX9(void)
{
	return true;
}

// `使用Direct3D9来绘图`
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->Clear(
		0, NULL, // `清除整个画面 `
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // `清除颜色和Z Buffer`
		D3DCOLOR_ARGB(0, 0, 0, 0), // `设置要把颜色清成黑色`
		1.0f, // `设置要把Z值清为1, 也就是离镜头最远.`
		0 // `设置要把Stencil buffer清为0, 在这是否设置没有区别.`
	);
	
	// `开始下绘图指令`
	device->BeginScene(); 
	// `设置数据格式`
	device->SetFVF(D3DFVF_XYZ); 

	// `4个金字塔的基本位置`
	Vector4 pos[4] = 
	{
		Vector4(-1.0f, -1.0f, 0.0f),
		Vector4( 1.0f, -1.0f, 0.0f),
		Vector4(-1.0f,  1.0f, 0.0f),
		Vector4( 1.0f,  1.0f, 0.0f),
	};

	Matrix4x4 world_matrix;

	for ( int i=0; i<4; i++ )
	{
		// `设置转换矩阵`
		world_matrix.Translate_Replace(pos[i]);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_matrix);
		// `画出金字塔的8条边线`
		device->DrawPrimitiveUP(D3DPT_LINELIST, 8, g_vertices, sizeof(Vector4)); 
	}

	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
