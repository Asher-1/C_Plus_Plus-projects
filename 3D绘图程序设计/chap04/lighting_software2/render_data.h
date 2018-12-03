#include "Vector4.h"
#include "Matrix4x4.h"
#include "GutDefs.h"

struct Vertex_VCN
{
	Vector4 m_Position; // 顶点位置
	Vector4 m_Color;	// 顶点颜色
	Vector4 m_Normal;	// 顶点法线
};

struct Vertex_DX9
{
	float m_Position[3];
	unsigned char m_Color[4];
};

struct Light_Info
{
	Vector4 m_vPosition;
	Vector4 m_vDirection;

	Vector4 m_vAmbientColor;
	Vector4 m_vDiffuseColor;
	Vector4 m_vSpecularColor;
	Vector4 m_vAttenuation;

	float	m_fSpotLightInnerCone;
	union
	{
		float	m_fSpotlightCutoff;
		float	m_fSpotlightHalfCutoffCosine;
	};
	float	m_fSpotlightExponent;
	
	int		m_eType;
	bool	m_bEnabled;

	Light_Info(void)
	{
		m_vPosition = Vector4::GetZero();
		m_vDirection.Set(0.0f, 0.0f, -1.0f, 0.0f);
		
		m_vAmbientColor.Set (0.0f, 0.0f, 0.0f, 1.0f);
		m_vDiffuseColor.Set (1.0f, 1.0f, 1.0f, 1.0f);
		m_vSpecularColor.Set(0.0f, 0.0f, 0.0f, 1.0f);

		m_vAttenuation.Set(1.0f, 0.0f, 0.0f);

		m_fSpotLightInnerCone = 0.0f;
		m_fSpotlightCutoff = 180.0f;
		m_fSpotlightExponent = 1.0f;

		m_eType = LIGHT_DIRECTIONAL;
		m_bEnabled = false;
	}
};

const int g_iNumMaxLights = 8;

struct GraphicsDevice_Simulation
{
	Light_Info m_Lights[g_iNumMaxLights];
	Vector4 m_vAmbientLight;
	
	Vector4 m_vMaterialAmbient;
	Vector4 m_vMaterialDiffuse;
	Vector4 m_vMaterialSpecular;
	Vector4 m_vMaterialEmissive;

	Matrix4x4 m_ViewMatrix;
	Matrix4x4 m_WorldMatrix;
	Matrix4x4 m_WorldViewMatrix;

	float m_fMaterialShininess;
	bool  m_bLocalViewer;
	bool  m_bPhongSpecular;
	bool  m_bAutoNormalize;

	void CalculateLighting(Vertex_VCN *pVertices, int num_vertices);
	void SetViewMatrix(Matrix4x4 &matrix);
	void SetWorldMatrix(Matrix4x4 &matrix);
	void UpdateMatrix(void) { m_WorldViewMatrix = m_WorldMatrix * m_ViewMatrix; }
};

extern GraphicsDevice_Simulation g_Simulation;

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

// 生成格子的函数
bool GenerateGrids(int x_grids, int y_grids, 
				   Vertex_VCN **ppVertices, int &num_vertices, 
				   unsigned short **ppIndices, int &num_indices,
				   int &num_triangles);
// 释放格子模型资源
bool ReleaseGridsResource(Vertex_VCN **ppVertices, unsigned short **ppIndices);
//
void ConvertToDX9(Vertex_VCN g_vertices_in[], Vertex_DX9 g_vertices_out[], int num_vertices);
