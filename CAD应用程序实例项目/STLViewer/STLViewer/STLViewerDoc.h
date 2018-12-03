// STLViewerDoc.h : interface of the CSTLViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STLVIEWERDOC_H__1A27C2B5_C135_45BF_986A_BD43824D093D__INCLUDED_)
#define AFX_STLVIEWERDOC_H__1A27C2B5_C135_45BF_986A_BD43824D093D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\inc\GeomKernel\Entity.h"

class CSTLViewerDoc : public CDocument
{
protected: // create from serialization only
	CSTLViewerDoc();
	DECLARE_DYNCREATE(CSTLViewerDoc)

// Attributes
public:
	CPart	m_Part;
	CSTLModel* pSTLModel;
	
	int m_Mode;
// Operations
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar); // 串行化读取函数

// Implementation
public:
	virtual ~CSTLViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSTLViewerDoc)
	afx_msg void OnStlFilein();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLVIEWERDOC_H__1A27C2B5_C135_45BF_986A_BD43824D093D__INCLUDED_)
