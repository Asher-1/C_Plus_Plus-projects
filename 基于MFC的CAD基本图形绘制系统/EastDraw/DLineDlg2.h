#if !defined(AFX_DLINEDLG2_H__883752FA_BBB4_4601_8D6B_1AB0DDC40F44__INCLUDED_)
#define AFX_DLINEDLG2_H__883752FA_BBB4_4601_8D6B_1AB0DDC40F44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLineDlg2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLineDlg2 dialog

class CDLineDlg2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CDLineDlg2)

// Construction
public:
	void OnChange(COLORREF color);
	CDLineDlg2();
	~CDLineDlg2();
	COLORREF m_Color;
	int m_Draw_Status;
	COLORREF m_PenColor;
	COLORREF m_BrushColor;
	COLORREF m_BackgroundColor;
	CBrush brush;

// Dialog Data
	//{{AFX_DATA(CDLineDlg2)
	enum { IDD = IDD_PROPPAGE_DLine2 };
	CButton	m_Check_Pen;
	CButton	m_Check_Brush;
	CButton	m_Check_Background;
	CStatic	m_Static_ShowBrushColor;
	CStatic	m_Static_ShowBackgroundColor;
	CStatic	m_Static_ShowPenColor;
	CSpinButtonCtrl	m_Spin_Red;
	CSpinButtonCtrl	m_Spin_Green;
	CSpinButtonCtrl	m_Spin_Blue;
	CEdit	m_Edit_Red;
	CEdit	m_Edit_Green;
	CEdit	m_Edit_Blue;
	CSliderCtrl	m_Slider_Red;
	CSliderCtrl	m_Slider_Green;
	CSliderCtrl	m_Slider_Blue;
	CStatic	m_CStatic_ShowColor;
	int		m_Blue;
	int		m_Green;
	int		m_Red;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDLineDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDLineDlg2)
	afx_msg void OnSTATICBlack();
	afx_msg void OnSTATICRed();
	afx_msg void OnSTATICDarkGray();
	afx_msg void OnSTATICLightRray();
	afx_msg void OnSTATICWhite();
	afx_msg void OnSTATICDrakBlue();
	afx_msg void OnSTATICBlue();
	afx_msg void OnSTATICGreen();
	afx_msg void OnSTATICDrakGreen();
	afx_msg void OnSTATICYellow();
	afx_msg void OnSTATICRarkRed();
	afx_msg void OnStaticBitmap6();
	afx_msg void OnStaticBitmap5();
	afx_msg void OnSTATICDarkYellow();
	afx_msg void OnStaticBitmap10();
	afx_msg void OnStaticBitmap9();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBUTTONSelect();
	afx_msg void OnBUTTONColorDlg();
	afx_msg void OnCHECKPen();
	afx_msg void OnCHECKBrush();
	afx_msg void OnCHECKBackground();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLINEDLG2_H__883752FA_BBB4_4601_8D6B_1AB0DDC40F44__INCLUDED_)
