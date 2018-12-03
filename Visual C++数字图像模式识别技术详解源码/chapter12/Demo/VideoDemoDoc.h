// VideoDemoDoc.h : interface of the CVideoDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEODEMODOC_H__1DF8EC5C_8D7A_4CAD_9E48_473C3B65250E__INCLUDED_)
#define AFX_VIDEODEMODOC_H__1DF8EC5C_8D7A_4CAD_9E48_473C3B65250E__INCLUDED_

#include "AVIHandle.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVideoDemoDoc : public CDocument
{
protected: // create from serialization only
	CVideoDemoDoc();
	DECLARE_DYNCREATE(CVideoDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	AVIHandle *m_avi;
	bool isAvi;
	unsigned char *Avidata;
	virtual ~CVideoDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVideoDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEODEMODOC_H__1DF8EC5C_8D7A_4CAD_9E48_473C3B65250E__INCLUDED_)
