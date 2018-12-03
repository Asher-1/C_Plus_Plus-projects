// MyTabCtrl.cpp : implementation file
//"HOWTO: Change the Background Color of a Tab Control"

#include "stdafx.h"
#include "dynviews.h"
#include "MyTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl
// self draw fixed.
CMyTabCtrl::CMyTabCtrl()
{
}

CMyTabCtrl::~CMyTabCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTabCtrl)
	//}}AFX_MSG_MAP
	ON_WM_DRAWITEM_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl message handlers
void CMyTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	char szTabText[100];
    memset(szTabText, '\0', sizeof(szTabText));
	
	TC_ITEM tci;
    memset(&tci, 0, sizeof(tci));
	
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	//tci.pszText     = szTabText;
	tci.cchTextMax  = sizeof(szTabText)-1;
	// get item
	GetItem(lpdis->itemID, &tci);
	//
	CDC *pdc = CDC::FromHandle(lpdis->hDC);
	// light green
	CRect rc=lpdis->rcItem;
	rc.DeflateRect(3,3);
	
	pdc->FillSolidRect(&rc, RGB(180,255,255));
	pdc->SetBkMode(TRANSPARENT);
	//	pdc->SetBkColor(RGB(180,255,255));
	pdc->SetTextColor(RGB(255,0,0));
	// draw img
	if(tci.iImage > -1)
	{
		CImageList *pImg=GetImageList();
		HICON hicon=pImg->ExtractIcon(lpdis->itemID);
		DrawIconEx(lpdis->hDC,rc.left,rc.top,hicon,0,0,0,0,DI_NORMAL);
		rc.left += 20;
	}
//
	TextOut(lpdis->hDC,rc.left,rc.top,tci.pszText,lstrlen(tci.pszText));
}
