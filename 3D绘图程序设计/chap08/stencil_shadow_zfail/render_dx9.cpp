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
	//
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
}

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFOV, 1.0f, 0.1f, 100.0f);

	InitStateDX9();

	CGutModel::SetTexturePath("../../textures/");
	g_Model_DX9.ConvertToDX9Model(&g_Model_textured);

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
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 world_matrix = g_Control.GetObjectMatrix();
	Matrix4x4 ident_matrix; ident_matrix.Identity();
	device->BeginScene(); 
	// 清除画面
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_RGBA(30, 30, 30, 255), 1.0f, 0);
	// light position & orientation
	Vector4 light_pos(5.0f, 0.0f, 5.0f);
	Vector4 light_lookat(0.0f, 0.0f, 0.0f);
	Vector4 light_up(0.0f, 1.0f, 0.0f);
	// light matrix
	Matrix4x4 light_view = GutMatrixLookAtRH(light_pos, light_lookat, light_up);
	Matrix4x4 light_world_view = world_matrix * light_view;
	Matrix4x4 shadow_matrix;
	// 创建shadow volume
	if ( g_bDirectionalLight )
	{
		g_ShadowVolume.BuildShadowVolume_DirectionalLight(light_world_view, 20.0f, true);
		shadow_matrix = light_view;
		shadow_matrix.FastInvert();
	}
	else
	{
		g_ShadowVolume.BuildShadowVolume_PointLight(light_pos, world_matrix, 20.0f, true);
		shadow_matrix.Identity();
	}
	// 画出空间中的茶壶
	{
		device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
		device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_matrix);

		g_Model_DX9.Render();
	}
	// 画出墙壁
	{
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &ident_matrix);

		sModelMaterial_DX9 material;

		material.m_Material.Diffuse.r = 0.0f;
		material.m_Material.Diffuse.g = 0.0f;
		material.m_Material.Diffuse.b = 1.0f;
		material.m_Material.Diffuse.a = 1.0f;

		material.Submit();

		device->SetFVF(D3DFVF_XYZ);
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad, sizeof(Vertex_VT));
	}
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	device->SetRenderState(D3DRS_STENCILREF, 0x01);
	device->SetRenderState(D3DRS_STENCILMASK, 0xff);
	// 在Stencil Buffer上标示出阴影区域
	{
		sModelMaterial_DX9 material;
		material.m_bCullFace = false;
		material.Submit();
		// 使用矩阵
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &shadow_matrix);
		// 设置顶点数据格式
		device->SetFVF(D3DFVF_XYZ);
		// `启动 Two sided stencil test 功能`
		device->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);
		// `顺时针方向的 Stencil Test 设置`
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
		// `逆时针方向的 Stencil Test 设置`
		device->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_DECR);
		// `关闭颜色的更新`
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
		// 画出Shadow Volume
		device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, g_ShadowVolume.m_iNumShadowVolumeFaces, g_ShadowVolume.m_pShadowVolume, sizeof(Vector4));
		// `还原设置给正常绘图使用`
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xff);
		device->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
		device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
	}
	// 画出阴影
	{
		sModelMaterial_DX9 material;
		material.m_bCullFace = false;
		material.Submit();

		device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
		// 使用矩阵
		device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &ident_matrix);
		device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &ident_matrix);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &ident_matrix);
		// 只更新stencil buffer上值为1的像素
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		device->SetRenderState(D3DRS_STENCILREF, 0x01);
		// 使用黑色再画一次墙壁
		device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(0, 0, 0,255) );
		device->SetFVF(D3DFVF_XYZ);
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_FullScreenQuad, sizeof(Vertex_VT));
	}
	device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	// 观察shadow volume, 调试用.
	if ( g_bDrawShadowVolume )
	{
		device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
		device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &shadow_matrix);

		sModelMaterial_DX9 material;
		material.m_bCullFace = false;
		material.Submit();

		device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_RGBA(255, 255, 255, 255) );
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, g_ShadowVolume.m_iNumShadowVolumeFaces, g_ShadowVolume.m_pShadowVolume, sizeof(Vector4));
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	// 恢复更新zbuffer
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	// 把颜色来源还原为 diffuse * texture
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	// 声明所有的绘图指令都下完了
	device->EndScene(); 
	// 把背景backbuffer的画面显示出来
    device->Present( NULL, NULL, NULL, NULL );
}
