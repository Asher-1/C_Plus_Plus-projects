// LeftDockBar.h: interface for the CLeftDockBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTDOCKBAR_H__7A0EF27C_C98F_46FE_A9C9_FE977A73627E__INCLUDED_)
#define AFX_LEFTDOCKBAR_H__7A0EF27C_C98F_46FE_A9C9_FE977A73627E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
//#include "resource.h"

#include "CoolDialogBar.h"

//AFX_EXT_API void WINAPI InitClass();

class AFX_EXT_CLASS CTabSheet : public CTabCtrl
{
// Construction
public:
	CTabSheet();
	CArray<CWnd*,CWnd*> m_arrView; // 指向控件中页面的指针数组

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSheet)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	//}}AFX_VIRTUAL

// Implementation
public:
	void RemoveAllPage(); // 删除所有页面
	void SetMargin(int margin); // 设置页边距
	void SetActiveView(int nView); // 设置当前页
	
	// 使用动态创建方式创建一个页面，并增加到控件中
	// lpszLabel是该窗口的标签名称
	BOOL AddView(LPTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext=NULL);

	// 将一个已创建的页面增加到对话框中，lpszLabel是该窗口的标签名称
	BOOL AddView(LPTSTR name, CWnd* pView);

	virtual ~CTabSheet();

	// Generated message map functions
protected:
	int		m_iMargin; // 页边距
	CWnd*	m_curView; // 当前页的指针
	CRect	m_rcView; // 页窗口大小
	CFont	m_TabFont; // 标签的字体

	virtual void InitFont(); // 初始化标签的字体
	//{{AFX_MSG(CTabSheet)

	// 响应页面的选择变化
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);

	// 响应页面的尺寸变化，使得页面的尺寸随控件尺寸的变化而变化
	afx_msg void OnSize(UINT nType,int cx,int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


class AFX_EXT_CLASS CTabSheetDockBar : public CCoolDialogBar
{
	DECLARE_DYNCREATE(CTabSheetDockBar)
public:
	CTabSheetDockBar();
	virtual ~CTabSheetDockBar();
	virtual BOOL Create( CWnd* pParentWnd,LPCTSTR pTitle);

	// Dialog Data
	//{{AFX_DATA(CTabSheetDockBar)
	//}}AFX_DATA

	CTabSheet	m_TabCtrl;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSheetDockBar)
	//}}AFX_VIRTUAL

	void	SetFloatSize(const CSize& size);
	void	SetDockSize(const CSize& size);
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabSheetDockBar)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class AFX_EXT_CLASS CMessageViewDockBar : public CCoolDialogBar  
{
	DECLARE_DYNCREATE(CMessageViewDockBar)
protected:
	RECT	m_rcView;
	CView*	m_pOutputView;
public:
	CMessageViewDockBar();
	virtual ~CMessageViewDockBar();
	virtual BOOL Create( CWnd* pParentWnd,LPCTSTR pTitle);

	void	AddMessageLine(LPCTSTR pStr);
	void	RemoveAllMessageLines();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageViewDockBar)
	//}}AFX_VIRTUAL

	void	SetFloatSize(const CSize& size);
	void	SetDockSize(const CSize& size);

protected:
	BOOL	AddView(CRuntimeClass *pViewClass, CCreateContext *pContext);

	// Generated message map functions
	//{{AFX_MSG(COutputDockBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LEFTDOCKBAR_H__7A0EF27C_C98F_46FE_A9C9_FE977A73627E__INCLUDED_)
