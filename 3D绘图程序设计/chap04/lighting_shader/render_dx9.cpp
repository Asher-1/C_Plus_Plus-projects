#include "Gut.h"
#include "render_data.h"

static LPDIRECT3DVERTEXSHADER9 g_pDirLight_VS = NULL;
static LPDIRECT3DVERTEXSHADER9 g_pPointLight_VS = NULL;
static LPDIRECT3DVERTEXSHADER9 g_pSpotLight_VS = NULL;
static LPDIRECT3DVERTEXSHADER9 g_pSelected_VS = NULL;

static LPDIRECT3DPIXELSHADER9  g_pVertexColor_PS = NULL;

static Matrix4x4 g_view_matrix;
static Matrix4x4 g_view_proj_matrix;
static Matrix4x4 g_proj_matrix;

bool InitResourceDX9(void)
{
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `载入Pixel Shader`
	g_pVertexColor_PS = GutLoadPixelShaderDX9_HLSL("../../shaders/vertex_lighting_directional.shader", "PS", "ps_2_0");
	if ( g_pVertexColor_PS==NULL )
		return false;
	// `载入Directional Light Vertex Shader`
	g_pDirLight_VS = GutLoadVertexShaderDX9_HLSL("../../shaders/vertex_lighting_directional.shader", "VS", "vs_1_1");
	if ( g_pDirLight_VS==NULL )
		return false;
	// `载入Point Light Vertex Shader`
	g_pPointLight_VS = GutLoadVertexShaderDX9_HLSL("../../shaders/vertex_lighting_point.shader", "VS", "vs_1_1");
	if ( g_pPointLight_VS==NULL )
		return false;
	// `载入Spot Light Vertex Shader`
	g_pSpotLight_VS = GutLoadVertexShaderDX9_HLSL("../../shaders/vertex_lighting_spot.shader", "VS", "vs_1_1");
	if ( g_pSpotLight_VS==NULL )
		return false;

	// `视角转换矩阵`
	g_proj_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, 1.0f, 0.1f, 100.0f);
	// `镜头转换矩阵`
	g_view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	// `预先把view_matrix和proj_matrix相乘`
	g_view_proj_matrix = g_view_matrix * g_proj_matrix;

	// `画出正向和反向的三角形`
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return true;
}

bool ReleaseResourceDX9(void)
{
	SAFE_RELEASE(g_pVertexColor_PS);
	SAFE_RELEASE(g_pDirLight_VS);
	SAFE_RELEASE(g_pPointLight_VS);
	SAFE_RELEASE(g_pSpotLight_VS);

	return true;
}

void ResizeWindowDX9(int width, int height)
{
	GutResetGraphicsDeviceDX9();
	// `投影矩阵, 重设水平和垂直方向的视角.`
	float aspect = (float) height / (float) width;
	g_proj_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, aspect, 0.1f, 100.0f);
	g_view_proj_matrix = g_view_matrix * g_proj_matrix;
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `画出正向和反向的三角形`
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

static void SetupLightingDX9(void)
{
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	int base = 12;
	Vector4 vShininess = g_fMaterialShininess;
	Vector4 vAmbient, vDiffuse, vSpecular, vSpotLightCoeff;
	// `设置环境光`
	device->SetVertexShaderConstantF(base, &g_vAmbientLight[0], 1);
	
	// `设置光源`
	switch(g_iSelectedLight)
	{
	// `方向光`
	default:
	case 0:
		g_pSelected_VS = g_pDirLight_VS;
		
		vAmbient = g_vMaterialAmbient * g_Lights[0].m_vAmbientColor;
		vDiffuse = g_vMaterialDiffuse * g_Lights[0].m_vDiffuseColor;
		vSpecular = g_vMaterialSpecular * g_Lights[0].m_vSpecularColor;

		device->SetVertexShaderConstantF(base + 1, &g_Lights[0].m_vDirection.x, 1);
		device->SetVertexShaderConstantF(base + 2, &vAmbient.x, 1);
		device->SetVertexShaderConstantF(base + 3, &vDiffuse.x, 1);
		device->SetVertexShaderConstantF(base + 4, &vSpecular.x, 1);
		device->SetVertexShaderConstantF(base + 5, &vShininess.x, 1);

		break;
	// `点光源`
	case 1:
		g_pSelected_VS = g_pPointLight_VS;
		
		vAmbient = g_vMaterialAmbient * g_Lights[1].m_vAmbientColor;
		vDiffuse = g_vMaterialDiffuse * g_Lights[1].m_vDiffuseColor;
		vSpecular = g_vMaterialSpecular * g_Lights[1].m_vSpecularColor;

		device->SetVertexShaderConstantF(base + 1, &g_Lights[1].m_vPosition.x, 1);
		device->SetVertexShaderConstantF(base + 2, &g_Lights[1].m_vAttenuation.x, 1);
		device->SetVertexShaderConstantF(base + 3, &vAmbient.x, 1);
		device->SetVertexShaderConstantF(base + 4, &vDiffuse.x, 1);
		device->SetVertexShaderConstantF(base + 5, &vSpecular.x, 1);
		device->SetVertexShaderConstantF(base + 6, &vShininess.x, 1);

		break;
	// `聚光灯`
	case 2:
		g_pSelected_VS = g_pSpotLight_VS;
		
		vAmbient = g_vMaterialAmbient * g_Lights[2].m_vAmbientColor;
		vDiffuse = g_vMaterialDiffuse * g_Lights[2].m_vDiffuseColor;
		vSpecular = g_vMaterialSpecular * g_Lights[2].m_vSpecularColor;

		float spotlight_cutoff_cosine = FastMath::Cos( FastMath::DegreeToRadian(g_Lights[2].m_fSpotlightCutoff * 0.5f) );
		vSpotLightCoeff.Set(spotlight_cutoff_cosine, g_Lights[2].m_fSpotlightExponent, 0.0f, 0.0f);

		device->SetVertexShaderConstantF(base + 1, &g_Lights[2].m_vPosition.x, 1);
		device->SetVertexShaderConstantF(base + 2, &g_Lights[2].m_vDirection.x, 1);
		device->SetVertexShaderConstantF(base + 3, &g_Lights[2].m_vAttenuation.x, 1);
		device->SetVertexShaderConstantF(base + 4, &vSpotLightCoeff.x, 1);
		device->SetVertexShaderConstantF(base + 5, &vAmbient.x, 1);
		device->SetVertexShaderConstantF(base + 6, &vDiffuse.x, 1);
		device->SetVertexShaderConstantF(base + 7, &vSpecular.x, 1);
		device->SetVertexShaderConstantF(base + 8, &vShininess.x, 1);

		break;
	}
}

// `使用Direct3D9来绘图`
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `清除画面`
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	// `开始下绘图指令`
	device->BeginScene(); 
	// `设置数据格式`
	device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);
	// `使用shader`
	device->SetVertexShader(g_pSelected_VS);
	device->SetPixelShader(g_pVertexColor_PS);
	// `设置光源`
	SetupLightingDX9();
	// `设置转换矩阵`
	Matrix4x4 world_view_proj_matrix = g_world_matrix * g_view_proj_matrix;
	device->SetVertexShaderConstantF(0, &world_view_proj_matrix[0][0], 4);
	device->SetVertexShaderConstantF(4, &g_world_matrix[0][0], 4);
	// `镜头位置, 计算Specular会用到.`
	device->SetVertexShaderConstantF(8, &g_eye[0], 1);
	// `画出格子`
	device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, g_iNumGridVertices, g_iNumGridTriangles, 
		g_pGridIndices, D3DFMT_INDEX16, g_pGridVertices, sizeof(Vertex_V3N3) );
	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
