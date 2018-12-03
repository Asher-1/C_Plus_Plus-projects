#include "Vector4.h"

struct Vertex_VC
{
	float m_Position[3];
	// R,G,B,A in OpenGL
	// B,G,R,A in Direct3D 9
	// Direct3D 10可是是两种其中之1, 看设置而定.
	unsigned char m_RGBA[4];
};

// 金字塔形的8条边线
extern Vertex_VC g_vertices[5];
extern unsigned short g_indices[18];

// 镜头位置
extern Vector4 g_eye; 
// 镜头对准的点
extern Vector4 g_lookat; 
// 镜头正上方的方向
extern Vector4 g_up; 
