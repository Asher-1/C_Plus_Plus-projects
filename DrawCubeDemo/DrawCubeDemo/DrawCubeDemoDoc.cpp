// DrawCubeDemoDoc.cpp : implementation of the CDrawCubeDemoDoc class
//

#include "stdafx.h"
#include "DrawCubeDemo.h"

#include "DrawCubeDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoDoc

IMPLEMENT_DYNCREATE(CDrawCubeDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDrawCubeDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDrawCubeDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoDoc construction/destruction

CDrawCubeDemoDoc::CDrawCubeDemoDoc()
{
	// TODO: add one-time construction code here

}

CDrawCubeDemoDoc::~CDrawCubeDemoDoc()
{
}

BOOL CDrawCubeDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoDoc serialization

void CDrawCubeDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoDoc diagnostics

#ifdef _DEBUG
void CDrawCubeDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDrawCubeDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoDoc commands
