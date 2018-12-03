#include "Vector4.h"
#include "render_data.h"
#include "gut.h"
#include "GutModel.h"

// 镜头位置
Vector4 g_eye(0.0f, 0.0f, 2.0f); 
// 镜头对准的点
Vector4 g_lookat(0.0f, 0.0f, 0.0f); 
// 镜头正上方的方向
Vector4 g_up(0.0f, 1.0f, 0.0f); 
// 镜头转换矩阵
Matrix4x4 g_view_matrix;
//
Matrix4x4 g_world_matrix;
//
Matrix4x4 g_sun_matrix, g_earth_matrix, g_moon_matrix;
//
CGutModel g_sun_model, g_earth_model;
//
CGutUserControl g_Control;