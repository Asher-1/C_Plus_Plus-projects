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
// 光源数据
Vector4 g_vGlobal_AmbientLight(0.1f, 0.1f, 0.1f, 1.0f);
Vector4 g_vLightDirection(0.0f, 0.0f, -1.0f, 0.0f);
Vector4 g_vLightAmbient(0.0f, 0.0f, 0.0f, 0.0f);
Vector4 g_vLightDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
Vector4 g_vLightSpecular(0.0f, 0.0f, 0.0f, 0.0f);
// 正面纹理
Vector4 g_vMaterialAmbient(1.0f, 0.0f, 0.0f, 1.0f);
Vector4 g_vMaterialDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
Vector4 g_vMaterialSpecular(0.0f, 0.0f, 0.0f, 1.0f);
Vector4 g_vMaterialEmissive(0.0f, 0.0f, 0.0f, 0.0f);
float	g_fMaterialShininess = 0.0f;
// 背面纹理
Vector4 g_vMaterialAmbient_Back(0.0f, 1.0f, 0.0f, 1.0f);
Vector4 g_vMaterialDiffuse_Back(0.0f, 1.0f, 0.0f, 1.0f);
Vector4 g_vMaterialSpecular_Back(0.0f, 0.0f, 0.0f, 1.0f);
Vector4 g_vMaterialEmissive_Back(0.0f, 0.0f, 0.0f, 0.0f);
float	g_fMaterialShininess_Back = 0.0f;

// 矩形的4个顶点
// Position, Normal
Vertex_VN g_Quad[4] =
{
	{{ -0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}},
	{{  0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}},
	{{ -0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}},
	{{  0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}}
};
