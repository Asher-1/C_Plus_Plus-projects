// DlgDCT.cpp : 实现文件
//

#include "stdafx.h"
#include "DIPDemo.h"
#include "DlgDCT.h"
#include "afxdialogex.h"


// CDlgDCT 对话框

IMPLEMENT_DYNAMIC(CDlgDCT, CDialogEx)

CDlgDCT::CDlgDCT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDCT::IDD, pParent)
	, m_path(_T(""))
	, m_rad(0)
{

}

CDlgDCT::~CDlgDCT()
{
}

void CDlgDCT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_path);
	DDX_Control(pDX, IDC_RAD_EN, m_cEncode);
	DDX_Control(pDX, IDC_RA_DE, m_cDecode);

	DDX_Radio(pDX, IDC_RAD_EN, m_rad);
}


BEGIN_MESSAGE_MAP(CDlgDCT, CDialogEx)
	ON_BN_CLICKED(IDC_BUT_BR, &CDlgDCT::OnBnClickedButBr)
	ON_BN_CLICKED(IDC_RAD_EN, &CDlgDCT::OnBnClickedRadEn)
	ON_BN_CLICKED(IDC_RA_DE, &CDlgDCT::OnBnClickedRaDe)
	ON_BN_CLICKED(IDOK, &CDlgDCT::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgDCT 消息处理程序
void CDlgDCT::OnBnClickedButBr()
{
	UpdateData(true);
	static char szFilter[] = "DCT(*.dct)|*.dct|";
	bool bEncode = (m_rad==0?FALSE:TRUE);
	CFileDialog FileDlg( bEncode, NULL,NULL,OFN_HIDEREADONLY, szFilter);

	if( FileDlg.DoModal() == IDOK ) 
	{
		m_path=FileDlg.GetPathName();
		if (m_path.Right(4)!=".dct")
			m_path=m_path+".dct";
	}
	else
	{  
		m_path = "D:\\lena.dct";
	}

	UpdateData(false);
} 


BOOL CDlgDCT::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_rad=0;
	UpdateData(false);
	return TRUE; 

}

void CDlgDCT::OnBnClickedRadEn()
{
	m_rad=0;
	UpdateData(false);
}


void CDlgDCT::OnBnClickedRaDe()
{
	m_rad=1;
	UpdateData(false);
}


void CDlgDCT::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
