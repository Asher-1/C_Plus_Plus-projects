// DlgInvFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "DIPDemo.h"
#include "DlgInvFilter.h"
#include "afxdialogex.h"


// CDlgInvFilter 对话框

IMPLEMENT_DYNAMIC(CDlgInvFilter, CDialogEx)

CDlgInvFilter::CDlgInvFilter(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInvFilter::IDD, pParent)
	, m_iRad(0)
	, m_show(_T(""))
{

}

CDlgInvFilter::~CDlgInvFilter()
{
}

void CDlgInvFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iRad);
	DDX_Text(pDX, IDC_STATIC1, m_show);
}


BEGIN_MESSAGE_MAP(CDlgInvFilter, CDialogEx)
END_MESSAGE_MAP()


// CDlgInvFilter 消息处理程序


BOOL CDlgInvFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_show = "逆滤波半径";
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
