// GLPlotDlg.h : header file
//

#if !defined(AFX_GLPLOTDLG_H__0D343863_2F90_4CA4_82B8_98B999E0A9A3__INCLUDED_)
#define AFX_GLPLOTDLG_H__0D343863_2F90_4CA4_82B8_98B999E0A9A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGLPlotDlg dialog

class CGLDrawView;
class CGLPlotDlg : public CDialog
{
// Construction
public:
	CGLPlotDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGLPlotDlg)
	enum { IDD = IDD_GLPLOT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLPlotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CGLDrawView* glDrawView_;
	// Generated message map functions
	//{{AFX_MSG(CGLPlotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLPLOTDLG_H__0D343863_2F90_4CA4_82B8_98B999E0A9A3__INCLUDED_)
