#include "Gut.h"
#include "render_data.h"

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, 1.0f, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// 计算出一个可以转换到镜头坐标系的矩阵
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
	// 画出正向和反向的三角形
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return true;
}

bool ReleaseResourceDX9(void)
{
	return true;
}

void ResizeWindowDX9(int width, int height)
{
	GutResetGraphicsDeviceDX9();
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFovW, aspect, 0.1f, 100.0f);
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// 画出正向和反向的三角形
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

D3DCOLOR ConvertToD3DCOLOR(Vector4 &vColor)
{
	Vector4 vColor_0_255 = vColor;
	vColor_0_255.Saturate();
	vColor_0_255 *= 255.0f;

	int r = (int) vColor_0_255.GetX();
	int g = (int) vColor_0_255.GetY();
	int b = (int) vColor_0_255.GetZ();
	int a = (int) vColor_0_255.GetW();

	D3DCOLOR d3dcolor = D3DCOLOR_ARGB(a, r, g, b);
	return d3dcolor;
}

void SetupLightingDX9(void)
{
	// `获得Direct3D9设备`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	
	// `打开光照功能`
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	// `设置环境光`
	device->SetRenderState(D3DRS_AMBIENT, ConvertToD3DCOLOR(g_vGlobal_AmbientLight));
	// `指定Diffuse纹理的数据来源, 把它设为从SetMaterial函数中来设置.`
	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	// `设置对象纹理的反光能力`
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(mtrl) );
	mtrl.Ambient = *(D3DCOLORVALUE*) &g_vMaterialAmbient;
	mtrl.Diffuse = *(D3DCOLORVALUE*) &g_vMaterialDiffuse;
	mtrl.Specular = *(D3DCOLORVALUE*) &g_vMaterialSpecular;
	mtrl.Emissive = *(D3DCOLORVALUE*) &g_vMaterialEmissive;
	// `通过调用SetMaterial来设置纹理`
	device->SetMaterial(&mtrl);
	// `设置光源`
	for ( int i=0; i<g_iNumLights; i++ )
	{
		if ( g_Lights[i].m_bEnabled )
		{
			// `打开这盏光`
			device->LightEnable(i, TRUE);
			// `D3D9通过设置D3DLIGHT9 struct来设置光源属性`
			D3DLIGHT9 light;
			ZeroMemory( &light, sizeof(light) );

			switch(g_Lights[i].m_eType)
			{
			case LIGHT_DIRECTIONAL:
				// `设置成方向光`
				light.Type = D3DLIGHT_DIRECTIONAL;
				// `设置方向`
				light.Direction = *(D3DVECTOR *) &g_Lights[i].m_vDirection;	
				break;
			case LIGHT_POINT:
				// `设置成点光源`
				light.Type = D3DLIGHT_POINT;
				// `设置位置`
				light.Position = *(D3DVECTOR *) &g_Lights[i].m_vPosition;
				// `随距离的衰减值`
				// 1/(CONSTANT+LINEAR*d+QUADRATIC*d^2)
				// `公式中的CONSTANT, LINEAR, QUADRATIC值`
				light.Attenuation0 = g_Lights[i].m_vAttenuation[0];
				light.Attenuation1 = g_Lights[i].m_vAttenuation[1];
				light.Attenuation2 = g_Lights[i].m_vAttenuation[2];
				break;
			case LIGHT_SPOT:
				// `设置成聚光灯`
				light.Type = D3DLIGHT_SPOT;
				// `设置位置`
				light.Position = *(D3DVECTOR *) &g_Lights[i].m_vPosition;
				// `设置方向`
				light.Direction = *(D3DVECTOR *) &g_Lights[i].m_vDirection;	
				// `随距离的衰减值`
				// 1/(CONSTANT+LINEAR*d+QUADRATIC*d^2)
				// `公式中的CONSTANT, LINEAR, QUADRATIC值`
				light.Attenuation0 = g_Lights[i].m_vAttenuation[0];
				light.Attenuation1 = g_Lights[i].m_vAttenuation[1];
				light.Attenuation2 = g_Lights[i].m_vAttenuation[2];
				// `光柱圆锥的一半角度`
				light.Phi = FastMath::DegreeToRadian(g_Lights[i].m_fSpotlightCutoff);
				// `Inner Cone的角度, Inner Cone内部没有角度衰灭.`
				light.Theta = FastMath::DegreeToRadian(g_Lights[i].m_fSpotLightInnerCone);
				// `光柱的角度衰灭值`
				light.Falloff = g_Lights[i].m_fSpotlightExponent;
				break;
			}
			// `光源的Ambient值`
			light.Ambient = *(D3DCOLORVALUE*) &g_Lights[i].m_vAmbientColor;
			// `光源的Diffuse值`
			light.Diffuse = *(D3DCOLORVALUE*) &g_Lights[i].m_vDiffuseColor;
			// `光源有效范围, 在此把它设置成浮点数所容计的最大值.`
			light.Range = 1000.0f;
			// `用SetLight来设置光源属性`
			device->SetLight(i, &light);
		}
		else
		{
			// `关闭这盏光`
			device->LightEnable(i, FALSE);
		}
	}
}

// 使用DirectX 9来绘图
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 清除画面
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	// 开始下绘图指令
	device->BeginScene(); 
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &g_world_matrix);
	// 设置光源
	SetupLightingDX9();
	// 设置数据格式
	device->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL);
	// 画出格子
	device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP, 0, g_iNumGridVertices, g_iNumGridTriangles, 
		g_pGridIndices, D3DFMT_INDEX16, g_pGridVertices, sizeof(Vertex_V3N3) );
	// 声明所有的绘图指令都下完了
	device->EndScene(); 
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}
