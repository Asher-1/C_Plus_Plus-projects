#include "Vector4.h"
#include "render_data.h"
#include "gut.h"

// controller object
CGutUserControl g_Control;
// 矩形的4个顶点
// Position
Vertex g_Quad[4] =
{
	{-2.0f, -2.0f, 0.0f},
	{ 2.0f, -2.0f, 0.0f},
	{-2.0f,  2.0f, 0.0f},
	{ 2.0f,  2.0f, 0.0f}
};
