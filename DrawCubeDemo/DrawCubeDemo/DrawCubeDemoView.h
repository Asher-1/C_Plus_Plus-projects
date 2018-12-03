// DrawCubeDemoView.h : interface of the CDrawCubeDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWCUBEDEMOVIEW_H__14CA8B5F_DF83_4AA2_B471_C50D19923F70__INCLUDED_)
#define AFX_DRAWCUBEDEMOVIEW_H__14CA8B5F_DF83_4AA2_B471_C50D19923F70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "gl.h"
//#include "glu.h"
#include "glut.h"
#include "glaux.h"
#pragma comment(lib, "glaux")// 必须加的声明，放置出现无法识别的外部符号
#pragma comment(lib, "legacy_stdio_definitions") // 强制兼容旧的代码，编译通过

class CDrawCubeDemoView : public CView
{
protected: // create from serialization only
	CDrawCubeDemoView();
	DECLARE_DYNCREATE(CDrawCubeDemoView)

// Attributes
public:
	CDrawCubeDemoDoc* GetDocument();
	
	   HGLRC m_hRC;    //Rendering Context
	   CDC* m_pDC;        //Device Context
	   BOOL InitializeOpenGL();    //Initialize OpenGL
	   BOOL SetupPixelFormat();    //Set up the Pixel Format
     void RenderScene();            //Render the Scene
	 GLfloat m_xAngle;
     GLfloat m_yAngle;
     GLfloat m_xPos;
     GLfloat m_yPos;
     CPoint m_MouseDownPoint;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawCubeDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	float m_Scale;
	virtual ~CDrawCubeDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawCubeDemoView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DrawCubeDemoView.cpp
inline CDrawCubeDemoDoc* CDrawCubeDemoView::GetDocument()
   { return (CDrawCubeDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWCUBEDEMOVIEW_H__14CA8B5F_DF83_4AA2_B471_C50D19923F70__INCLUDED_)
