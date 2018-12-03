#if !defined(AFX_ROTATESETDLG_H__E270A01E_447A_4C0C_A549_3839A6FA8529__INCLUDED_)
#define AFX_ROTATESETDLG_H__E270A01E_447A_4C0C_A549_3839A6FA8529__INCLUDED_

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
	float	m_rotateAngle;
	int		m_interpolationSelect;
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
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATESETDLG_H__E270A01E_447A_4C0C_A549_3839A6FA8529__INCLUDED_)
