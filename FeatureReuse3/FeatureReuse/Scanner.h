#pragma once
#include <vector>
using namespace std;
class  COpenGLDC;

class Scanner
{
public:
	Scanner();
	~Scanner();

	double y;
	vector<double> rank; // 颜色等级
	vector<double> x_Set; // 扫描线与等值线交点的横坐标

private:

};


