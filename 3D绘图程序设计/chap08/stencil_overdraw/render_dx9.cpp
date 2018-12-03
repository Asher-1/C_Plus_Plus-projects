#include "Gut.h"
#include "render_data.h"
#include "GutModel_DX9.h"
#include "GutWin32.h"

CGutModel_DX9 g_Model_DX9;

static Matrix4x4 g_projection_matrix;

void InitStateDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
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
	g_Model_DX9.Release();
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
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	device->BeginScene(); 
	// `清除画面`
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// `画出茶壶, 并更新 Stencil Buffer.`
	{
		Matrix4x4 view_matrix = g_Control.GetViewMatrix();
		Matrix4x4 world_matrix = g_Control.GetObjectMatrix();

		device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
		device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_matrix);

		// `启动 Stencil Test`
		device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		// `设置 Stencil Test 条件, 让它永远成立.`
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		// `递增 Stencil Buffer 内容`
		device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
		// `画出茶壶`
		g_Model_DX9.Render();
		// `不再去改变 Stencil Buffer 内容`
		device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	}

	Matrix4x4 identity_matrix = Matrix4x4::IdentityMatrix();
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &identity_matrix);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &identity_matrix);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &identity_matrix);
	device->SetFVF(D3DFVF_XYZ);
	device->SetRenderState(D3DRS_ZENABLE, FALSE);

	// `还原纹理设置`
	sModelMaterial_DX9 material;
	material.Submit();

	// `设置颜色来源为 texture factor`
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

	// `用绿色标示出只更新1次的像素`
	{	
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		device->SetRenderState(D3DRS_STENCILREF, 1);
		device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(0,255,0,255));
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_V));
	}
	// `用蓝色标示出更新2次的像素`
	{	
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		device->SetRenderState(D3DRS_STENCILREF, 2);
		device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(0,0,255,255));
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_V));
	}
	// `用红色标示出更新3次的像素`
	{	
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		device->SetRenderState(D3DRS_STENCILREF, 3);
		device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255,0,0,255));
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_V));
	}
	// `用白色标示出更新超过3次的像素`
	{	
		// `如果3小于stencil buffer的值, 条件才成立.`
		// `也就是stencil buffer值要大于3的意思`
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESS);
		device->SetRenderState(D3DRS_STENCILREF, 3);
		device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255,255,255,255));
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_V));
	}
	// `还原设置`
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	// `设置颜色来源为` diffuse * texture
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
