// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E0C622E4_E7F9_417F_B009_D19B762E32D7__INCLUDED_)
#define AFX_STDAFX_H__E0C622E4_E7F9_417F_B009_D19B762E32D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifndef WINVER    // 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501  // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif
#ifndef _WIN32_WINNT  // 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501 // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif 

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E0C622E4_E7F9_417F_B009_D19B762E32D7__INCLUDED_)
