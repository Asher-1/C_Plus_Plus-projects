#include "Vector4.h"
#include "Matrix4x4.h"

struct Vertex_VT
{
	float m_Position[3]; // 顶点位置
	float m_Texcoord[2]; // 贴图坐标
};

extern int g_iWindowWidth, g_iWindowHeight;
extern float g_fOrthoWidth, g_fOrthoHeight;
extern float g_fOrthoSize;

// 常数定义
const float g_fFovW = 45.0f;
// 矩形
extern Vertex_VT g_Quad[4];
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
extern bool g_bStencil;
