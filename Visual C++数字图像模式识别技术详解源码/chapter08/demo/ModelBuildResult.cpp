// ModelBuildResult.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "ModelBuildResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ModelBuildResult dialog


ModelBuildResult::ModelBuildResult(CWnd* pParent /*=NULL*/)
	: CDialog(ModelBuildResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(ModelBuildResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ModelBuildResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ModelBuildResult)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ModelBuildResult, CDialog)
	//{{AFX_MSG_MAP(ModelBuildResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModelBuildResult message handlers


void ModelBuildResult::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

BOOL ModelBuildResult::Create()
{
	return CDialog::Create(ModelBuildResult::IDD);
}
