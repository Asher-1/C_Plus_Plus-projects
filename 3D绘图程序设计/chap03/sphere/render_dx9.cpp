#include "Gut.h"
#include "render_data.h"

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
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// 把RGBA改成BGRA
	for ( int i=0; i<g_iNumSphereVertices; i++ )
	{
		unsigned char temp = g_pSphereVertices[i].m_RGBA[0];
		g_pSphereVertices[i].m_RGBA[0] = g_pSphereVertices[i].m_RGBA[2];
		g_pSphereVertices[i].m_RGBA[2] = temp;
	}

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

	GutResetGraphicsDeviceDX9();

	// 设置视角转换矩阵
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

	device->Clear(
		0, NULL, // 清除整个画面 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // 清除颜色和Z buffer 
		D3DCOLOR_ARGB(0, 0, 0, 0), // 设置要把颜色清成黑色
		1.0f, // 设置要把Z值清为1, 也就是离镜头最远
		0 // 设置要把Stencil buffer清为0, 在这是否设置没有区别.
	);
	
	// 开始下绘图指令
	device->BeginScene(); 

	// 计算出一个可以转换到镜头坐标系的矩阵
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);

	// 设置数据格式
	// D3DFVF_XYZ = 使用3个浮点数来记录位置
	// D3DFVF_DIFFUSE = 使用32bits整数类型来记录BGRA颜色
	device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE); 

	// 画出球
	device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST, // 指定所要画的基本图形种类 
		0, // 会使用的最小顶点编号, 事实上没太大用处
		g_iNumSphereVertices, // 顶点数组里有几个顶点
		g_iNumSphereTriangles, // 要画出几个基本图形
		g_pSphereIndices, // 索引数组
		D3DFMT_INDEX16, // 索引数组的类型
		g_pSphereVertices, // 顶点数组
		sizeof(Vertex_VC) // 顶点数组里每个顶点的大小
	);

	// 声明所有的绘图指令都下完了
	device->EndScene(); 
	
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}
