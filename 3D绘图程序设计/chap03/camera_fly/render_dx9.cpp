#include "Gut.h"
#include "render_data.h"

bool InitResourceDX9(void)
{
	// 获得Direct3D 9设备
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	Matrix4x4 projection_matrix = GutMatrixPerspectiveRH_DirectX(90.0f, 1.0f, 0.1f, 100.0f);
	// 设置视角转换矩阵
	device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *) &projection_matrix);
	// 关闭光照
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 改变三角形正面的法线
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	int num_vertices = sizeof(g_pyramid_vertices)/sizeof(Vertex_VC);

	// 把RGBA的颜色数据转换成DX9 BGRA格式
	for ( int i=0; i<num_vertices; i++ )
	{
		unsigned char temp = g_pyramid_vertices[i].m_RGBA[0];
		g_pyramid_vertices[i].m_RGBA[0] = g_pyramid_vertices[i].m_RGBA[2];
		g_pyramid_vertices[i].m_RGBA[2] = temp;
	}

	num_vertices = sizeof(g_road_vertices)/sizeof(Vertex_VC);
	for ( int i=0; i<num_vertices; i++ )
	{
		unsigned char temp = g_road_vertices[i].m_RGBA[0];
		g_road_vertices[i].m_RGBA[0] = g_road_vertices[i].m_RGBA[2];
		g_road_vertices[i].m_RGBA[2] = temp;
	}

	return true;
}

bool ReleaseResourceDX9(void)
{
	return true;
}

// 使用DirectX 9来绘图
void RenderFrameDX9(void)
{
	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

	// `清除画面`
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	
	// `开始下绘图指令`
	device->BeginScene(); 

	// `设置数据格式`
	// `D3DFVF_XYZ = 使用3个浮点数来记录位置`
	// `D3DFVF_DIFFUSE = 使用32bits整数类型来记录BGRA颜色`
	device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE); 

	// `计算出一个可以转换到镜头坐标系的矩阵`
	Matrix4x4 view_matrix = GutMatrixLookAtRH(g_eye, g_lookat, g_up);
	device->SetTransform(D3DTS_VIEW, (D3DMATRIX *) &view_matrix);

	Vector4 border(-15.0f, 0.0f, -15.0f);
	Vector4 grid_position = border;

	const int grids_x = 30;
	const int grids_z = 30;

	for ( int x=0; x<grids_x; x++ )
	{
		int grid_x = x & 0x07;
		grid_position[2] = border[2];

		for ( int z=0; z<grids_z; z++ )
		{
			int grid_z = z & 0x07;
			char c = g_map[grid_x][grid_z];

			// `设置转换矩阵`
			Matrix4x4 object_matrix;
			object_matrix.Translate_Replace(grid_position);

			if ( c==0 ) // `马路`
			{
				device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &object_matrix);
				// `画出地板`
				device->DrawIndexedPrimitiveUP(
					D3DPT_TRIANGLESTRIP,
					0,
					4,
					2,
					g_road_trianglestrip_indices,
					D3DFMT_INDEX16,
					g_road_vertices,
					sizeof(Vertex_VC)
				);
			}
			else // `金字塔`
			{
				// `设置金字塔的高度`
				object_matrix.Scale_Replace(1.0f, (float) c, 1.0f);
				object_matrix[3] = grid_position;
				device->SetTransform(D3DTS_WORLD, (D3DMATRIX *) &object_matrix);
				// `画出金字塔`
				device->DrawIndexedPrimitiveUP(
					D3DPT_TRIANGLEFAN, // `指定所要画的基本图形种类`
					0, // `会使用的最小顶点编号, 事实上没太大用处.`
					5, // `顶点数组里有几个顶点`
					4, // `要画出几个基本图形`
					g_pyramid_trianglefan_indices,	// `索引数组`
					D3DFMT_INDEX16,		// `索引数组的类型`
					g_pyramid_vertices,	// `顶点数组`
					sizeof(Vertex_VC)	// `顶点数组里每个顶点的大小`
				);
			}

			grid_position[2] += 1.0f;
		}

		grid_position[0] += 1.0f;
	}

	// `声明所有的绘图指令都下完了`
	device->EndScene(); 
	
	// `把背景backbuffer的画面显示出来`
    device->Present( NULL, NULL, NULL, NULL );
}
