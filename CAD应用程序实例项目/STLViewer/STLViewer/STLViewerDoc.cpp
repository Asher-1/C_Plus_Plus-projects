// STLViewerDoc.cpp : implementation of the CSTLViewerDoc class
//

#include "stdafx.h"
#include "STLViewer.h"

#include "STLViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc

IMPLEMENT_DYNCREATE(CSTLViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSTLViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CSTLViewerDoc)
	ON_COMMAND(ID_STL_FILEIN, OnStlFilein)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc construction/destruction

CSTLViewerDoc::CSTLViewerDoc()
{
	// TODO: add one-time construction code here
	pSTLModel = NULL; // ³õÊ¼»¯
	m_Mode = 0; // ³õÊ¼»¯»æÖÆÄ£Ê½Îª0£¬¼´ÒÔÔ­ÐÍ»æÖÆ
}

CSTLViewerDoc::~CSTLViewerDoc()
{

}

BOOL CSTLViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_Part.RemoveAllEntity(); // Çå¿Õm_PartÖÐµÄËùÓÐ×ÓÄ£ÐÍ
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc serialization

void CSTLViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()){
	}
	else{
	}
	m_Part.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc diagnostics

#ifdef _DEBUG
void CSTLViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSTLViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc commands

void CSTLViewerDoc::OnStlFilein() 
{
	// 用文件对话框打开一个以STL为后缀的文件
	CFileDialog dlg(TRUE,_T("stl"),NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Stereo Lithograpic File(*.stl)|*.stl"), NULL );

	if(dlg.DoModal()==IDOK){

		// ´´½¨Ò»¸ö¿ÕµÄCSTLModelµÄ¶ÔÏó
		pSTLModel = new CSTLModel();

		// ÔÚ¶ÔÏópSTLModelÖÐÔØÈëÎÄ¼þÄÚµÄSTLÄ£ÐÍ
		CString strName = dlg.GetPathName();

		// ¶ÁÈ¡STLÎÄ¼þ
		pSTLModel->LoadSTLFile(strName, m_Mode);

		if(pSTLModel->IsEmpty()) // Èç¹ûSTLÄ£ÐÍÎª¿Õ£¬ÊÍ·ÅpSTLModel¶ÔÏó
			delete pSTLModel;
		else			// Èç¹ûSTLÄ£ÐÍ²»Îª¿Õ£¬¼ÓÈëpSTLModel¶ÔÏóµ½m_PartÖÐ
			m_Part.AddEntity(pSTLModel);

		UpdateAllViews(NULL); // ¸üÐÂËùÓÐÓëÎÄµµ¹ØÁªµÄÊÓÍ¼£¬ÒÔÏÔÊ¾ÐÂÔØÈëµÄÄ£ÐÍ
	}
}

