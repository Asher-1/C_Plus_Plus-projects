// STLViewer.h : main header file for the STLVIEWER application
//

#if !defined(AFX_STLVIEWER_H__A1AFD80F_9D09_4632_A654_451C63C95A03__INCLUDED_)
#define AFX_STLVIEWER_H__A1AFD80F_9D09_4632_A654_451C63C95A03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerApp:
// See STLViewer.cpp for the implementation of this class
//

class CSTLViewerApp : public CWinApp
{
public:
	CSTLViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTLViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	virtual int ExitInstance();

// Implementation
	//{{AFX_MSG(CSTLViewerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLVIEWER_H__A1AFD80F_9D09_4632_A654_451C63C95A03__INCLUDED_)
