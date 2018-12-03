#if !defined(AFX_OPENGLVIEW_H__74EC4762_679C_4978_A5E2_7484A31F9EBE__INCLUDED_)
#define AFX_OPENGLVIEW_H__74EC4762_679C_4978_A5E2_7484A31F9EBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGLView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenGLView view

//#include <glu.h>
#include "MyView.h"
//#include <GL.h>
#include <glut.h>

// This view encapsulates a personalized 3D view
class COpenGLView : public CView
{
public:
	COpenGLView();           // protected constructor used by dynamic creation
	virtual ~COpenGLView();

protected:
	DECLARE_DYNCREATE(COpenGLView)

// Attributes
protected:
	CDC*			m_pDC;
	HGLRC			m_hRC;
	GLUquadricObj*	m_quadric;
// Operations
public:
	int m_idView;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void SetupPixelFormat();
	void InitGL();
	//{{AFX_MSG(COpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLVIEW_H__74EC4762_679C_4978_A5E2_7484A31F9EBE__INCLUDED_)
