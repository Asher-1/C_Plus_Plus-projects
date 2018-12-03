#include "Gut.h"
#include "render_data.h"
#include "GutModel_DX9.h"
#include "GutWin32.h"

CGutModel_DX9 g_Model_DX9;

static Matrix4x4 g_projection_matrix;
static Matrix4x4 g_mirror_view_matrix;

void InitStateDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 使用trilinear
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFOV, 1.0f, 0.1f, 100.0f);

	InitStateDX9();

	CGutModel::SetTexturePath("../../textures/");
	g_Model_DX9.ConvertToDX9Model(&g_Model);

	return true;
}

bool ReleaseResourceDX9(void)
{
	return true;
}

void ResizeWindowDX9(int width, int height)
{
	// Reset Device
	GutResetGraphicsDeviceDX9();
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFOV, aspect, 0.1f, 100.0f);

	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);

	InitStateDX9();
}

// 使用DirectX 9来绘图
void RenderFrameDX9(void)
{
	DWORD FogColor = D3DCOLOR_RGBA(128, 128, 128, 255);

	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->BeginScene(); 
	// 清除画面
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, FogColor, 1.0f, 0);
	// 设置转换矩阵
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 object_matrix = g_Control.GetObjectMatrix();

	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &object_matrix);
	// 设置雾
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);
	device->SetRenderState(D3DRS_FOGCOLOR, FogColor);

	switch(g_iFogMode)
	{
	case 0:
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		break;
	case 1:
		{
			// 随距离线性变浓的雾
			float fStart = 0.0f;
			float fEnd = 10.0f;
			device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fStart));
			device->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fEnd));
			// 计算公式为
			// (fog_end - distance_to_camera) / (fog_end - fog_start)
			break;
		}
	case 2:
		{
			// `使用指数函数来变化的雾 `
			float fFogDensity = 0.5f;
			device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP);
			device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)&fFogDensity);
			// 计算公式为
			// power(e, -(fog_density * distance_to_camera))
			break;
		}
	case 3:
		{
			// `使用指数函数来变化的雾 `
			float fFogDensity = 0.5f;
			device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP2);
			device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)&fFogDensity);
			// 计算公式为
			// power(e, -(fog_density * distance_to_camera)^2)
			break;
		}
	}
	// 画出模型
	g_Model_DX9.Render();
	// 声明所有的绘图指令都下完了
	device->EndScene(); 
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}
