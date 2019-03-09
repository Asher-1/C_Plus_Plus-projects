// EastDrawView.h : interface of the CEastDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EASTDRAWVIEW_H__3327611D_6660_4C0B_B9C2_6AE46861E30C__INCLUDED_)
#define AFX_EASTDRAWVIEW_H__3327611D_6660_4C0B_B9C2_6AE46861E30C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CUnit;
class CDLineProperty;
class CEastDrawDoc;
class CEditPane;
//class CTextEdit;
#include "stdafx.h"

#include <direct.h>
#include <math.h>
#include <io.h>
#include <errno.h>



class CEastDrawView : public CScrollView
{
protected: // create from serialization only
	CEastDrawView();
	DECLARE_DYNCREATE(CEastDrawView)

// Attributes
public:
	CEastDrawDoc* GetDocument();

// Operations
public:
    int m_CurrentDrawTool;
    int	m_CurrentDrawStatus;
	CClientDC *m_DrawDC;
	CPoint m_FirstPoint;
	CPoint m_SecondPoint;
	CUnit *m_pCurrentUnit;
	BOOL m_IsDrawing;
    CDLineProperty* m_DLineProperty;
	BOOL m_IsPolygonFirst;
    CBitmap *m_Bitmap;
	int m_LBDownTimes;
	BOOL m_bHaveFindFirst;
	BOOL m_bHaveFindSecond;
	int L_iPointCount;
	CPoint m_LastDownPoint;
	CPoint m_MinPoint;
	CPoint m_MaxPoint;
	CRect  m_InvalidRect;
	CPoint m_ChangeingPoint;
	CString string;
	CPoint m_CenterPoint;
	BOOL m_HaveAxis;
	BOOL m_IsLocked;
	int  m_Rtimes;
	int  m_Index;
	BOOL m_IsMouseCap;
	BOOL m_IsLastMove;
	
//	CTextEdit* m_pTextEdit;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEastDrawView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEastDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
public:
	void SetBoxFont();
	void ReMoveTemp();
	void ExchangeSave();
	void AutoSave();
	void AddTempFile();
	void DrawAll(CDC*pDrawDC);
	void DeleteSelect();
	BOOL SaveDIB(CBitmap*pbitmap, CFile&file);
	void DrawAxis(CDC*pDC);
	void SetMousePosText(CPoint Logpoint,CPoint Devpoint);
	void ShowFontBar(BOOL isShow);
	void GetFontInfo(LPTSTR pszFaceName, int& nSize);
	void InvalidateMovingRect(CUnit*pMovingUnit);
	void ComputeMinandMaxPoint(CPoint point);
	void ResetSelectStatus();
	
// Generated message map functions
protected:
	//{{AFX_MSG(CEastDrawView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBUTTONDLine();
	afx_msg void OnBUTTON32774Move();
	afx_msg void OnBUTTON32775CircumRotate();
	afx_msg void OnBUTTON32777Curve();
	afx_msg void OnBUTTON32776Rectangle();
	afx_msg void OnBUTTON32778RoundRectangle();
	afx_msg void OnBUTTON32779Ellipse();
	afx_msg void OnBUTTON32780Round();
	afx_msg void OnBUTTON32791Polygon();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMENUITEM32794SetRegin();
	afx_msg void OnBUTTONLiEllipse();
	afx_msg void OnBUTTON32800RoundArc();
	afx_msg void OnBUTTON32805EllipseArc();
	afx_msg void OnBUTTON32807PolyBezier();
	afx_msg void OnBUTTON32809Text();
	afx_msg void OnCharBold();
	afx_msg void OnCharItalic();
	afx_msg void OnCharUnderline();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuitemOk();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCirCu();
	afx_msg void OnUpdateCirCu(CCmdUI* pCmdUI);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDelete();
	afx_msg void OnBUTTON32830Lock();
	afx_msg void OnUpdateBUTTON32830Lock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTONDLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32776Rectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32778RoundRectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32791Polygon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32779Ellipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32780Round(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32777Curve(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTONLiEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32800RoundArc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32805EllipseArc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32807PolyBezier(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32809Text(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32774Move(CCmdUI* pCmdUI);
	afx_msg void OnBUTTON32845Axis();
	afx_msg void OnUpdateBUTTON32845Axis(CCmdUI* pCmdUI);
	afx_msg void OnBUTTON32850ReDraw();
	afx_msg void OnBUTTON32856DONothing();
	afx_msg void OnBUTTON32858OnDo();
	afx_msg void OnBUTTON32859ReDo();
	afx_msg void OnUpdateBUTTON32858OnDo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBUTTON32859ReDo(CCmdUI* pCmdUI);
	afx_msg void OnMouseCapOn();
	afx_msg void OnUpdateMouseCapOn(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EastDrawView.cpp
inline CEastDrawDoc* CEastDrawView::GetDocument()
   { return (CEastDrawDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASTDRAWVIEW_H__3327611D_6660_4C0B_B9C2_6AE46861E30C__INCLUDED_)
