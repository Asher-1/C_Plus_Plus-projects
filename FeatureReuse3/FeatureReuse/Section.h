#pragma once
#include "stdafx.h"
#include <vector>
#include "OpenGL\OpenGLDC.h"

class Section
{
public:
	Section();
	~Section();

	bool isModified;
	vector<int> Index; // 用于存储每个定点的拓扑关系（索引号）
	vector<CPoint2D> point_2D; // 工件平面坐标点
	vector<CPoint2D> speed_2D;  // 平面速度矢
	vector<double> strain;				// 应变
	vector<double> stress;				// 应力
};

