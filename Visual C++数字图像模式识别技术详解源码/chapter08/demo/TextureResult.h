#if !defined(AFX_TEXTURERESULT_H__716AB63A_B9A7_4E4C_B77B_1286C18B6591__INCLUDED_)
#define AFX_TEXTURERESULT_H__716AB63A_B9A7_4E4C_B77B_1286C18B6591__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextureResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TextureResult dialog

class TextureResult : public CDialog
{
// Construction
public:
	TextureResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TextureResult)
	enum { IDD = IDD_DIALOG2 };
	double	m_dCorrelation;
	double	m_dEnergy;
	double	m_dEntropy;
	double	m_dInertiaQuadrature;
	double	m_dLocalCalm;

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TextureResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TextureResult)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTURERESULT_H__716AB63A_B9A7_4E4C_B77B_1286C18B6591__INCLUDED_)
