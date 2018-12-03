// GLPlot.h : main header file for the GLPLOT application
//

#if !defined(AFX_GLPLOT_H__2B92CB6C_C56D_48F2_86DD_BED205A443CB__INCLUDED_)
#define AFX_GLPLOT_H__2B92CB6C_C56D_48F2_86DD_BED205A443CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGLPlotApp:
// See GLPlot.cpp for the implementation of this class
//

class CGLPlotApp : public CWinApp
{
public:
	CGLPlotApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLPlotApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGLPlotApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLPLOT_H__2B92CB6C_C56D_48F2_86DD_BED205A443CB__INCLUDED_)
