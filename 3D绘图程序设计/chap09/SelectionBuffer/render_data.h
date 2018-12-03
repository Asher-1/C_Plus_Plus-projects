#include "Vector4.h"
#include "Matrix4x4.h"
#include "GutUserControl.h"
#include "GutModel.h"

// 镜头位置
extern Vector4 g_eye; 
// 镜头对准的点
extern Vector4 g_lookat; 
// 镜头正上方的方向
extern Vector4 g_up; 
// 镜头转换矩阵
extern Matrix4x4 g_view_matrix;
extern Matrix4x4 g_world_matrix;
extern Matrix4x4 g_sun_matrix, g_earth_matrix, g_moon_matrix;
//
extern CGutModel g_sun_model, g_earth_model;
//
extern CGutUserControl g_Control;