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
	device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
}

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFOV, 1.0f, 0.1f, 100.0f);
	// 
	InitStateDX9();
	//
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
	// 获得Direct3D9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(g_fFOV, aspect, 0.1f, 100.0f);

	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);

	InitStateDX9();
}

static void RenderModelDX9(bool mirror, Vector4 *pPlane)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	Matrix4x4 view_matrix;

	if ( mirror )
	{
		Vector4 vEye = g_Control.GetCameraPosition();
		Vector4 vLookAt = g_Control.m_vLookAt;
		Vector4 vUp = g_Control.m_vUp;

		Vector4 mirror_eye = MirrorPoint(vEye, *pPlane);
		Vector4 mirror_lookat = MirrorPoint(vLookAt, *pPlane);
		Vector4 mirror_up = MirrorVector(vUp, *pPlane);

		Matrix4x4 temp_matrix = GutMatrixLookAtRH(mirror_eye, mirror_lookat, mirror_up);

		// 因为是镜射, 在转换到镜头坐标系后要做个左右对调的动作.
		Matrix4x4 mirror_x;
		mirror_x.Identity();
		mirror_x.Scale(-1.0f, 1.0f, 1.0f);

		view_matrix = temp_matrix * mirror_x;

		// 右左对调后, 3角形的顶点排列顺序会被反过来.
		GutSetDX9BackFace(D3DCULL_CCW);
	}
	else
	{
		view_matrix = g_Control.GetViewMatrix();
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		GutSetDX9BackFace(D3DCULL_CW);
	}

	Matrix4x4 world_matrix = g_Control.GetObjectMatrix();

	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &world_matrix);

	g_Model_DX9.Render();

	GutSetDX9BackFace(D3DCULL_CW);
}

// 使用Direct3D9来绘图
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	Vector4 vPlane(0.0f, 0.0f, 1.0f, -g_mirror_z);

	device->BeginScene(); 
	// `清除画面`
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_RGBA(0, 0, 150, 255), 1.0f, 0);
	// `画出茶壶`
	RenderModelDX9(false, NULL);

	// `画出镜面, 同时把stencil值设为1.`
	{	
		device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_STENCILREF, 1);

		Matrix4x4 identity_matrix = Matrix4x4::IdentityMatrix();
		device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &identity_matrix);
		
		sModelMaterial_DX9 material;
		material.m_Material.Diffuse.r = material.m_Material.Diffuse.g = material.m_Material.Diffuse.b = material.m_Material.Diffuse.a = 0.0f;
		material.Submit();
		// `画出镜面`
		device->SetFVF(D3DFVF_XYZ);
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad_v, sizeof(Vertex_V));
	}
	// `把镜面范围的ZBuffer清为1.0`
	{
		// `设置stencil test条件, 只更新镜面范围的像素.`
		device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS); // `关闭z test`
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0); // `关闭RGBA的输出`
		// `通过修改viewport Z范围的方法, 把3D对象的Z值输出固定为1.`
		int w, h; 
		GutGetWindowSize(w, h);
		D3DVIEWPORT9 viewport;
		viewport.X = viewport.Y = 0;
		viewport.Width = w; viewport.Height = h;
		viewport.MinZ = 1.0f; viewport.MaxZ = 1.0f;
		device->SetViewport(&viewport);
		// `画出镜面`
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_Quad_v, sizeof(Vertex_V));
		// `把Z值范围还原成0-1`
		viewport.MinZ = 0.0f; viewport.MaxZ = 1.0f;
		device->SetViewport(&viewport);
		// `还原zbuffer test的测试条件`
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		// `重新打开RGBA的输出`
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 
			D3DCOLORWRITEENABLE_RED | 
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE |
			D3DCOLORWRITEENABLE_ALPHA);
	}
	// `在镜面范围里画出镜射的茶壶`
	{
		RenderModelDX9(true, &vPlane);
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
