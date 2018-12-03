#include "Vector4.h"
#include "Matrix4x4.h"
#include "GutModel.h"

// 常数定义
const float g_fFovW = 45.0f;
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
extern CGutModel g_Model;

void CopyVertexBuffer(void *target, sModelVertex *source, int num_vertices);
