#include "Vector4.h"
#include "render_data.h"
#include "gut.h"
#include "GutModel.h"

// 画cubemap 6个面时的镜头位置和法线

Vector4 g_vCubemap_Eye(0.0f, 0.0f, 0.0f);

Vector4 g_vCubemap_Up[6] =
{
	Vector4(0.0f, 1.0f, 0.0f), // +x
	Vector4(0.0f, 1.0f, 0.0f), // -x

	Vector4(0.0f, 0.0f,-1.0f), // +y
	Vector4(0.0f, 0.0f, 1.0f), // -y

	Vector4(0.0f, 1.0f, 0.0f), // +z
	Vector4(0.0f, 1.0f, 0.0f) // -z
};

Vector4 g_vCubemap_Lookat[6] =
{
	Vector4(1.0f, 0.0f, 0.0f), // +x
	Vector4(-1.0f, 0.0f, 0.0f), // -x

	Vector4(0.0f, 1.0f, 0.0f), // +y
	Vector4(0.0f,-1.0f, 0.0f), // -y

	Vector4(0.0f, 0.0f, 1.0f), // +z
	Vector4(0.0f, 0.0f,-1.0f) // -z
};

// 镜头位置
Vector4 g_eye(0.0f, 0.0f, 15.0f, 1.0f); 
// 镜头对准的点
Vector4 g_lookat(0.0f, 0.0f, 0.0f, 1.0f); 
// 镜头正上方的方向
Vector4 g_up(0.0f, 1.0f, 0.0f, 1.0f); 
// 镜头转换矩阵
Matrix4x4 g_view_matrix;
//
Matrix4x4 g_world_matrix;
Matrix4x4 g_scale_matrix;
//
Matrix4x4 g_sun_matrix, g_earth_matrix, g_moon_matrix, g_mars_matrix;

// 镜头视向
float g_fFOV = 45.0f;
// 模型
CGutModel g_Models[4];
// controller object
CGutUserControl g_Control;

