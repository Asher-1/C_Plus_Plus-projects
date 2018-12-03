#include "Vector4.h"
#include "Matrix4x4.h"
#include "GutUserControl.h"

struct Vertex_VN
{
	float m_Position[3]; // 顶点位置
	float m_Normal[3];
};

// 常数定义
const float g_fFovW = 45.0f;
// controller object
extern CGutUserControl g_Control;

extern Vertex_VN *g_pSphereVertices;
extern unsigned short *g_pSphereIndices;

extern int g_iNumSphereVertices;
extern int g_iNumSphereTriangles;
extern int g_iNumSphereIndices;

// 生成球模型的程序
bool CreateSphere(float radius, Vertex_VN **ppVertices, unsigned short **ppIndices, int stacks = 20, int pieces = 20);
