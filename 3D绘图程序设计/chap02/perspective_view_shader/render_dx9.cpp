#include "Gut.h"
#include "render_data.h"

static LPDIRECT3DVERTEXSHADER9 g_pVertexShaderDX9 = NULL;
static LPDIRECT3DPIXELSHADER9  g_pPixelShaderDX9 = NULL;
static LPDIRECT3DVERTEXDECLARATION9 g_pVertexShaderDecl = NULL;

bool InitResourceDX9(void)
{
	// `载入Vertex Shader`
	g_pVertexShaderDX9 = GutLoadVertexShaderDX9_HLSL(
		"../../shaders/vertex_transform.shader", "VS", "vs_1_1");
	if ( g_pVertexShaderDX9==NULL )
		return false;
	// `载入Pixel Shader`
	g_pPixelShaderDX9 = GutLoadPixelShaderDX9_HLSL(
		"../../shaders/vertex_transform.shader", "PS", "ps_2_0");
	if ( g_pPixelShaderDX9==NULL )
		return false;

	// `镜头坐标系转换矩阵`
	Matrix4x4 view_matrix = 
		GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// `投影转换矩阵`
	Matrix4x4 projection_matrix = 
		GutMatrixPerspectiveRH_DirectX(90.0f, 1.0f, 1.0f, 100.0f);
	// `把前两个矩阵相乘`
	Matrix4x4 view_projection_matrix = 
		view_matrix * projection_matrix;
	
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `设置视角转换矩阵`
	device->SetVertexShaderConstantF(0, (float *)&view_projection_matrix, 4);

	return true;
}

bool ReleaseResourceDX9(void)
{
	if ( g_pVertexShaderDX9 )
	{
		g_pVertexShaderDX9->Release();
		g_pVertexShaderDX9 = NULL;
	}

	if ( g_pPixelShaderDX9 )
	{
		g_pPixelShaderDX9->Release();
		g_pPixelShaderDX9 = NULL;
	}

	return true;
}

// `使用DirectX9来绘图`
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
    device->Clear(
		0, NULL, // `清除整个画面`
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // `清除颜色和Z Buffer`
		D3DCOLOR_ARGB(0, 0, 0, 0), // `设置要把颜色清成黑色`
		1.0f, // `设置要把Z值清为1, 也就是离镜头最远.`
		0 // `设置要把Stencil buffer清为0, 在这是否设置没有区别.`
	);
	
	// `开始下绘图指令`
	device->BeginScene(); 
	// `设置数据格式`
	device->SetFVF(D3DFVF_XYZW); 
	// `设置Vertex/Pixel Shader`
	device->SetVertexShader(g_pVertexShaderDX9);
	device->SetPixelShader(g_pPixelShaderDX9);
	// `画出金字塔的8条边线`
	device->DrawPrimitiveUP(D3DPT_LINELIST, 8, g_vertices, sizeof(Vector4)); 
	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
