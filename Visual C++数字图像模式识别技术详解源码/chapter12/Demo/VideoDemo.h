// VideoDemo.h : main header file for the VIDEODEMO application
//

#if !defined(AFX_VIDEODEMO_H__EC00FE14_6D78_48A8_97C4_C03A8BA674D8__INCLUDED_)
#define AFX_VIDEODEMO_H__EC00FE14_6D78_48A8_97C4_C03A8BA674D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "AVIHandle.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoApp:
// See VideoDemo.cpp for the implementation of this class
//

class CVideoDemoApp : public CWinApp
{
public:
	CVideoDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CVideoDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEODEMO_H__EC00FE14_6D78_48A8_97C4_C03A8BA674D8__INCLUDED_)
