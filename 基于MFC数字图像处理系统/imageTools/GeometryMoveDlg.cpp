/////////////////////////////////////////////////////////////////////////////
// GeometryMoveDlg dialog
// GeometryMoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\MSMoneyDemo.h"
#include "GeometryMoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GeometryMoveDlg::GeometryMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GeometryMoveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(GeometryMoveDlg)
	m_Xmove = 0;
	m_Ymove = 0;
	//}}AFX_DATA_INIT
}


void GeometryMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GeometryMoveDlg)
	DDX_Text(pDX, IDC_Edit_Xmove, m_Xmove);
	DDX_Text(pDX, IDC_Edit_Ymove, m_Ymove);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GeometryMoveDlg, CDialog)
	//{{AFX_MSG_MAP(GeometryMoveDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &GeometryMoveDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GeometryMoveDlg message handlers


void GeometryMoveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialog::OnOK();
}
