#pragma once
#include "afxwin.h"


// CDlgDCT 对话框

class CDlgDCT : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDCT)

public:
	CDlgDCT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDCT();

// 对话框数据
	enum { IDD = IDD_DLG_DCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_path;
	afx_msg void OnBnClickedButBr();
	virtual BOOL OnInitDialog();
	CButton m_cEncode;
	CButton m_cDecode;
	afx_msg void OnBnClickedRadEn();
	afx_msg void OnBnClickedRaDe();
	
	int m_rad;
	afx_msg void OnBnClickedOk();
};
