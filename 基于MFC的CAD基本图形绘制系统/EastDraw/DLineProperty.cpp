// DLineProperty.cpp : implementation file
//

#include "stdafx.h"
#include "EastDraw.h"
#include "DLineProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLineProperty

IMPLEMENT_DYNAMIC(CDLineProperty, CPropertySheet)

CDLineProperty::CDLineProperty(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_DLineDlg1=new CDLineDlg1;
    m_DLineDlg2=new CDLineDlg2;
   // m_Mode=new CMode;
	this->AddPage(m_DLineDlg1);
	this->AddPage(m_DLineDlg2);
//	this->AddPage(m_Mode);
	//m_Parent=(CEastDrawView*)pParentWnd;


}

CDLineProperty::CDLineProperty(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{	
	
	m_DLineDlg1=new CDLineDlg1;
    m_DLineDlg2=new CDLineDlg2;
//	m_Mode=new CMode;

	this->AddPage(m_DLineDlg1);
	this->AddPage(m_DLineDlg2);
//	this->AddPage(m_Mode);
	
	
    
}

CDLineProperty::~CDLineProperty()
{
}


BEGIN_MESSAGE_MAP(CDLineProperty, CPropertySheet)
	//{{AFX_MSG_MAP(CDLineProperty)
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLineProperty message handlers

void CDLineProperty::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	

	this->ShowWindow(false);
	//CPropertySheet::OnClose();
}











void CDLineProperty::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		
	 CPropertySheet::OnNcLButtonDblClk(nHitTest, point);
     
     static BOOL s_bExchange=true;
	 CRect rect;
	 this->GetWindowRect(&rect);

	 s_bExchange?this->SetWindowPos(&wndTop,0,0,rect.Width(),rect.Height()/13+1,SWP_NOMOVE):
	             this->SetWindowPos(&wndTop,0,0,rect.Width(),(rect.Height()-1)*13,SWP_NOMOVE);
	 s_bExchange=!s_bExchange;
}

HBRUSH CDLineProperty::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
   pDC->SetTextColor(RGB(0,100,255));
   pDC->SetBkColor(RGB(100,200,200));
   pDC->SelectObject(&brush);
   pDC->SetBkMode(OPAQUE);
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDLineProperty::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	 CPen pen;
     CRect rect;
	 pen.CreatePen(PS_SOLID,20,RGB(156,200,255));
     CPen*oldPen=dc.SelectObject(&pen);
    
    this->GetClientRect(&rect);

    dc.SetBkColor(RGB(255,0,0));
    dc.SetBkMode(TRANSPARENT);

     dc.Rectangle(rect);
	 
     dc.SetBkMode(OPAQUE);
     

	
	

	// Do not call CPropertySheet::OnPaint() for painting messages
}

void CDLineProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPropertySheet::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDLineProperty::OnNcPaint() 
{
	// TODO: Add your message handler code here
	
	// Do not call  for painting messages
    CPropertySheet::OnNcPaint();
    CPen pen;
    CRect rect;
	pen.CreatePen(PS_SOLID,2,RGB(0,0,255));
    CDC *pDC=GetWindowDC();
	this->GetWindowRect(rect);
	this->ScreenToClient(&rect);
    CPen*pOldPen=pDC->SelectObject(&pen);
	
	pDC->MoveTo(rect.right+1,rect.top+20);
    pDC->LineTo(rect.left+3,rect.top+20);
	pDC->LineTo(rect.left+4,rect.bottom+17);
	pDC->LineTo(rect.right+2,rect.bottom+17);
	pDC->LineTo(rect.right+2,rect.top+19);
    
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	pDC->SelectObject(&pen);
    
	pDC->MoveTo(rect.right-1,rect.top+21);
    pDC->LineTo(rect.left+5,rect.top+21);
	pDC->LineTo(rect.left+5,rect.bottom+16);
	pDC->LineTo(rect.right,rect.bottom+16);
	pDC->LineTo(rect.right,rect.top+20);
}
