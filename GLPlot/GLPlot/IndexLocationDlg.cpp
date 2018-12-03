// IndexLocationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IndexLocationDlg.h"


// CIndexLocationDlg 对话框

IMPLEMENT_DYNAMIC(CIndexLocationDlg, CDialog)

CIndexLocationDlg::CIndexLocationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIndexLocationDlg::IDD, pParent)
    , m_EditIndex(0)
{

}

CIndexLocationDlg::~CIndexLocationDlg()
{
}

void CIndexLocationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_INDEX, m_EditIndex);
}


BEGIN_MESSAGE_MAP(CIndexLocationDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CIndexLocationDlg 消息处理程序

void CIndexLocationDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    if (!UpdateData())
    {
        return;
    }
    OnOK();
}
