// MyLeftDockBar.cpp: implementation of the CMyLeftDockBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STLViewer.h"
#include "MyLeftDockBar.h"

#include "PartTreeView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMyLeftDockBar,CTabSheetDockBar )
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyLeftDockBar::CMyLeftDockBar():CTabSheetDockBar()
{
	SetDockSize(CSize(200,200));
	SetFloatSize(CSize(200,200));
}

BEGIN_MESSAGE_MAP(CMyLeftDockBar,CTabSheetDockBar)
	//{{AFX_MSG_MAP(CMyLeftDockBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//message handles
int CMyLeftDockBar::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CTabSheetDockBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_TabCtrl.AddView(_T("Part"),RUNTIME_CLASS(CPartTreeView));
	m_TabCtrl.SetActiveView(0);

	return 0;
}
