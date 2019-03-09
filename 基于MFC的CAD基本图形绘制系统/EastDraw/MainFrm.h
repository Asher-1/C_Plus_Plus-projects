// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__A2F7CE67_D58B_48EC_BDC8_F7C32BC788A6__INCLUDED_)
#define AFX_MAINFRM_H__A2F7CE67_D58B_48EC_BDC8_F7C32BC788A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DLineProperty.h"
#include "StyleBar.h"
#include"Resource.h"
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
     
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndDrawToolBar;
public: 	
	CStyleBar	m_wndStyleBar;

public:   
	void AddTempFile();
	void SetMousePosText(CPoint Logpoint,CPoint Devpoint);
	void ShowFontBar(BOOL isShow=true);
	CDLineProperty* m_DLineProperty;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowPorDlg();
	afx_msg void OnUpdateShowPorDlg(CCmdUI* pCmdUI);
	afx_msg void OnShowDrawToolBar();
	afx_msg void OnUpdateShowDrawToolBar(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnUpdateMousPoint(CCmdUI* pCmdUI);
	
    afx_msg void OnUpdateMousCaption(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A2F7CE67_D58B_48EC_BDC8_F7C32BC788A6__INCLUDED_)
