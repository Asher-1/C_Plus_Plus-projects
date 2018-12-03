// MyLeftDockBar.h: interface for the CMyLeftDockBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLEFTDOCKBAR_H__99693328_5A79_4FD0_B100_AFB53CAB4CF6__INCLUDED_)
#define AFX_MYLEFTDOCKBAR_H__99693328_5A79_4FD0_B100_AFB53CAB4CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\inc\dockTool\DockTool.h"

class CMyLeftDockBar : public CTabSheetDockBar  
{
	DECLARE_DYNCREATE(CMyLeftDockBar)
public:
	CMyLeftDockBar();

protected:

	// Generated message map functions
	//{{AFX_MSG(CMyLeftDockBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MYLEFTDOCKBAR_H__99693328_5A79_4FD0_B100_AFB53CAB4CF6__INCLUDED_)
