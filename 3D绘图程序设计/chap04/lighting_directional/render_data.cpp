#include "Vector4.h"
#include "render_data.h"
#include "gut.h"

// 镜头位置
Vector4 g_eye(0.0f, 0.0f, 2.0f); 
// 镜头对准的点
Vector4 g_lookat(0.0f, 0.0f, 0.0f); 
// 镜头正上方的方向
Vector4 g_up(0.0f, 1.0f, 0.0f); 
// 镜头转换矩阵
Matrix4x4 g_view_matrix;
// 对象旋转矩阵
Matrix4x4 g_world_matrix;
// `光源数据`
Vector4 g_vLightAmbient(0.1f, 0.1f, 0.1f, 0.1f);
Vector4 g_vLightDirection(0.0f, 0.0f, 1.0f, 0.0f);
Vector4 g_vLightColor(1.0f, 1.0f, 1.0f, 1.0f);
// 矩形的4个顶点
// Position, Color, Normal
Vertex_VCN g_Quad[4] =
{
	{ Vector4(-0.5f, -0.5f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ Vector4( 0.5f, -0.5f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ Vector4(-0.5f,  0.5f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ Vector4( 0.5f,  0.5f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) }
};

Vertex_DX9 g_Quad_dx9[4];

// `把颜色初值设置成环境光`
void CalculateAmbientLight(Vertex_VCN *pVertices, int num_vertices)
{
	for ( int i=0; i<num_vertices; i++ )
	{
		pVertices[i].m_Color = g_vLightAmbient;
	}
}

// `计算方向光, 它只与顶点法线和光源方向有关.`
void CalculateDirectionalLight(Vertex_VCN *pVertices, int num_vertices)
{
	for ( int i=0; i<num_vertices; i++ )
	{
		// `求出转换后在世界坐标系的顶点法线, RotateVector函数只做旋转, 忽略位移.`
		Vector4 vNormal = g_world_matrix.RotateVector(pVertices[i].m_Normal);
		// `顶点法线和光线方向的夹角, 决定反射光的强度.`
		Vector4 vIntensity = Vector3Dot(vNormal, g_vLightDirection);
		// `把intensity局限在0-1的范围`
		vIntensity.Clamp_to_0();
		// `累加上计算出来方向光的强度`
		pVertices[i].m_Color += vIntensity * g_vLightColor;
		pVertices[i].m_Color = pVertices[i].m_Color.Clamp_to_1();
	}
}

void ConvertToDX9(Vertex_VCN g_quad_in[], Vertex_DX9 g_quad_out[], int num_vertices)
{
	for ( int i=0; i<num_vertices; i++ )
	{
		g_quad_in[i].m_Position.StoreXYZ(g_quad_out[i].m_Position);
		GutConvertColor(g_quad_in[i].m_Color, g_quad_out[i].m_Color);
	}
}
