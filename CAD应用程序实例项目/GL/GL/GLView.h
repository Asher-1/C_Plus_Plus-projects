// GLView.h : interface of the CGLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVIEW_H__B2996431_1A48_492F_87B8_D8E922845B95__INCLUDED_)
#define AFX_GLVIEW_H__B2996431_1A48_492F_87B8_D8E922845B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glut.h"
#include "gl/glaux.h"
#pragma comment(lib, "glaux")// 必须加的声明，放置出现无法识别的外部符号
#pragma comment(lib, "legacy_stdio_definitions") // 强制兼容旧的代码，编译通过

class CGLView : public CView
{
protected: // create from serialization only
	CGLView();
	DECLARE_DYNCREATE(CGLView)

// Attributes
public:
	CGLDoc* GetDocument();

// Operations
public:

	GLfloat m_xAngle;
	GLfloat m_yAngle;
	GLfloat m_xPos;
	GLfloat m_yPos;
	CPoint m_MouseDownPoint;
	float m_Scale;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	public:

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void RenderScene();

	virtual ~CGLView();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGLView)
	afx_msg int	 OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void		GLInit();
	void		GLRelease();
	void		GLResize(int cx,int cy);
	void		GLSetupRC();

	HGLRC		m_hRC;		//rendering context
	HDC			m_hDC;		//device context
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in GLView.cpp
inline CGLDoc* CGLView::GetDocument()
   { return (CGLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLVIEW_H__B2996431_1A48_492F_87B8_D8E922845B95__INCLUDED_)
