// DrawCubeDemoDoc.h : interface of the CDrawCubeDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWCUBEDEMODOC_H__DA79D550_9523_4DD4_A639_E2F8C0FE8835__INCLUDED_)
#define AFX_DRAWCUBEDEMODOC_H__DA79D550_9523_4DD4_A639_E2F8C0FE8835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDrawCubeDemoDoc : public CDocument
{
protected: // create from serialization only
	CDrawCubeDemoDoc();
	DECLARE_DYNCREATE(CDrawCubeDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawCubeDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawCubeDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawCubeDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWCUBEDEMODOC_H__DA79D550_9523_4DD4_A639_E2F8C0FE8835__INCLUDED_)
