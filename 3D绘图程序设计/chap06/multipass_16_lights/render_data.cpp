#include "Vector4.h"
#include "render_data.h"
#include "gut.h"

// controller object
CGutUserControl g_Control;
// 矩形的4个顶点
// Position, Texcoord
Vertex_VT g_Quad[4] =
{
	{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
	{{ 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
	{{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}}
};

Vertex_VT g_FullScreenQuad[4] =
{
	{{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f}},
	{{ 1.0f, -1.0f, 1.0f}, {1.0f, 1.0f}},
	{{-1.0f,  1.0f, 1.0f}, {0.0f, 0.0f}},
	{{ 1.0f,  1.0f, 1.0f}, {1.0f, 0.0f}}
};

int g_iSwitch = 0x03;

Light g_Lights[NUM_LIGHTS];
Light g_Light;

Vector4 g_vAmbientLight(0.3f, 0.3f, 0.3f, 1.0f);


// 读取模型的对象
CGutModel g_Model;
