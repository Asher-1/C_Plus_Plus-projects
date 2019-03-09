#if !defined(AFX_TEXTEDIT_H__DDDF3BEE_ED75_40D9_B233_1E43EEDCAFBF__INCLUDED_)
#define AFX_TEXTEDIT_H__DDDF3BEE_ED75_40D9_B233_1E43EEDCAFBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextEdit window

class CTextEdit : public CEdit
{
// Construction
public:
	CTextEdit();
	CString m_String;
	BOOL m_IsMoving;
	BOOL m_IsOnMoving;
	CPoint m_FirstPoint;
	CPoint m_SecondPoint;
	CPoint m_PositionPoint;
    BOOL m_Sizing;
	CFont *m_Font;


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetMyFont(CFont *font);
	virtual ~CTextEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextEdit)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUnDo();
	afx_msg void OnCut();
	afx_msg void OnCopy();
	afx_msg void OnPast();
	afx_msg void OnDelete();
	afx_msg void OnSelectAll();
	afx_msg void OnMove();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMENUITEMSize();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTEDIT_H__DDDF3BEE_ED75_40D9_B233_1E43EEDCAFBF__INCLUDED_)
