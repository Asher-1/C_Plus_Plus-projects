#ifdef _ENABLE_DX10_

#include "Gut.h"
#include "render_data.h"

static ID3D10Device			*g_pDevice			= NULL;
static ID3D10InputLayout	*g_pVertexLayout	= NULL;

static ID3D10Buffer			*g_pVertexBuffer	= NULL;
static ID3D10Buffer			*g_pIndexBuffer		= NULL;

static ID3D10Buffer			*g_pMatrixConstantBuffer= NULL;
static ID3D10Buffer			*g_pLightConstantBuffer	= NULL;

static ID3D10VertexShader	*g_pVertexShader	= NULL;
static ID3D10PixelShader	*g_pPixelShader		= NULL;
static ID3D10RasterizerState *g_pRasterizerState= NULL;

static Matrix4x4 g_proj_matrix;
static Matrix4x4 g_view_matrix;
static Matrix4x4 g_view_proj_matrix;

struct LightBuffer
{
	Vector4 vAmbientLight;
	Vector4 vMaterialShininess;
	Light_Info Lights[3];
};

bool InitResourceDX10(void)
{
	g_pDevice = GutGetGraphicsDeviceDX10();
	ID3D10Blob *pVSCode = NULL;

	// `载入Vertex Shader`
	g_pVertexShader = GutLoadVertexShaderDX10_HLSL("../../shaders/vertex_lighting_all_dx10.hlsl", "VS", "vs_4_0", &pVSCode);
	if ( NULL==g_pVertexShader )
		return false;
	// `载入Pixel Shader`
	g_pPixelShader = GutLoadPixelShaderDX10_HLSL("../../shaders/vertex_lighting_all_dx10.hlsl", "PS", "ps_4_0");
	if ( NULL==g_pPixelShader )
		return false;

    // `设置Vertex数据格式`
    D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

    if ( D3D_OK != g_pDevice->CreateInputLayout( layout, sizeof(layout)/sizeof(D3D10_INPUT_ELEMENT_DESC), pVSCode->GetBufferPointer(), pVSCode->GetBufferSize(), &g_pVertexLayout ) )
		return false;

	SAFE_RELEASE(pVSCode);

	D3D10_BUFFER_DESC cbDesc;
	// vertex buffer
    cbDesc.ByteWidth = sizeof(Vertex_V3N3) * g_iMaxNumGridVertices;
    cbDesc.Usage = D3D10_USAGE_DYNAMIC ;
    cbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
	// `分配一块可以存放Vertex的内存, 也就是Vertex Buffer.`
	if ( D3D_OK != g_pDevice->CreateBuffer( &cbDesc, NULL, &g_pVertexBuffer ) )
		return false;

	// `设置一块可以用来放Index的内存.`
    cbDesc.ByteWidth = sizeof(unsigned short) * g_iMaxNumGridIndices;
    cbDesc.Usage = D3D10_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
    // `分配一块可以存放Index的内存, 也就是Index Buffer.`
	if ( D3D_OK != g_pDevice->CreateBuffer( &cbDesc, NULL, &g_pIndexBuffer ) )
		return false;

	// `分配Shader读取参数的内存空间`
    cbDesc.ByteWidth = sizeof(Matrix4x4) * 3;
    cbDesc.Usage = D3D10_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
	if ( D3D_OK != g_pDevice->CreateBuffer( &cbDesc, NULL, &g_pMatrixConstantBuffer ) )
		return false;

    cbDesc.ByteWidth = sizeof(LightBuffer);
    cbDesc.Usage = D3D10_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
	if ( D3D_OK != g_pDevice->CreateBuffer( &cbDesc, NULL, &g_pLightConstantBuffer ) )
		return false;

	// `rasterizer state对象`
	D3D10_RASTERIZER_DESC rasterizer_state_desc;
	
	rasterizer_state_desc.FillMode = D3D10_FILL_SOLID;
	rasterizer_state_desc.CullMode = D3D10_CULL_NONE;
	rasterizer_state_desc.FrontCounterClockwise = true;
	rasterizer_state_desc.DepthBias = 0;
	rasterizer_state_desc.DepthBiasClamp = 0.0f;
	rasterizer_state_desc.SlopeScaledDepthBias = 0.0f;
	rasterizer_state_desc.DepthClipEnable = false;
	rasterizer_state_desc.ScissorEnable = false;
	rasterizer_state_desc.MultisampleEnable = false;
	rasterizer_state_desc.AntialiasedLineEnable = false;

	if ( D3D_OK != g_pDevice->CreateRasterizerState(&rasterizer_state_desc, &g_pRasterizerState) )
		return false;

	g_pDevice->RSSetState(g_pRasterizerState);

	// `计算投影和镜头转换矩阵`
	g_proj_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, 1.0f, 0.1f, 100.0f);
	g_view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// `先把两个矩阵相乘`
	g_view_proj_matrix = g_view_matrix * g_proj_matrix;

	return true;
}

bool ReleaseResourceDX10(void)
{
 	SAFE_RELEASE(g_pVertexLayout);
 	SAFE_RELEASE(g_pVertexBuffer);
 	SAFE_RELEASE(g_pIndexBuffer);
 	SAFE_RELEASE(g_pMatrixConstantBuffer);
 	SAFE_RELEASE(g_pLightConstantBuffer);
 	SAFE_RELEASE(g_pVertexShader);
 	SAFE_RELEASE(g_pPixelShader);
	SAFE_RELEASE(g_pRasterizerState);

	return true;
}

void ResizeWindowDX10(int width, int height)
{
	GutResetGraphicsDeviceDX10();
	float aspect = (float) height / (float) width;
	g_proj_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, aspect, 0.1f, 100.0f);

	g_view_proj_matrix = g_view_matrix * g_proj_matrix;
}

void RenderFrameDX10(void)
{
	Vector4 vClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	UINT stride = sizeof(Vertex_V3N3);
	UINT offset = 0;
	// `获得调用GutCreateGraphicsDeviceDX10时所生成的D3D10对象`
	//frame buffer
	ID3D10RenderTargetView *pRenderTargetView = GutGetDX10RenderTargetView(); 
	//depth/stencil buffer
    ID3D10DepthStencilView *pDepthStencilView = GutGetDX10DepthStencilView(); 
	// front/back buffer
	IDXGISwapChain *pSwapChain = GutGetDX10SwapChain(); 
	// `清除颜色`
	g_pDevice->ClearRenderTargetView(pRenderTargetView, (float *)&vClearColor);
	// `清除`Depth/Stencil buffer
	g_pDevice->ClearDepthStencilView(pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
	// `设置`vertex shader
	g_pDevice->VSSetShader(g_pVertexShader);
	// `设置`pixel shader
	g_pDevice->PSSetShader(g_pPixelShader);
	// `设置vertex shader读取参数的内存位罝`
    g_pDevice->VSSetConstantBuffers(0, 1, &g_pMatrixConstantBuffer);
    g_pDevice->VSSetConstantBuffers(1, 1, &g_pLightConstantBuffer);
	// `设置vertex数据格式`
	g_pDevice->IASetInputLayout(g_pVertexLayout);
	// `设置`vertex buffer
	g_pDevice->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	// `设置`index buffer
	g_pDevice->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// `设置三角形顶点索引值数据排列是`triangle strip
	g_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// `计算矩阵`
	Matrix4x4 world_view_proj_matrix = g_world_matrix * g_view_proj_matrix;
	
	// `设置shader参数`
	Matrix4x4 *pMatrices = NULL;
	g_pMatrixConstantBuffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, (void **) &pMatrices );
	pMatrices[0] = world_view_proj_matrix;
	pMatrices[1] = g_world_matrix;
	pMatrices[2][0] = g_eye;
	g_pMatrixConstantBuffer->Unmap();

	LightBuffer *pLightBuffer = NULL;
	g_pLightConstantBuffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, (void **) &pLightBuffer );
	pLightBuffer->vAmbientLight = g_vAmbientLight;
	pLightBuffer->vMaterialShininess = g_fMaterialShininess;
	pLightBuffer->Lights[0] = g_Lights[0];
	pLightBuffer->Lights[1] = g_Lights[1];
	pLightBuffer->Lights[2] = g_Lights[2];
	pLightBuffer->Lights[2].m_fSpotlightCutoff = FastMath::Cos(FastMath::DegreeToRadian(g_Lights[2].m_fSpotlightCutoff*0.5f));
	for ( int i=0; i<3; i++ )
	{
		pLightBuffer->Lights[i].m_vAmbientColor *= g_vMaterialAmbient;
		pLightBuffer->Lights[i].m_vDiffuseColor *= g_vMaterialDiffuse;
		pLightBuffer->Lights[i].m_vSpecularColor *= g_vMaterialSpecular;
	}
	g_pLightConstantBuffer->Unmap();

	// `更新顶点数据`
	Vertex_V3N3 *pVertex;
	g_pVertexBuffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, (void **) &pVertex );
	memcpy(pVertex, g_pGridVertices, sizeof(Vertex_V3N3) * g_iNumGridVertices);
	g_pVertexBuffer->Unmap();
	// `更新索引值数组`
	unsigned short *pIndex;
	g_pIndexBuffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, (void **) &pIndex );
	memcpy(pIndex, g_pGridIndices, sizeof(unsigned short) * g_iNumGridIndices);
	g_pIndexBuffer->Unmap();
	// `画出格子`
	g_pDevice->DrawIndexed(g_iNumGridIndices, 0, 0);
	// `等待硬件扫描结束, 然后再更新画面`
	pSwapChain->Present(1, 0);
}

#else

void ResizeWindowDX10(int width, int height) {}
void RenderFrameDX10(void) {}
bool ReleaseResourceDX10(void) { return false; }
bool InitResourceDX10(void) { return false; }

#endif // _ENABLE_DX10_