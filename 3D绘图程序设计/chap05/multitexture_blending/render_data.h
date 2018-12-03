#include "Vector4.h"
#include "Matrix4x4.h"
#include "GutUserControl.h"

struct Vertex_VT
{
	float m_Position[3]; // 顶点位置
	float m_Texcoord[2]; // 贴图坐标
};

// 常数定义
const float g_fFovW = 45.0f;
// 矩形
extern Vertex_VT g_Quad[4];
// controller object
extern CGutUserControl g_Control;
//
extern int g_iMode;
