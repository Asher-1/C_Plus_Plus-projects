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

int g_iSelectedLight = 0;
// Ambient Light
Vector4 g_vAmbientLight(0.1f, 0.1f, 0.1f, 0.1f);
// 光源
Light_Info g_Lights[g_iNumLights];

// 对象纹理
Vector4 g_vMaterialAmbient(1.0f, 1.0f, 1.0f, 1.0f);
Vector4 g_vMaterialDiffuse(1.0f ,1.0f, 1.0f ,1.0f);
Vector4 g_vMaterialSpecular(0.0f, 0.0f, 0.0f, 0.0f);
Vector4 g_vMaterialEmissive(0.0f, 0.0f, 0.0f, 0.0f);
float	g_fMaterialShininess = 30.0f;

// 设置
bool	g_bLocalViewer = true;
bool	g_bAutoNormalize = true;
bool	g_bDiffuseReflection = true;
bool	g_bSpecularReflection = true;

//模型
int g_iNumGridVertices = 0;
int g_iNumGridIndices = 0;
int g_iNumGridTriangles = 0;
Vertex_V3N3 *g_pGridVertices = NULL;
unsigned short *g_pGridIndices = NULL;

