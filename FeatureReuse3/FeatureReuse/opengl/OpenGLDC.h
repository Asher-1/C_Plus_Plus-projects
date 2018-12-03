// OpenGLDC.h: interface for the COpenGLDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGLDC_H__30E692A3_4129_11D4_B1EE_0010B539EBC0__INCLUDED_)
#define AFX_OPENGLDC_H__30E692A3_4129_11D4_B1EE_0010B539EBC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "\ShoePatternDesign\ShoePatternDesign\stdafx.h"
#pragma once
#include "..\stdafx.h"
#include "gl.h"
#include "glu.h"
#include "glaux.h"
#pragma comment(lib, "glaux")// 必须加的声明，防止出现无法识别的外部符号
#pragma comment(lib, "legacy_stdio_definitions") // 强制兼容旧的代码，编译通过

#include "glut.h"
#include <vector>
#include "camera.h"
#include "cadbase.h"
#include "../Scanner.h"

using namespace std;

#define BUFFER_LENGTH 64

#define ROTATE_ANGLE			-0.08726646260
#define ROTATE_ANGLE_NEG		0.08726646260
class Scanner;
class  COpenGLDC
{
public:
	COpenGLDC(HWND hWnd);
	virtual ~COpenGLDC();

private:
	HWND				m_hWnd;
	HGLRC			m_hRC;
	HDC					m_hDC;

	COLORREF	m_clrBk;		//Background Color
	COLORREF	m_clr;			//Polygon Color for unshading
	COLORREF	m_clrHighlight; //for highlight using
	COLORREF	m_clrMaterial;  //for normal rendering
	COLORREF	m_clrBkMaterial;  //for normal rendering
	BOOL				m_bShading;		//use material property
   //GLfloat			m_vecLight[3][3];	//lighting direction
	GLfloat				m_vecLight[3];
	BOOL				m_NormolViewMode;  //正常显示还是同时显示着色和线框模式

	//selection
	BOOL				m_bSelectionMode;
	GLuint				m_selectBuff[BUFFER_LENGTH];

	GLuint				texture[1];		// 存储一个纹理

public:
	GCamera			m_Camera;

	////新添加+++++++++>>2011.6.10
	//GLuint				texture[1];		// 存储一个纹理
	//CBMPLoader       texture;
     GLuint        texture_id;
	 BOOL    BuildTexture(char *szPathName, GLuint &texid);

protected:
	void					ClearBkground();
	void					OnShading();

public:
	//initialize
	BOOL				InitDC();
	void					GLResize(int cx,int cy);
	void					GLSetupRC();

	//uMode :zero for normal rendering. non-zero for selection
	void					Ready();
	void					Finish();

	/////////////////////// light & color //////////////////////
	void					Shading(BOOL bShading);
	BOOL				IsShading();

	void					Lighting(BOOL bLighting);
	BOOL				IsLighting();

	void					ViewMode();

	//Light direction
	void					SetLightDirection(float dx,float dy,float dz);
	void					GetLightDirection(float& dx,float& dy,float& dz);
			//Light direction
//	void SetLightDirection(float dx,float dy,float dz, short sn);
//	void GetLightDirection(float& dx,float& dy,float& dz, short sn);

	//material
	void					SetMaterialColor(COLORREF clr);
	void					GetMaterialColor(COLORREF& clr);

	void					SetBkMaterialColor(COLORREF clr);
	void					GetBkMaterialColor(COLORREF& clr);
	
	//back ground
	void					SetBkColor(COLORREF rgb);
	void					GetBkColor(COLORREF& rgb);
	
	//frame material
	void					SetColor(COLORREF rgb);
	void					GetColor(COLORREF& rgb);

	//high light setting
	void					SetHighlightColor(COLORREF clr);
	void					GetHighlightColor(COLORREF& clr);
	void					Highlight(BOOL bLight = TRUE);




	/////////////////// Drawing ////////////////////
	void					DrawPoint(const CPoint3D&);
	void					DrawPoint(const CPoint3D& pt, COLORREF ctr);
	void					DrawJuxing(const CPoint3D& p1,const CPoint3D& p2,const CPoint3D& p3,const CPoint3D& p4);

	//drawing curves
	void					DrawCoord();

	void					DrawLine(const CPoint3D& sp,const CPoint3D& ep);
	void					DrawPolyline(const CPoint3D* pt,int size);
	void					DrawPolyline(const vector<CPoint3D> pts, int R, int G, int B);
	
	// 自定义绘制方式
	void					DrawArrows(double x0, double y0, double Vx, double Vy);
	void					DrawSingleModel(double x1, double y1, double x2, double y2, double x3, double y3);

	void					DrawArrowsModel(double x1, double y1, double x2, double y2, double x3, double y3,
								double Vx1, double Vy1, double Vx2, double Vy2, double Vx3, double Vy3);
	void					DrawSingleModel(double x1, double y1, double x2, double y2, double x3, double y3,
								double Vx1, double Vy1, double Vx2, double Vy2, double Vx3, double Vy3);
	void					DrawSingleModel(double x1, double y1, double x2, double y2, double x3, double y3, 
								double Vx1, double Vy1, double Vx2, double Vy2, double Vx3, double Vy3, 
								double strain1,double stress1, double strain2, double stress2, double strain3, double stress3);

	//drawing surface
	void					DrawTriChip(vector<CPoint3D> &dingdian, vector<CPoint3D> &dingdianfashi);
	void					DrawTriChip(vector<CPoint3D> &dingdian, vector<CPoint3D> &dingdianfashi,vector< vector<float> > &vcolors);

	void					DrawTriChip(double n0,double n1,double n2,double v00,double v01,double v02,
					double v10,double v11,double v12,double v20,double v21,double v22,vector< vector<float> > &vcolors);

	void					DrawTriChip(double n0,double n1,double n2,double v00,double v01,double v02,
		double v10,double v11,double v12,double v20,double v21,double v22,bool beselect);

	// 原型绘制
	void DrawTriChip(double n0, double n1, double n2, double v00, double v01, double v02,
		double v10, double v11, double v12, double v20, double v21, double v22);

	// 三角面绘制
	void DrawTriChip(double v00, double v01, double v02,
		double v10, double v11, double v12, double v20, double v21, double v22);

	// 四面体绘制
	void COpenGLDC::DrawTriChip(double v00, double v01, double v02, double v10, double v11,
		double v12, double v20, double v21, double v22, double v30, double v31, double v32, boolean b1, boolean b2);



	void					DrawTriChipwithtecture(double n0,double n1,double n2,double v00,double v01,double v02,
		double v10,double v11,double v12,double v20,double v21,double v22);


	void					DrawQuads(vector<CPoint3D> waist1, vector<CPoint3D> waist2, vector<CVector3D> norm);

	//drawing solid entities
	void					DrawSphere(const CPoint3D& cen,double r);
	void					DrawSphere(const CPoint3D& cen,double r,const CVector3D& vec);
	void					DrawCylinder(const CPoint3D& cen,double r,const CVector3D& h);
	void					DrawCylinder(const vector<CPoint3D> pdata, double r);
	void					DrawCone(const CPoint3D& cen,double r,const CVector3D& h);
	void					DrawTorus(const CPoint3D& cen,const CVector3D& ax,double r_in,double r_out);

	//selection Mode
	void					BeginSelection(int xPos,int yPos);
	int					EndSelection(UINT* items);
	BOOL				IsSelectionMode();

	void					InitNames();
	void					LoadName(UINT name);
	void					PushName(UINT name);
	void					PopName();

	void					CreateLight();
	//旋转视图
	void					RotateView(double dRotX,double dRotY,double dRotZ);
	void					RotateView(double dRot,VECTOR3D vect);
	void					RotateView(double dRot,short snXyz);
	//
	void					TransWorldtoScreen();
	void					EndTransWorldtoScreen();
	void					DrawText(char* string, int flag=12);

	// 构建颜色填充表函数
	double *r, *g, *b;
	void TColor(int m);


	//typedef struct
	//{
	//	double y;
	//	vector<double> rank; // 颜色等级
	//	vector<double> x_Set; // 扫描线与等值线交点的横坐标
	//}Scanner;  // 扫描线结构体


	void COpenGLDC::DrawNonClosedRegion(vector<CPoint3D> p_set);
	void COpenGLDC::DrawNonClosedRegion(CPoint3D  p1, CPoint3D  p2);
	// 绘制边缘线
	void COpenGLDC::DrawEdge(CPoint2D p1, CPoint2D p2);

	// 绘制彩色云图函数
	void					DrawCloudPicture(Scanner scanner);
	// 绘制等值线
	void					DrawCloudPicture(double p1x, double p1y, double p2x, double p2y);

	// 指定文本在窗口中的输出信息及位置
	void					gltRasterText(double x, double y, const char *text, void *font);

	// 绘制色码标识
	void					DrawCodeIdentifier(double x, double y, int Ncolor, int column, int Dc, int min);

	// 循环递归调用四叉树算法填充网格矩形颜色值
	void					TreeMath(double x, double y, double dx, double dy, double z1, double z2, double z3, double z4, double min, int Dc);

	void					FinshForEps();//仅应用于Eps输出


	////新添加+++++++++>>2011.6.10
	AUX_RGBImageRec *LoadBMP(CHAR *Filename)	;			// 载入位图图象
	BOOL               LoadTexture(char *file); 
	void                  DrawTexture();
	void                  OneNeighborTri();
	void                  DrawTexture(CPoint2D CoordUV1,CPoint2D CoordUV2,CPoint2D CoordUV3, CPoint3D FaceNormal,
		CPoint3D MeshVertice1,CPoint3D MeshVertice2,CPoint3D MeshVertice3);
	void                  DrawTriChipG(double n0,double n1,double n2,double v00,double v01,double v02,
		double v10,double v11,double v12,double v20,double v21,double v22,bool beselect);
	void                 DrawSphereb(const CPoint3D& cen,double r);   //蓝色（blue）小球
	void                 DrawSpherep(const CPoint3D& cen,double r);   //粉色（pink）小球
    void                 DrawLine(const CPoint3D& sp,const CPoint3D& ep,int R,int G,int B);
};

class  CGLView : public CView
{
protected:
	COpenGLDC*	m_pGLDC;
	
	CPoint				m_pntMouseMove;
	CPoint				m_pntLBntDown;
	CPoint				m_pntMBntDown;
	CPoint				m_pntRBntDown;

	CString				m_strViewName;

	//新添加
	GLuint				texture[1];		// 存储一个纹理

protected: // create from serialization only
	CGLView();
	DECLARE_DYNCREATE(CGLView)

// Attributes
public:
	virtual void		RenderScene(COpenGLDC* pDC);
	void					OnViewType(UINT type);
	void					Orient_To_Plane(CVector3D nor);  //视图定向于某一指定的平面

	CString				GetViewName();
	void					SetViewName(LPCTSTR);
		
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	public:
	virtual void		OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void		DrawTuli(COpenGLDC* pDC);
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGLView();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGLView)
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnDestroy();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual BOOL	GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1);
	
	void					Zoom(double dScale);
	void					ZoomAll();
	void					MoveView(double dpx,double dpy);

	//旋转视图
	void					RotateView(double dRotX,double dRotY,double dRotZ);
	void					RotateView(double dRot,VECTOR3D vect);
	void					RotateView(double dRot,short snXyz);

	//新添加
	AUX_RGBImageRec *LoadBMP(CHAR *Filename)	;			// 载入位图图象
	int						LoadGLTextures(CHAR *Filename);										// 载入位图(调用上面的代码)并转换成纹理

	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
#endif // !defined(AFX_OPENGLDC_H__30E692A3_4129_11D4_B1EE_0010B539EBC0__INCLUDED_)
