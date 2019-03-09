#if !defined(AFX_MODE_H__B7279911_A903_4090_B8EC_3918CB6F8511__INCLUDED_)
#define AFX_MODE_H__B7279911_A903_4090_B8EC_3918CB6F8511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMode dialog
#include <afxtempl.h>
#include <afxcoll.h>
class CMode : public CPropertyPage
{
	DECLARE_DYNCREATE(CMode)

// Construction
public:
	CMode();
	~CMode();

// Dialog Data
	//{{AFX_DATA(CMode)
	enum { IDD = IDD_PROPPAGE_Mode };
	CButton	m_Check_mode;
	CComboBox	m_Combo_BackgroundMode;
	CTreeCtrl	m_Tree_DrawingMode;
	CString	m_Edit_Detail;
	BOOL	m_bMode;
	//}}AFX_DATA
    int m_DrawingMode;
	int m_BKMode;
	CBrush brush;
	//CMap<CString,CString&,int,int&>*m_StringtoIntmap;
    CMapStringToString *m_StringtoString;
	
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMode)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTREEDrawingMode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCOMBOBackgroundMode();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCHECKModeCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODE_H__B7279911_A903_4090_B8EC_3918CB6F8511__INCLUDED_)
