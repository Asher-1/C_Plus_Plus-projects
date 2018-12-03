#include "Vector4.h"
#include "render_data.h"

// 金字塔形的5个顶点位置及颜色的数据
Vertex_VC g_vertices[5] = 
{
	{
		{0.0f, 0.0f, 2.0f}, // top
		{255, 0, 0, 255},
	},
	{
		{-1.0f,-1.0f, 0.0f},
		{0, 255, 0, 255},
	},
	{
		{1.0f,-1.0f, 0.0f},
		{0, 0, 255, 255},
	},
	{
		{1.0f, 1.0f, 0.0f},
		{255, 255, 0, 255},
	},
	{
		{-1.0f, 1.0f, 0.0f},
		{0, 255, 255, 255},
	},
};

// 组成金字塔的6个三角形的顶点索引值
unsigned short g_indices[18] =
{
	0, 1, 2, // triangle 1　
	0, 2, 3, // triangle 2
	0, 3, 4, // triangle 3
	0, 4, 1, // triangle 4
	1, 4, 2, // triangle 5
	2, 4, 3, // triangle 6
};

// 镜头位置
Vector4 g_eye(0.0f, 4.0f, 1.0f); 
// 镜头对准的点
Vector4 g_lookat(0.0f, 0.0f, 1.0f); 
// 镜头正上方的方向
Vector4 g_up(0.0f, 0.0f, 1.0f); 
