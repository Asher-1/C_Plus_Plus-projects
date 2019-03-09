#if !defined(AFX_REGINSET_H__C3DA9B06_357D_4E7B_9B85_E4BF1C6A6B02__INCLUDED_)
#define AFX_REGINSET_H__C3DA9B06_357D_4E7B_9B85_E4BF1C6A6B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReginSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReginSet dialog

class CReginSet : public CDialog
{
// Construction
public:
	CReginSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReginSet)
	enum { IDD = IDD_DIALOG_ReginSet };
	CEdit	m_Edit_Y;
	CEdit	m_Edit_X;
	DWORD	m_Length;
	DWORD	m_Width;
	int		m_X;
	int		m_Y;
	//}}AFX_DATA
    BOOL    m_bIsViewReign;
    CBrush brush;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReginSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReginSet)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGINSET_H__C3DA9B06_357D_4E7B_9B85_E4BF1C6A6B02__INCLUDED_)
