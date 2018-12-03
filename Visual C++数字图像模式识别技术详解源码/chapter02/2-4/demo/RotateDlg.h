#if !defined(AFX_ROTATEDLG_H__C10805BA_A6C1_472D_866E_ED789EC22287__INCLUDED_)
#define AFX_ROTATEDLG_H__C10805BA_A6C1_472D_866E_ED789EC22287__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RotateDlg dialog

class RotateDlg : public CDialog
{
// Construction
public:
	RotateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RotateDlg)
	enum { IDD = IDD_DIALOG_Rotate };
	int		m_RotateAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RotateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RotateDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATEDLG_H__C10805BA_A6C1_472D_866E_ED789EC22287__INCLUDED_)
