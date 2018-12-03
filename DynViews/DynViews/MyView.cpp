// MyView.cpp : implementation file
//

#include "stdafx.h"
#include "DynViews.h"
#include "MyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyView

IMPLEMENT_DYNCREATE(CMyView, CView)

CMyView::CMyView()
{
}

CMyView::~CMyView()
{
	if(! m_points.IsEmpty()) 
	{
		POSITION pos = m_points.GetHeadPosition();
		while(pos != NULL)
			delete ((CMyPoint*) m_points.GetNext(pos));
	};
}


BEGIN_MESSAGE_MAP(CMyView, CView)
	//{{AFX_MSG_MAP(CMyView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyView drawing

void CMyView::OnDraw(CDC* pDC)
{

	CRect rect;
	GetClientRect(&rect);
	
	pDC->DrawEdge(rect, EDGE_ETCHED, BF_RECT);

	pDC->SelectStockObject(BLACK_PEN);

	BOOL first = TRUE;
	POSITION pos = m_points.GetHeadPosition();
	if(!pos)
	{
		//pDC->TextOut(0,0,"Try to draw some lines !");
		return;
	}
	while(pos != NULL) 
	{
		CMyPoint* p = (CMyPoint*) m_points.GetNext(pos);
		if(first) { pDC->MoveTo(p->m_location); first = FALSE; }
		pDC->LineTo(p->m_location);
	};
}

/////////////////////////////////////////////////////////////////////////////
// CMyView diagnostics

#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CView::AssertValid();
}

void CMyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyView message handlers

void CMyView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_points.AddTail(new CMyPoint(point));
	
	CView::OnLButtonDown(nFlags, point);

	Invalidate();
}

void CMyView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(nFlags && MK_LBUTTON) 
	{
		m_points.AddTail(new CMyPoint(point));
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

