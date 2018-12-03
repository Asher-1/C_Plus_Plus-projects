// TextureResult.cpp : implementation file
//

#include "stdafx.h"
#include "demo1.h"
#include "TextureResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TextureResult dialog


TextureResult::TextureResult(CWnd* pParent /*=NULL*/)
	: CDialog(TextureResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(TextureResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void TextureResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ENERGY, m_dEnergy);
	DDX_Text(pDX, IDC_EDIT_ENTROPY, m_dEntropy);
	DDX_Text(pDX, IDC_EDIT_INERTIA_QUADRATURE, m_dInertiaQuadrature);
	DDX_Text(pDX, IDC_EDIT_LOCAL_CALM, m_dLocalCalm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TextureResult, CDialog)
	//{{AFX_MSG_MAP(TextureResult)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TextureResult message handlers
