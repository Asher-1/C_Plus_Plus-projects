#include "Vector4.h"
#include "Matrix4x4.h"

struct Vertex_VN
{
	float m_Position[3];	// 顶点位置
	float m_Normal[3];		// 顶点法线
};

// 常数定义
const float g_fFovW = 45.0f;
extern Vertex_VN g_Quad[4];

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
extern Vector4 g_vGlobal_AmbientLight;
extern Vector4 g_vLightAmbient;
extern Vector4 g_vLightDirection;
extern Vector4 g_vLightDiffuse;
extern Vector4 g_vLightSpecular;
// 正面纹理
extern Vector4 g_vMaterialAmbient;
extern Vector4 g_vMaterialDiffuse;
extern Vector4 g_vMaterialSpecular;
extern Vector4 g_vMaterialEmissive;
extern float   g_fMaterialShininess;
// 背面纹理
extern Vector4 g_vMaterialAmbient_Back;
extern Vector4 g_vMaterialDiffuse_Back;
extern Vector4 g_vMaterialSpecular_Back;
extern Vector4 g_vMaterialEmissive_Back;
extern float   g_fMaterialShininess_Back;
