// STLViewerView.h : interface of the CSTLViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STLVIEWERVIEW_H__D5F8FC2E_373C_442E_AAF5_D321322B1658__INCLUDED_)
#define AFX_STLVIEWERVIEW_H__D5F8FC2E_373C_442E_AAF5_D321322B1658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define TETLIBRARY

#include "..\inc\glContext\openGLDC.h" // 说明基类CGLView
#include "..\inc\glContext\Camera.H"

//#include "..\inc\tetgen\tetgen.h"
#pragma comment(lib,"TetGen.lib")

class CSTLViewerView : public CGLView // 在基类CGLView上派生
{
protected: // create from serialization only
	CSTLViewerView();
	DECLARE_DYNCREATE(CSTLViewerView)

// Attributes
public:
	CSTLViewerDoc* GetDocument();

	GLfloat m_xAngle;
	GLfloat m_yAngle; // 声明旋转角度
	GLfloat m_xPos;
	GLfloat m_yPos;
	CPoint m_MouseDownPoint;
	float m_Scale;

	// 重新定义虚拟函数，实现多态性
	virtual void RenderScene(COpenGLDC* pDC);

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTLViewerView)
	public:
		// 注释掉此函数，使用基类的OnDraw()函数
		//virtual void OnDraw(CDC*pDC);  
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTLViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSTLViewerView)
	// 视图选择的命令处理函数
	afx_msg void OnViewBack();
	afx_msg void OnViewBottom();
	afx_msg void OnViewFront();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnViewTop();
	afx_msg void OnViewSWIsometric();
	afx_msg void OnViewSEIsometric();
	afx_msg void OnViewNEIsometric();
	afx_msg void OnViewNWIsometric();

	// 全局缩放函数
	afx_msg void OnViewZoomall();

	// 模型显示放大函数
	afx_msg void OnViewZoomin();

	// 模型显示缩小函数
	afx_msg void OnViewZoomout();

	// 渲染/非渲染切换
	afx_msg void OnViewShade();

	// 响应键盘消息
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	// 实现类自己的虚拟函数GetBox()
	virtual BOOL	GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in STLViewerView.cpp
inline CSTLViewerDoc* CSTLViewerView::GetDocument()
   { return (CSTLViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLVIEWERVIEW_H__D5F8FC2E_373C_442E_AAF5_D321322B1658__INCLUDED_)
