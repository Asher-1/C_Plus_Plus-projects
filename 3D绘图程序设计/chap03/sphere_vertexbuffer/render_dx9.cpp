#include "Gut.h"
#include "render_data.h"

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

bool InitResourceDX9(void)
{
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `设置视角转换矩阵`
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(90.0f, 1.0f, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// `关闭光照`
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// `改变三角形正面的法线`
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	for ( int i=0; i<g_iNumSphereVertices; i++ )
	{
		unsigned char temp = g_pSphereVertices[i].m_RGBA[0];
		g_pSphereVertices[i].m_RGBA[0] = g_pSphereVertices[i].m_RGBA[2];
		g_pSphereVertices[i].m_RGBA[2] = temp;
	}
	
	int vsize = g_iNumSphereVertices * sizeof(Vertex_VC);

	if ( D3D_OK!=device->CreateVertexBuffer(vsize, 0, 0, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL) )
	{
		return false;
	}

	Vertex_VC *pVertexArray = NULL;
	// `获得Vertex Buffer的内存位置`
	g_pVertexBuffer->Lock(0, vsize, (void **)&pVertexArray, 0);
	memcpy(pVertexArray, g_pSphereVertices, vsize);
	g_pVertexBuffer->Unlock();

	int isize = g_iNumSphereIndices * sizeof(unsigned short);

	if ( D3D_OK!=device->CreateIndexBuffer(isize, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL) )
	{
		return false;
	}

	unsigned short *pIndexArray = NULL;
	// `获得Index Buffer的内存位置`
	g_pIndexBuffer->Lock(0, isize, (void **)&pIndexArray, 0);
	memcpy(pIndexArray, g_pSphereIndices, isize);
	g_pIndexBuffer->Unlock();

	return true;
}

bool ReleaseResourceDX9(void)
{
	if ( g_pVertexBuffer )
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}

	if ( g_pIndexBuffer )
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}

	return true;
}

void ResizeWindowDX9(int width, int height)
{
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	GutResetGraphicsDeviceDX9();

	// `设置视角转换矩阵`
	float aspect = (float) height / (float) width;
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(90.0f, aspect, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);

	// `关闭光照`
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// `改变三角形正面的法线`
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

// `使用Direct3D9来绘图`
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	
	// `开始下绘图指令`
	device->BeginScene(); 

	// `计算出一个可以转换到镜头坐标系的矩阵`
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);

	// `设置数据格式`
	// D3DFVF_XYZ = `使用3个浮点数来记录位置`
	// D3DFVF_DIFFUSE = `使用32bits整数类型来记录BGRA颜色`
	device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE); 

	device->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex_VC));
	device->SetIndices(g_pIndexBuffer);

	// `画出球`
	device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, // `指定所要画的基本图形种类`
		0, 
		0, // `会使用的最小顶点编号, 事实上没太大用处.`
		g_iNumSphereVertices, // `顶点数组里有几个顶点`
		0, 
		g_iNumSphereTriangles // `要画出几个基本图形`
	);

	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
