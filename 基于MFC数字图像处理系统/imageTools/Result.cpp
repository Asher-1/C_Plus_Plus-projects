// Result.cpp : implementation file
//

#include "stdafx.h"
#include "..\MSMoneyDemo.h"
#include "Result.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResult dialog


CResult::CResult(CWnd* pParent /*=NULL*/)
	: CDialog(CResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResult)
	m_str=_T("");
	//}}AFX_DATA_INIT
}


void CResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResult)
	DDX_Text(pDX, IDC_EDITResult, m_str);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResult, CDialog)
	//{{AFX_MSG_MAP(CResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResult message handlers

void CResult::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
