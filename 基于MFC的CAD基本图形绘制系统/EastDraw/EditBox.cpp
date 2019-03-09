// EditBox.cpp : implementation file
//

#include "stdafx.h"
#include "eastdraw.h"
#include "EditBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditBox dialog


CEditBox::CEditBox(CWnd* pParent /*=NULL*/)
	: CDialog(CEditBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditBox)
	//}}AFX_DATA_INIT
   m_RichEditCtrl.Create(
   WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE,
   CRect(10,10,100,200),this, 1);	
}


void CEditBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditBox)
	DDX_Control(pDX, IDC_RICHEDIT2, m_RichEditCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditBox, CDialog)
	//{{AFX_MSG_MAP(CEditBox)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditBox message handlers
