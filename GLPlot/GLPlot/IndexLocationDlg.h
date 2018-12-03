#pragma once
#include "resource.h"

// CIndexLocationDlg 对话框

class CIndexLocationDlg : public CDialog
{
	DECLARE_DYNAMIC(CIndexLocationDlg)

public:
	CIndexLocationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIndexLocationDlg();

// 对话框数据
	enum { IDD = IDD_INDEX_LOCATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    UINT m_EditIndex;
public:
    afx_msg void OnBnClickedOk();
};
