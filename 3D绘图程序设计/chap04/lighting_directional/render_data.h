#include "Vector4.h"
#include "Matrix4x4.h"

struct Vertex_VCN
{
	Vector4 m_Position; // 顶点位置
	Vector4 m_Color;	// 顶点颜色
	Vector4 m_Normal;	// 顶点法线
};

struct Vertex_DX9
{
	float m_Position[3];	// 顶点位置
	unsigned char m_Color[4]; // 顶点颜色
};

// 常数定义
const float g_fFovW = 45.0f;
extern Vertex_VCN g_Quad[4];
extern Vertex_DX9 g_Quad_dx9[4];

// 镜头位置
extern Vector4 g_eye; 
// 镜头对准的点
extern Vector4 g_lookat; 
// 镜头正上方的方向
extern Vector4 g_up; 
// 镜头转换矩阵
extern Matrix4x4 g_view_matrix;
// 对象旋转矩阵
extern Matrix4x4 g_world_matrix;
// 光源数据
extern Vector4 g_vLightAmbient;
extern Vector4 g_vLightDirection;
extern Vector4 g_vLightColor;

// 计算光照的函数
void CalculateAmbientLight(Vertex_VCN *pVertices, int num_vertices);
void CalculateDirectionalLight(Vertex_VCN *pVertices, int num_vertices);

void ConvertToDX9(Vertex_VCN g_quad_in[], Vertex_DX9 g_quad_out[], int num_vertices);
