#if !defined(AFX_STYLEBAR_H__C85C9099_A154_11D2_8E53_006008A82731__INCLUDED_)
#define AFX_STYLEBAR_H__C85C9099_A154_11D2_8E53_006008A82731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleBar.h : header file
//
#include"Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CStyleBar command target
#include <afxtempl.h>
#include <afxcoll.h>
class CStyleBar : public CToolBar
{
// Attributes
public:
  
void OnCharBold();
// Operations
public:
	void IniFont();
	void OnCharUnderline();
	void OnCharItalic();
    static int CALLBACK EnumFontNameProc (ENUMLOGFONT* lpelf,
    NEWTEXTMETRIC* lpntm, int nFontType, LPARAM lParam);
	BOOL isBoldUp;
	BOOL isUnderlineUp;
    BOOL isItalicUp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleBar)
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitTypefaceList (CDC* pDC);
	CFont m_font;
	CComboBox m_wndFontNames;
	CComboBox m_wndFontSizes;
	CMap<int,int,ENUMLOGFONT,ENUMLOGFONT>m_FontMap;
	// Generated message map functions
	//{{AFX_MSG(CStyleBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
    afx_msg void OnSelectFont ();
    afx_msg void OnSelectSize ();
	//afx_msg void OnCharBold();
    // afx_msg	void OnCharItalic();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEBAR_H__C85C9099_A154_11D2_8E53_006008A82731__INCLUDED_)
