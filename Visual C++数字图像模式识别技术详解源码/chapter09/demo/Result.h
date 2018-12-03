#if !defined(AFX_RESULT_H__10CBAE89_7E0C_4DC7_B28B_EB4A8ABAE116__INCLUDED_)
#define AFX_RESULT_H__10CBAE89_7E0C_4DC7_B28B_EB4A8ABAE116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Result.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResult dialog

class CResult : public CDialog
{
// Construction
public:
	CResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResult)
	enum { IDD = IDD_DIALOG2 };
	CString m_str;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResult)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULT_H__10CBAE89_7E0C_4DC7_B28B_EB4A8ABAE116__INCLUDED_)
