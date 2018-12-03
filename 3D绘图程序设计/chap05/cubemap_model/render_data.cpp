#include "Vector4.h"
#include "render_data.h"
#include "gut.h"
#include "GutModel.h"

// 镜头位置
Vector4 g_eye(0.0f, 0.0f, 30.0f); 
// 镜头对准的点
Vector4 g_lookat(0.0f, 0.0f, 0.0f); 
// 镜头正上方的方向
Vector4 g_up(0.0f, 1.0f, 0.0f); 
// 镜头转换矩阵
Matrix4x4 g_view_matrix;
// 对象旋转矩阵
Matrix4x4 g_world_matrix;
// 读取模型的对象
CGutModel g_Model;

// 转换顶点数据的函数, Direct3D和OpenGL可以共用.
void CopyVertexBuffer(void *target, sModelVertex *source, int num_vertices)
{
	// 这个范例中, OpenGL和Direct3D运行时只需要Position & Normal
	struct sRunTimeVertex
	{
		float m_Position[3];
		float m_Normal[3];
	};

	sRunTimeVertex *pVertexArray = (sRunTimeVertex *) target;

	for ( int v=0; v<num_vertices; v++ )
	{
		sRunTimeVertex *p = pVertexArray + v;

		p->m_Position[0] = source[v].m_Position[0];
		p->m_Position[1] = source[v].m_Position[1];
		p->m_Position[2] = source[v].m_Position[2];

		p->m_Normal[0] = source[v].m_Normal[0];
		p->m_Normal[1] = source[v].m_Normal[1];
		p->m_Normal[2] = source[v].m_Normal[2];
	}
}
