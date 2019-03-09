// ReginSet.cpp : implementation file
//

#include "stdafx.h"
#include "eastdraw.h"
#include "ReginSet.h"
#include "TextEdit.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReginSet dialog


CReginSet::CReginSet(CWnd* pParent /*=NULL*/)
	: CDialog(CReginSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReginSet)
	m_Length = 0;
	m_Width = 0;
	m_X = 0;
	m_Y = 0;
	//}}AFX_DATA_INIT
	m_bIsViewReign=false;
	this->brush.CreateSolidBrush(RGB(255,0,0));
}


void CReginSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReginSet)
	DDX_Control(pDX, IDC_EDIT4, m_Edit_Y);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_X);
	DDX_Text(pDX, IDC_EDIT_Length, m_Length);
	DDX_Text(pDX, IDC_EDIT_Width, m_Width);
	DDX_Text(pDX, IDC_EDIT3, m_X);
	DDX_Text(pDX, IDC_EDIT4, m_Y);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReginSet, CDialog)
	//{{AFX_MSG_MAP(CReginSet)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReginSet message handlers

HBRUSH CReginSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
   pDC->SetTextColor(RGB(0,100,255));
   pDC->SetBkColor(RGB(0,0,200));
   pDC->SelectObject(&brush);
   pDC->SetBkMode(TRANSPARENT);
	// TODO: Return a different brush if the default is not desired
	return hbr;
}




BOOL CReginSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(this->m_bIsViewReign)
	{	
		this->m_Edit_X.EnableWindow();
	    this->m_Edit_Y.EnableWindow();
	}  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
