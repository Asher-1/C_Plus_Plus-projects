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
const int	g_iMaxNumGrids = 100;
const int	g_iMaxNumGridVertices = (g_iMaxNumGrids+1) * (g_iMaxNumGrids+1);
const int	g_iMaxNumGridIndices = g_iMaxNumGrids * (g_iMaxNumGrids * 2 + 2);

// 格子模型数据
extern int	g_iNumGridVertices;
extern int	g_iNumGridIndices;
extern int	g_iNumGridTriangles;
extern Vertex_VCN *g_pGridVertices;
extern Vertex_DX9 *g_pGridVerticesDX9;
extern unsigned short *g_pGridIndices;

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
extern Vector4 g_vLightPosition;
extern Vector4 g_vLightColor;
extern Vector4 g_vLightAttenuation;
extern float   g_fSpotLightCutoff;
extern float   g_fSpotLightExponent;

//
void ConvertToDX9(Vertex_VCN g_quad_in[], Vertex_DX9 g_quad_out[], int num_vertices);
// 生成格子的函数
bool GenerateGrids(int x_grids, int y_grids, 
				   Vertex_VCN **ppVertices, int &num_vertices, 
				   unsigned short **ppIndices, int &num_indices,
				   int &num_triangles);
// 释放格子模型资源
bool ReleaseGridsResource(Vertex_VCN **ppVertices, unsigned short **ppIndices);
// 计算光照的函数
void CalculateAmbientLight(Vertex_VCN *pVertices, int num_vertices);
void CalculateDirectionalLight(Vertex_VCN *pVertices, int num_vertices);
void CalculatePointLight(Vertex_VCN *pVertices, int num_vertices);
void CalculateSpotLight(Vertex_VCN *pVertices, int num_vertices);
