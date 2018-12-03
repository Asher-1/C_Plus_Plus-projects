// VideoDemoDoc.cpp : implementation of the CVideoDemoDoc class
//

#include "stdafx.h"
#include "VideoDemo.h"

#include "VideoDemoDoc.h"
//#include "AVIHandle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoDoc

IMPLEMENT_DYNCREATE(CVideoDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CVideoDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CVideoDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoDoc construction/destruction

CVideoDemoDoc::CVideoDemoDoc()
{
	m_avi = NULL;
	isAvi = false;
}

CVideoDemoDoc::~CVideoDemoDoc()
{
	if(m_avi!= NULL)
	delete m_avi;
}

BOOL CVideoDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVideoDemoDoc serialization

void CVideoDemoDoc::Serialize(CArchive& ar)
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
// CVideoDemoDoc diagnostics

#ifdef _DEBUG
void CVideoDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVideoDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoDoc commands

BOOL CVideoDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	m_avi=new AVIHandle(lpszPathName);	// 打开AVI文件
	isAvi=true;
	Avidata=m_avi->GetSingleFrame(0);	// 获得第一帧图像
	SetModifiedFlag(FALSE);     // start off with unmodified
	return true;
}
