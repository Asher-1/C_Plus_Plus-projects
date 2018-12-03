// SellDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SaleSystem.h"
#include "SellDlg.h"
#include "InfoFile.h"


// CSellDlg

IMPLEMENT_DYNCREATE(CSellDlg, CFormView)

CSellDlg::CSellDlg()
	: CFormView(CSellDlg::IDD)
	, m_sellInfo(_T(""))
	, m_price(0)
	, m_num(0)
	, m_left(0)
{

}

CSellDlg::~CSellDlg()
{
}

void CSellDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);

	DDX_Text(pDX, IDC_EDIT4, m_sellInfo);

	DDX_Text(pDX, IDC_EDIT1, m_price);
	DDX_Text(pDX, IDC_EDIT5, m_num);
	DDX_Text(pDX, IDC_EDIT6, m_left);
}

BEGIN_MESSAGE_MAP(CSellDlg, CFormView)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSellDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSellDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSellDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CSellDlg 诊断

#ifdef _DEBUG
void CSellDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSellDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSellDlg 消息处理程序


void CSellDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	//初始化消息订单窗口

	//初始化下拉框
	
	CInfoFile file;
	//把商品读到file对象中
	file.ReadDocline();

	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		//遍历商品容器  把容器名称放到下拉框中

		//it->name 是 string  AddString(CString )
		// string  -> CString

		m_combo.AddString(CString(it->name.c_str()));
	}

	//默认选中第一个商品
	m_combo.SetCurSel(0);

	OnCbnSelchangeCombo1();//更新第一个商品里的数据

}


void CSellDlg::OnCbnSelchangeCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
	//切换商品 触发的事件


	//先获取到商品的名称

	int index = m_combo.GetCurSel();
	//根据索引获取到具体内容
	CString name;
	m_combo.GetLBText(index, name);

	//根据商品名称获取到该商品的价格和库存 并且显示到控件中
	CInfoFile file;
	file.ReadDocline();
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		if ((CString)it->name.c_str() == name)
		{
			m_price = it->price;
			m_left = it->num;
			//同步到控件上
			UpdateData(FALSE);
		}
	}
}


void CSellDlg::OnBnClickedButton3()
{
	UpdateData(TRUE);
	// TODO:  在此添加控件通知处理程序代码
	//购买的功能实现
	if (m_num <= 0)
	{
		MessageBox(TEXT("购买数量要大于0"));
		return;
	}
	//购买的数量 不能大于库存
	if (m_num > m_left)
	{
		MessageBox(TEXT("购买数量不能大于库存量"));
		return;
	}

	//购买
	//获取到具体要买的商品名称
	int index = m_combo.GetCurSel();
	CString name;
	m_combo.GetLBText(index, name);

	CInfoFile file;
	file.ReadDocline();
	for (list<msg>::iterator it = file.ls.begin(); it != file.ls.end(); it++)
	{
		if ( (CString)it->name.c_str() == name)
		{
			//同步库存量
			it->num -= m_num;
			m_left = it->num; //控件上的库存量
			//告诉用户具体的购买信息

			CString str;
			str.Format(_T("商品：%s \r\n单价：%d \r\n个数：%d \r\n总价：%d"), name, m_price, m_num, m_price*m_num);
			m_sellInfo = str;
			UpdateData(FALSE);
			MessageBox(TEXT("购买成功"));
		}
	}
	
	//把最新的数据写到文件中
	file.WirteDocline();

	//清空数据
	m_num = 0;
	m_sellInfo.Empty();
	UpdateData(FALSE);
}


void CSellDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	//取消按钮
	m_num = 0;
	UpdateData(FALSE);
	//默认选中第一个
	m_combo.SetCurSel(0);
	OnCbnSelchangeCombo1();//更新第一个商品里的数据

}
