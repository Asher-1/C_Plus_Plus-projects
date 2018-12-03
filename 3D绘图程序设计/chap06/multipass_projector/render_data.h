#include "Vector4.h"
#include "Matrix4x4.h"
#include "GutModel.h"

struct Vertex_VT
{
	float m_Position[3]; // 顶点位置
	float m_Texcoord[2]; // 贴图坐标
};

struct Light
{
	Vector4 m_Position;
	Vector4 m_LookAt;
	Vector4 m_Diffuse;
	Vector4 m_Specular;
};

extern Light g_Light;
extern Vector4 g_vAmbientLight;

extern bool g_bPass0;
extern bool g_bPass1;

// 常数定义
const float g_fFovW = 45.0f;
// 矩形
extern Vertex_VT g_Quad[4];
extern Vertex_VT g_FullScreenQuad[4];
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
//
extern int g_iMode;
//
extern CGutModel g_Model;