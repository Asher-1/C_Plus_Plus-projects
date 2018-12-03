// VideoDemoView.h : interface of the CVideoDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEODEMOVIEW_H__87E41874_C87F_496D_9E4D_C8F57A0CB223__INCLUDED_)
#define AFX_VIDEODEMOVIEW_H__87E41874_C87F_496D_9E4D_C8F57A0CB223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVideoDemoView : public CView
{
protected: // create from serialization only
	CVideoDemoView();
	DECLARE_DYNCREATE(CVideoDemoView)

// Attributes
public:
	CVideoDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoDemoView)
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
	virtual ~CVideoDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVideoDemoView)
	afx_msg void OnObjectDetect();
	afx_msg void OnObjectTrace();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in VideoDemoView.cpp
inline CVideoDemoDoc* CVideoDemoView::GetDocument()
   { return (CVideoDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEODEMOVIEW_H__87E41874_C87F_496D_9E4D_C8F57A0CB223__INCLUDED_)
