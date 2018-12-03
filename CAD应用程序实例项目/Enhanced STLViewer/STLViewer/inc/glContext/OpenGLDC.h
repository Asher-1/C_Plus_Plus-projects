// OpenGLDC.h: interface for the COpenGLDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGLDC_H__30E692A3_4129_11D4_B1EE_0010B539EBC0__INCLUDED_)
#define AFX_OPENGLDC_H__30E692A3_4129_11D4_B1EE_0010B539EBC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gl/gl.h"

#include "gl/glu.h"

#define GLUT_DISABLE_ATEXIT_HACK
#include <gl\glut.h>

#include "gl/glaux.h"
#pragma comment(lib, "glaux")// 必须加的声明，放置出现无法识别的外部符号
#pragma comment(lib, "legacy_stdio_definitions") // 强制兼容旧的代码，编译通过

#include "camera.h"
#include "..\inc\GeomCalc\cadbase.h"

#define BUFFER_LENGTH 64

class AFX_EXT_CLASS COpenGLDC
{
public:
	// 构造与析构函数
	COpenGLDC(HWND hWnd);
	virtual ~COpenGLDC();


private:
	// 关联窗口的句柄
	HWND	m_hWnd;
	// 渲染场境的句柄
	HGLRC	m_hRC;
	// 设备环境句柄
	HDC		m_hDC;

	COLORREF	m_clrBk;		//Background Color 窗口背景颜色
	// 非光照模式下的模型颜色
	COLORREF	m_clr;			//Polygon Color for unshading 模型颜色
	// 用于高亮度显示时的模型颜色，如拾取到一个物体时需要高亮度显示
	COLORREF	m_clrHighlight; //for highlight using  
	COLORREF	m_clrMaterial;  //for normal rendering 材料的颜色
	BOOL		m_bShading;		//use material property 是否采用着色显示
	GLfloat		m_vecLight[3];	//lighting direction 光源的方向

	//selection
	BOOL	m_bSelectionMode; // 当前是否是选择模式
	GLuint	m_selectBuff[BUFFER_LENGTH]; // 选择缓存区
public:
	GCamera		m_Camera; // 照相机，用于取景操作
	

protected:
	void ClearBkground(); // 清除背景颜色
	void OnShading(); // 光照/非光照模式设置
	

public:
	//initialize
	BOOL InitDC(); // 初始化
	void GLResize(int cx,int cy); // 对应窗口尺寸变化
	void GLSetupRC(); // 设置渲染场境

	//uMode :zero for normal rendering. non-zero for selection
	void Ready(); // 绘制前准备函数
	void Finish(); // 结束绘制函数

	/////////////////////// light & color //////////////////////
	// 光照/非光照模式设置
	void Shading(BOOL bShading);

	// 当前是否是着色模式
	BOOL IsShading();

	// 是否使用光源
	void Lighting(BOOL bLighting);
	BOOL IsLighting();

	//Light direction 设置与获取光源方向
	void SetLightDirection(float dx,float dy,float dz);
	void GetLightDirection(float& dx,float& dy,float& dz);

	//material 设置与获取材料颜色
	void SetMaterialColor(COLORREF clr);
	void GetMaterialColor(COLORREF& clr);
	
	//back ground 设置与获取背景颜色
	void SetBkColor(COLORREF rgb);
	void GetBkColor(COLORREF& rgb);
	
	//frame material 设置与获取非光照模式下的绘制颜色
	void SetColor(COLORREF rgb);
	void GetColor(COLORREF& rgb);

	//high light setting 设置与获取高亮度显示的颜色
	void SetHighlightColor(COLORREF clr);
	void GetHighlightColor(COLORREF& clr);

	// 高亮度与正常显示切换
	void Highlight(BOOL bLight = TRUE);


	/////////////////// Drawing ////////////////////

	// 绘制一个空间点
	void DrawPoint(const CPoint3D&); 

	//drawing curves
	// 绘制用户坐标系
	void DrawCoord();

    //绘制一条直线
	void DrawLine(const CPoint3D& sp,const CPoint3D& ep); 
	
	// 绘制连续折线
	void DrawPolyline(const CPoint3D* pt,int size);
	
	//drawing surface 
	//绘制一个三角面片
	void DrawTriChip(double n0,double n1,double n2,double v00,double v01,double v02,
					double v10,double v11,double v12,double v20,double v21,double v22);

	//drawing solid entities
	// 绘制一个圆球
	void DrawSphere(const CPoint3D& cen,double r,const CVector3D& vec);

	// 绘制一个圆柱
	void DrawCylinder(const CPoint3D& cen,double r,const CVector3D& h);

	// 绘制一个圆锥
	void DrawCone(const CPoint3D& cen,double r,const CVector3D& h);

	// 绘制一个圆环
	void DrawTorus(const CPoint3D& cen,const CVector3D& ax,double r_in,double r_out);

	// 绘制一个茶壶
	void DrawTeapot(const CPoint3D& cen, double size, const CVector3D& vec);

	// 绘制4面体
	void DrawTetrahedron(const CPoint3D& cen, double radius, const CVector3D& vec);

	// 绘制8面体
	void DrawOctahedron(const CPoint3D& cen, double radius, const CVector3D& vec);

	// 绘制12面体
	void DrawIcosahedron(const CPoint3D& cen, double radius, const CVector3D& vec);


	//selection Mode
	// 开始选择
	void	BeginSelection(int xPos,int yPos);

	// 结束选择，返回选择记录
	int		EndSelection(UINT* items);

	// 当前是否是选择模式
	BOOL	IsSelectionMode();

	// 用于选择模式下的对象名称操作
	void	InitNames();
	void	LoadName(UINT name);
	void	PushName(UINT name);
	void    PopName();

};

class AFX_EXT_CLASS CGLView : public CView
{
protected:
	COpenGLDC*	m_pGLDC;

protected: // create from serialization only
	CGLView();
	DECLARE_DYNCREATE(CGLView)

// Attributes
public:
	virtual void RenderScene(COpenGLDC* pDC);

	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLView();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// 获取当前模型的最大包容盒
	virtual BOOL GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1);
	
	// 缩放场景
	void	Zoom(double dScale);

	// 计算一个合适的缩放比，以将模型全部显示在场景中
	void	ZoomAll();

	// 使用典型视角来观察模型
	void	OnViewType(UINT type);

	// 按当前场景尺寸的百分比移动场景，参数dpx，dpy的范围是0-1
	void	MoveView(double dpx,double dpy);
	
};
#endif // !defined(AFX_OPENGLDC_H__30E692A3_4129_11D4_B1EE_0010B539EBC0__INCLUDED_)
