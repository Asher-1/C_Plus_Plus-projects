// Zoomdlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "Zoomdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Zoomdlg dialog


Zoomdlg::Zoomdlg(CWnd* pParent /*=NULL*/)
	: CDialog(Zoomdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(Zoomdlg)
	m_XstrechRatio = 0.0f;
	m_YstrechRatio = 0.0f;
	//}}AFX_DATA_INIT
}


void Zoomdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Zoomdlg)
	DDX_Text(pDX, IDC_EDIT_strechW, m_XstrechRatio);
	DDX_Text(pDX, IDC_EDIT_strechH, m_YstrechRatio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Zoomdlg, CDialog)
	//{{AFX_MSG_MAP(Zoomdlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Zoomdlg message handlers
