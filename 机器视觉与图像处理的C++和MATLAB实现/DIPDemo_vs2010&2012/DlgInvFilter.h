#pragma once


// CDlgInvFilter 对话框

class CDlgInvFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInvFilter)

public:
	CDlgInvFilter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInvFilter();

// 对话框数据
	enum { IDD = IDD_DLG_INV_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iRad;
	virtual BOOL OnInitDialog();
	CString m_show;
};
