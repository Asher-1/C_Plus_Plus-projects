#if !defined(AFX_MYTABCTRL_H__1F49EC13_162E_4BBF_BA0A_555CE0EA81CE__INCLUDED_)
#define AFX_MYTABCTRL_H__1F49EC13_162E_4BBF_BA0A_555CE0EA81CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl window

class CMyTabCtrl : public CTabCtrl
{
// Construction
public:
	CMyTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTabCtrl)
	//}}AFX_MSG
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDis);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__1F49EC13_162E_4BBF_BA0A_555CE0EA81CE__INCLUDED_)
