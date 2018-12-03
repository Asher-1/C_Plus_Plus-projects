#include "OpenGL\OpenGLDC.h"
#pragma once

class CTriChip
{
public:
	CTriChip();
	~CTriChip();

	CPoint3D  vex[3]; // 默认STL顶点数据 
	CVector3D normal; // 默认STL三角面法失数据

	CPoint3D  tri[3]; // 三角面片顶点数据 
	CPoint3D  fourFace[4]; //四面体四个角点数据 
};

