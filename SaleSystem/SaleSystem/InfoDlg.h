#pragma once
#include "afxwin.h"

// CInfoDlg 窗体视图

class CInfoDlg : public CFormView
{
	DECLARE_DYNCREATE(CInfoDlg)

protected:
	CInfoDlg();           // 动态创建所使用的受保护的构造函数
	virtual ~CInfoDlg();

public:
	enum {IDD = DIALOG_INFO};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

private:
	CListCtrl m_list;
};


