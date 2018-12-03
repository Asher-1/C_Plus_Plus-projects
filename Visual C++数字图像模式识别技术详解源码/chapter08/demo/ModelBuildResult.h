#if !defined(AFX_MODELBUILDRESULT_H__D52EE68A_EBAE_4B3A_AF53_D35C8A962349__INCLUDED_)
#define AFX_MODELBUILDRESULT_H__D52EE68A_EBAE_4B3A_AF53_D35C8A962349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelBuildResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ModelBuildResult dialog

class ModelBuildResult : public CDialog
{
// Construction
public:
	BOOL Create();
	ModelBuildResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ModelBuildResult)
	enum { IDD = IDD_ModelResult };

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModelBuildResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ModelBuildResult)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELBUILDRESULT_H__D52EE68A_EBAE_4B3A_AF53_D35C8A962349__INCLUDED_)
