#if !defined(AFX_DLINEPROPERTY_H__97614135_D466_427C_BBDF_A32C3123811E__INCLUDED_)
#define AFX_DLINEPROPERTY_H__97614135_D466_427C_BBDF_A32C3123811E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLineProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLineProperty
#include <afxdlgs.h>
#include "DLineDlg1.h"
#include "DLineDlg2.h"
#include "Mode.h"

class CEastDrawView;
class CDLineProperty : public CPropertySheet
{
	DECLARE_DYNAMIC(CDLineProperty)

// Construction
public:
	CDLineProperty(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDLineProperty(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

	CDLineDlg1   *m_DLineDlg1;
	CDLineDlg2   *m_DLineDlg2;
    //CMode *m_Mode;
	CBrush brush;
   // CEastDrawView *m_Parent;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLineProperty)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDLineProperty();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDLineProperty)
	afx_msg void OnClose();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLINEPROPERTY_H__97614135_D466_427C_BBDF_A32C3123811E__INCLUDED_)
