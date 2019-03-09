#if !defined(AFX_DLINEDLG1_H__00324F16_5568_4746_9EC1_47DDA898FC01__INCLUDED_)
#define AFX_DLINEDLG1_H__00324F16_5568_4746_9EC1_47DDA898FC01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLineDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLineDlg1 dialog

class CDLineDlg1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CDLineDlg1)

// Construction
public:
	void OnChangeBrushStyle(BOOL isHatched);
	void OnChangePenStyle();
	CDLineDlg1();
	~CDLineDlg1();
	CBrush brush;
    int m_BKMode;
// Dialog Data
	//{{AFX_DATA(CDLineDlg1)
	enum { IDD = IDD_PROPPAGE_DLine1 };
	CComboBox	m_Combo_BackgroundMode;
	CButton	m_radio_bs_Hollow;
	CButton	m_radio_ps_Solid;
	CButton	m_radio_vertical;
	CButton	m_radio_horizontal;
	CButton	m_radio_fdiagonal;
	CButton	m_radio_diagcross;
	CButton	m_radio_bdiagonal;
	CButton	m_radio_corss;
	CStatic	m_Static_ShowBrushResult;
	CStatic	m_Static_ShowPenStyle;
	CSpinButtonCtrl	m_Spin_DLine_Width;
	UINT	m_Edit_LineWidth;
	int		m_radio_BrushHatch;
	int		m_radio_BrushStyle;
	//}}AFX_DATA
    int m_PenStyle;
	UINT m_BrushStyle;
	LONG m_BrushHatch;
	COLORREF m_BrushColor;
	COLORREF m_PenColor;
	

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDLineDlg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDLineDlg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnRADIOPsSolid();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio15();
	afx_msg void OnRadio16();
	afx_msg void OnPaint();
	afx_msg void OnRADIO6bshollow();
	afx_msg void OnRADIO7bssolid();
	afx_msg void OnRADIO8hatched();
	afx_msg void OnDeltaposSPINDDLineWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEDITLineWidth();
	afx_msg void OnRadioHs();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeCOMBOBackgroundMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLINEDLG1_H__00324F16_5568_4746_9EC1_47DDA898FC01__INCLUDED_)
