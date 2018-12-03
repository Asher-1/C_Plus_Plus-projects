#include "Gut.h"
#include "render_data.h"
#include "GutModel_DX9.h"

#include "GutWin32.h"

Matrix4x4 g_projection_matrix;
LPDIRECT3DQUERY9 g_pOcclusionQuery[2];
static CGutModel_DX9 g_Models_DX9[3];

bool InitResourceDX9(void)
{
	// 获得Direct3D9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// 设置视角转换矩阵
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(90.0f, 1.0f, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 改变三角形正面的法线
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//
	device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &g_pOcclusionQuery[0]);
	device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &g_pOcclusionQuery[1]);
	//
	for ( int i=0; i<3; i++ )
	{
		g_Models_DX9[i].ConvertToDX9Model(&g_Models[i]);
	}

	return true;
}

bool ReleaseResourceDX9(void)
{
	SAFE_RELEASE(g_pOcclusionQuery[0]);
	SAFE_RELEASE(g_pOcclusionQuery[1]);

	for ( int i=0; i<3; i++ )
	{
		g_Models_DX9[i].Release();
	}

	return true;
}

void ResizeWindowDX9(int width, int height)
{
	// Reset Device
	GutResetGraphicsDeviceDX9();
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	// `投影矩阵, 重设水平和垂直方向的视角.`
	float aspect = (float) height / (float) width;
	g_projection_matrix = GutMatrixPerspectiveRH_DirectX(90.0f, aspect, 0.1f, 100.0f);
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &g_projection_matrix);
	// `关闭光照`
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// `改变三角形正面的法线`
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

static int g_FrameCount = 0;

void RenderSolarSystemDX9(void)
{
	int index = g_FrameCount % 2;
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	LPDIRECT3DQUERY9 pQuery = g_pOcclusionQuery[index];

	// `太阳`
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &g_sun_matrix);
	g_Models_DX9[0].Render();
	// `地球`
	pQuery->Issue(D3DISSUE_BEGIN);
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &g_earth_matrix);
	g_Models_DX9[1].Render();
	pQuery->Issue(D3DISSUE_END);
	// `月亮`
	device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &g_moon_matrix);
	g_Models_DX9[2].Render();

	if ( g_FrameCount )
	{
		int num_loops = 0;
		int num_samples_passed = 0;
		// `去检查前一个画面的Occlusion Query结果`
		pQuery = g_pOcclusionQuery[(index + 1) % 2];
		while( pQuery->GetData(&num_samples_passed, 4, D3DGETDATA_FLUSH)==S_FALSE  )
		{
			// `结果可能还没出来, 要再查询一次.`
			num_loops++;
		}

		printf("Earth %s, queried %05d times\r", num_samples_passed ? "visible" : "disappear", num_loops);
	}

	g_FrameCount++;
}

// `使用Direct3D9来绘图`
void RenderFrameDX9(void)
{
	// `获得窗口大小`
	int w, h;
	GutGetWindowSize(w, h);
	// `清除画面`
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);
	// `开始下绘图指令`
	device->BeginScene(); 
	// view matrix
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 object_matrix = g_Control.GetObjectMatrix();
	g_view_matrix = object_matrix * view_matrix;
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *)&g_view_matrix);
	// projection matrix
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&g_projection_matrix);
	// render objects
	RenderSolarSystemDX9();
	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
