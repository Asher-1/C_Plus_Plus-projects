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
	pSTLModel = NULL; // 初始化
	m_Mode = 0; // 初始化绘制模式为0，即以原型绘制
}

CSTLViewerDoc::~CSTLViewerDoc()
{
}

BOOL CSTLViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_Part.RemoveAllEntity();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSTLViewerDoc serialization

void CSTLViewerDoc::Serialize(CArchive& ar)
{
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
	CFileDialog dlg(TRUE,_T("stl"),NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Stereo Lithograpic File(*.stl)|*.stl"), NULL );

	if(dlg.DoModal()==IDOK){
		// 创建一个空的CSTLModel的对象
		pSTLModel = new CSTLModel();

		// 在对象pSTLModel中载入文件内的STL模型
		CString strName = dlg.GetPathName();

		// 读取STL文件
		pSTLModel->LoadSTLFile(strName, m_Mode);

		if (pSTLModel->IsEmpty()) // 如果STL模型为空，释放pSTLModel对象
			delete pSTLModel;
		else			// 如果STL模型不为空，加入pSTLModel对象到m_Part中
			m_Part.AddEntity(pSTLModel);

		UpdateAllViews(NULL); // 更新所有与文档关联的视图，以显示新载入的模型
	}
}


void CSTLViewerDoc::OnCloseDocument() 
{

	CDocument::OnCloseDocument();
}
