#include "Gut.h"
#include "render_data.h"

bool InitResourceDX9(void)
{
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `计算出镜头坐标系的转换矩阵`
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

	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	
	// `开始下绘图指令`
	device->BeginScene(); 
	// `设置数据格式`
	device->SetFVF(D3DFVF_XYZ); 

	// `旋转角度`
	static float angle = 0.0f;
	angle += 0.01f;

	// `设置旋转矩阵`
	Matrix4x4 world_matrix;
	world_matrix.RotateZ_Replace(angle);
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_matrix);

	// `画出金字塔的8条边线`
	device->DrawIndexedPrimitiveUP(
		D3DPT_LINELIST, // `指定所要画的基本图形种类 `
		0, // `会使用的最小顶点编号, 事实上没太大用处.`
		5, // `顶点数组里有几个顶点`
		8, // `要画出几个基本图形`
		g_indices,		// `索引数组`
		D3DFMT_INDEX16, // `索引数组的类型`
		g_vertices,		// `顶点数组`
		sizeof(Vector4)	// `顶点数组里每个顶点的大小`
	);

	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	
	// `把背景backbuffer显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
