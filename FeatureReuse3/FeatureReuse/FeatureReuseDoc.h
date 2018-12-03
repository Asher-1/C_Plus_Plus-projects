
// FeatureReuseDoc.h : CFeatureReuseDoc 类的接口
//

#pragma once
#include "TriMesh\include\TriMesh.h"
#include "OpenGL\OpenGLDC.h"
#include <malloc.h>    // 解决 malloc.h 与 crtdbg.h 顺序导致的 Debug Assertion Failed, "Corrupted pointer passed to _freea" 。
#include <crtdbg.h>

#include "TriChip.h"
#include "tetgen.h"
#include "Section.h"


class CFeatureReuseDoc : public CDocument
{
protected: // 仅从序列化创建
	CFeatureReuseDoc();
	DECLARE_DYNCREATE(CFeatureReuseDoc)

// 属性
public:
	vector<TriMesh*>		meshes;
	int								Current_mesh;					//当前的网格模型

// 属性
public:
	TriMesh *mesh;
	CTriChip tem_tri;  // 用于暂时指向CTriChip对象

	tetgenio v_tetIN;
	tetgenio v_tetOUT;

	vector<CPoint3D> vertices;
	vector<CPoint3D>  faces;

	// .node文件
	vector<CTriChip> temp_vertex; // 用于顶点数据缓存容器，在析构函数中对其内的对象一一析构
	vector<CTriChip> m_TriList;

	vector<CPoint3D> node; // 结点数据
	// .face文件
	vector<CPoint3D> triface; // 三角面数据
	vector<int> trifacemarker; // 三角面标记
	//.ele文件 
	vector<int> tetrahedronattribute; // 四面体属性
	vector<CPoint4D> tetrahedron; // 用于存储四面体四个角点的索引数据
	 // .edge文件
	vector<int> edge; // 四面体的边
	vector<int> edgemarker; // 四面体的边标记


	GLfloat m_xAngle;
	GLfloat m_yAngle; // 声明旋转角度
	CPoint m_MouseDownPoint;
	float m_Scale;

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	virtual BOOL				OnOpenDocument(LPCTSTR lpszPathName);
	bool								IsTextFile(const char * strFileName);					 //判断一个文件是否为文本文件
	void								split(char **arr, char *str, const char *del);		//字符分割函数的简单定义和实现;
	void								DrawModel(COpenGLDC* pDC);							// 绘制模型函数

	TriMesh::BBox			getbox();																		   // 获取包容盒

// 实现
public:


	bool inputStl_to_Tetgen(); // 为tetgen库传入STL模型的剖分参数

	bool Dissection(); // 剖分函数

	bool Input_to_CTriChip(); // 剖分数据 导入CTriChip对象

	bool Load_Stl_Points(LPCTSTR m_stlfile, TriMesh *themesh, int fn);// 自定义读取stl数据点函数

	void SetMode(int mode); // 设置绘制模式
	void SetArrows(bool b);  // 设置矢量箭头

	void Set_cloudPicture(bool b); // 设置彩色云图状态

	void SetScale(float scale); // 动态设置缩放比例函数
	void SetMouseRotate(GLfloat xAngle, GLfloat yAngle); // 动态设置旋转角度

	virtual ~CFeatureReuseDoc();

public:
	bool read_txt(string f); // 后处理数据文本读取函数

	int m_Mode; // 绘制模式状态变量，可设置为0，1，2, 3
	bool is_PrintArrows; // 是否显示矢量箭头
	bool is_Draw_cloudPicture; // 是否显示彩色云图
	int stage;  // 工件受力变形阶段
	int counts;
	int delta; // 动态显示模式的循环增量

	static int steps; // 工件受力变形阶段总数
	vector<Section> sections; // 每个阶段的模型状态集合
	vector<int> num_faces;
	void SetStage(int m_stage); // 设置工件模型显示阶段
	void SetDelta(int m_delta); // 设置工件模型动态显示循环变量
	void SetCounter(int counter); // 设置动态显示时的计时器




	typedef struct 
	{
		double x, y; // 点坐标
		double strain; // 点高程值1
		double stress; // 点高程值2
		double index; // 点索引号
	}DPoint;  // 自定义二维点结构体

	typedef struct
	{
		DPoint point[3];  // 用于保存三角面片的三个点的信息
		bool is_used = false; // 是否计算过等值点
		bool is_Original = false; // 等值线的起始三角面片标记
		bool is_edge = false; // 边界三角面片标志
		bool flag1 = false; // 用于标记三角面片1，2点所在边是否为边界边
		bool flag2 = false; // 用于标记三角面片2，3点所在边是否为边界边
		bool flag3 = false; // 用于标记三角面片3，1点所在边是否为边界边
	}Trigon; // 三角面片结构体

	typedef struct
	{
		double x, y;
		double H; // 高程值
		bool is_begin = false;
		bool is_end = false;
		vector<CPoint2D> m_lines; // 线头和线尾所在的边界边的两个端点

	}Dcontour;  // 等值线节点结构体


	//typedef struct
	//{
	//	double y;
	//	vector<double> rank; // 颜色等级
	//	vector<double> x_Set; // 扫描线与等值线交点的横坐标
	//}Scanner;  // 扫描线结构体


	int Ncolor;      // 颜色级数
	vector<int> Dc;
	vector<double>  delta_Set;
	vector<double>  Max_x, Min_x, Max_y, Min_y;							// 所有阶段的工件的左下角坐标和右上角坐标
	vector<double>  stress_Max, stress_Min;										// 所有阶段的工件的最大高程值和最小高程值 
	
	vector<vector<Scanner>> Scanner_TotalSet;								// 所有阶段的扫描线集合

	int findNumOfTrigon_Edge(vector<Trigon> vec);						// 寻找边缘三角面的个数

																		
	bool CFeatureReuseDoc::isAllUsed(vector<Trigon> vec); // 判断三角面集合vec所有的三角面是否都已经用过了

	bool CFeatureReuseDoc::findSame(vector<Dcontour> Dcontour_Set, Trigon rigon); // 寻找是否有相同的三角面片

	vector<vector<double>> stress_TotalSet;										// 所有阶段的高程值集合

	vector<vector<vector<Dcontour>>> Dcontour_TotalSet;		// 所有阶段的等值线集合

	vector<vector<Trigon>>  ContourTriArr;											// 含有等值线的三角面片集合

	vector<vector<Trigon>> Trigon_Edge_Set;									// 含有边缘三角面片的集合

	vector<vector<CPoint2D>>  BorderLine_Set;								// 用于保存所有阶段的边缘曲线

	vector<vector<vector<CPoint3D>>>  NonClosedRegion_Set;				// 用于保存所有阶段的开曲线的封闭区域
	vector<vector<vector<CPoint3D>>>  ClosedRegion_Set;						// 用于保存所有阶段的闭曲线的封闭区域

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void SetTitle(LPCTSTR lpszTitle);
};