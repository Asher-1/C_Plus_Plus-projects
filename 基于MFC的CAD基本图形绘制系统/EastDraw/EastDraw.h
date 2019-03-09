// EastDraw.h : main header file for the EASTDRAW application
//

#if !defined(AFX_EASTDRAW_H__0BFB64BD_393E_402A_B24C_ADEECCF11610__INCLUDED_)
#define AFX_EASTDRAW_H__0BFB64BD_393E_402A_B24C_ADEECCF11610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEastDrawApp:
// See EastDraw.cpp for the implementation of this class
//

class CEastDrawApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEastDrawApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEastDrawApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEastDrawApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASTDRAW_H__0BFB64BD_393E_402A_B24C_ADEECCF11610__INCLUDED_)
