// demo1View.h : interface of the CDemo1View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO1VIEW_H__22279905_D13D_47FC_A210_36D8D93C09EF__INCLUDED_)
#define AFX_DEMO1VIEW_H__22279905_D13D_47FC_A210_36D8D93C09EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDemo1View : public CView
{
protected: // create from serialization only
	CDemo1View();
	DECLARE_DYNCREATE(CDemo1View)

// Attributes
public:
	CDemo1Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo1View)
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
	virtual ~CDemo1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemo1View)
	afx_msg void OnMove();
	afx_msg void OnHorizontalMirror();
	afx_msg void OnVerticalMirror();
	afx_msg void OnZoom();
	afx_msg void OnClockwise90();
	afx_msg void OnAnticlockwise90();
	afx_msg void OnRotate180();
	afx_msg void OnFreeRotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in demo1View.cpp
inline CDemo1Doc* CDemo1View::GetDocument()
   { return (CDemo1Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO1VIEW_H__22279905_D13D_47FC_A210_36D8D93C09EF__INCLUDED_)
