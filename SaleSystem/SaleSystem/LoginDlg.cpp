// LoginDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SaleSystem.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

#include "InfoFile.h"

// LoginDlg 对话框

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DIALOG_LOGIN, pParent)
	, m_user(_T(""))
	, m_pwd(_T(""))
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_pwd);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &LoginDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &LoginDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// LoginDlg 消息处理程序


void LoginDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 登录按钮点击事件

	UpdateData(true);

	if (m_user.IsEmpty() || m_pwd.IsEmpty())
	{
		MessageBox(TEXT("输入内容不能为空"));
		return;
	}

	// 获取正确的数据
	CInfoFile file;
	CString name, pwd;
	file.ReadLogin(name, pwd);

	if (name == m_user) // 用户名一致
	{
		if (pwd == m_pwd)  // 密码一致
		{
			CDialog::OnOK(); // 登录成功
		} 
		else
		{
			MessageBox(TEXT("输入密码有误！"));
		}
	} 
	else
	{
		MessageBox(TEXT("输入用户名有误！"));
	}
}


BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 默认登录信息
	CInfoFile file;
	CString name, pwd;
	file.ReadLogin(name, pwd);
	m_user = name;
	m_pwd = pwd;

	// 同步到控件中
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void LoginDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}

// OnOk以及OnClose函数均是为了防止程序出现bug，必须重写
void LoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void LoginDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 结束整个程序
	CDialogEx::OnClose();
	exit(0);

}
