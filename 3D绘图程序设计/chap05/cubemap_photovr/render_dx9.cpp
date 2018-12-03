#include <d3dx9.h>

#include "Gut.h"
#include "GutWin32.h"
#include "GutTexture_DX9.h"
#include "GutModel_DX9.h"

#include "render_data.h"

static LPDIRECT3DCUBETEXTURE9 g_pTexture = NULL;
static LPDIRECT3DVERTEXDECLARATION9 g_pVertexDecl = NULL;
static CGutModel_DX9 g_Model_DX9;

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
	// 载入贴图
	const char *texture_array[] = 
	{
		"../../textures/uffizi_right.tga",
		"../../textures/uffizi_left.tga",
		"../../textures/uffizi_top.tga",
		"../../textures/uffizi_bottom.tga",
		"../../textures/uffizi_back.tga", // `右手坐标系上 Z+ 为镜头后方.`
		"../../textures/uffizi_front.tga" // `右手坐标系上 Z- 为镜头前方.`
	};

	g_pTexture = GutLoadCubemapTexture_DX9(texture_array);
	if ( g_pTexture==NULL )
	{
		// 有些旧硬件不支持 mipmapped cubemap , 改载入 dds 的版本.
		g_pTexture = GutLoadCubemapTexture_DX9("../../textures/uffizi_cubemap.dds");
		if ( g_pTexture==NULL )
			return false;
	}

	// 设置顶点数据格式
	D3DVERTEXELEMENT9 decl[] = 
	{
		// float*3 for position (x,y,z)
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		// float*3 for texcoord (x,y,z)
		{0,12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	device->CreateVertexDeclaration(decl, &g_pVertexDecl);

	return true;
}

bool ReleaseResourceDX9(void)
{
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pVertexDecl);

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

// `使用Direct3D9来绘图`
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `清除画面`
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	// `开始下绘图指令`
	device->BeginScene(); 
	// `设置转换矩阵`
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 world_matrix = g_Control.GetObjectMatrix();
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_matrix);
	// `设置数据格式`
	device->SetVertexDeclaration(g_pVertexDecl); 
	// `使用贴图`
	device->SetTexture(0, g_pTexture);
	// trilinear filter
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	// `画球`
	device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,	// `指定所要画的基本图形种类` 
		0, // `会使用的最小顶点编号, 事实上没太大用处.`
		g_iNumSphereVertices, // `顶点数组里有几个顶点`
		g_iNumSphereTriangles, // `要画出几个基本图形`
		g_pSphereIndices, // `索引数组`
		D3DFMT_INDEX16, // `索引数组的类型`
		g_pSphereVertices, // `顶点数组`
		sizeof(Vertex_VN) // `顶点数组里每个顶点的大小`
		);
	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
