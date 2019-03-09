#if !defined(AFX_EDITBOX_H__AE655D8B_4076_4E63_81FE_1FEC6E1E062B__INCLUDED_)
#define AFX_EDITBOX_H__AE655D8B_4076_4E63_81FE_1FEC6E1E062B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditBox dialog

class CEditBox : public CDialog
{
// Construction
public:
	CEditBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditBox)
	enum { IDD = IDD_DIALOG1 };
	CRichEditCtrl	m_RichEditCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditBox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITBOX_H__AE655D8B_4076_4E63_81FE_1FEC6E1E062B__INCLUDED_)
