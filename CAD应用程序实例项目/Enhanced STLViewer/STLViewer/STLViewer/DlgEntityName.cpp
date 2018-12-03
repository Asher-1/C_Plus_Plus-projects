// DlgEntityName.cpp : implementation file
//

#include "stdafx.h"
#include "STLViewer.h"
#include "DlgEntityName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEntityName dialog


CDlgEntityName::CDlgEntityName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEntityName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEntityName)
	m_EntityName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgEntityName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEntityName)
	DDX_Text(pDX, IDC_ENTITY_NAME, m_EntityName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEntityName, CDialog)
	//{{AFX_MSG_MAP(CDlgEntityName)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEntityName message handlers
