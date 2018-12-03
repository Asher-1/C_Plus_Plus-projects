// DynViews.h : main header file for the DYNVIEWS application
//

#if !defined(AFX_DYNVIEWS_H__53F8784F_EDD0_418E_B004_448FCC4BD5F1__INCLUDED_)
#define AFX_DYNVIEWS_H__53F8784F_EDD0_418E_B004_448FCC4BD5F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDynViewsApp:
// See DynViews.cpp for the implementation of this class
//

class CDynViewsApp : public CWinApp
{
public:
	CDynViewsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynViewsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDynViewsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNVIEWS_H__53F8784F_EDD0_418E_B004_448FCC4BD5F1__INCLUDED_)
