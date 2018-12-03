// RotateSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "RotateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RotateDlg dialog


RotateDlg::RotateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RotateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RotateDlg)
	m_rotateAngle = 0;
	m_interpolationSelect = 1;
	//}}AFX_DATA_INIT
}


void RotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RotateDlg)
	DDX_Text(pDX, IDC_Edit_Angle, m_rotateAngle);
	DDX_Radio(pDX, IDC_RADIO1, m_interpolationSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RotateDlg, CDialog)
	//{{AFX_MSG_MAP(RotateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RotateDlg message handlers

void RotateDlg::OnOK()
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
