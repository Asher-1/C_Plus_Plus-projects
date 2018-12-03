#include "Vector4.h"

// 金字塔形的5个顶点
Vector4 g_vertices[5] = 
{
	Vector4(-1.0f, 1.0f,-1.0f),
	Vector4(-1.0f,-1.0f,-1.0f),
	Vector4( 1.0f,-1.0f,-1.0f),
	Vector4( 1.0f, 1.0f,-1.0f),
	Vector4( 0.0f, 0.0f, 1.0f),
};

// 链接出金字塔8条边线的索引值
unsigned short g_indices[16] =
{
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	0, 4,
	1, 4,
	2, 4,
	3, 4
};

// 镜头位置
Vector4 g_eye(0.0f, 3.0f, 3.0f); 
//Vector4 g_eye(0.0f, 0.0f, 10.0f); 
// 镜头对准的点
Vector4 g_lookat(0.0f, 0.0f, 0.0f); 
// 镜头正上方的方向
Vector4 g_up(0.0f, -1.0f, 1.0f); 
//Vector4 g_up(0.0f, 1.0f, 0.0f); 
