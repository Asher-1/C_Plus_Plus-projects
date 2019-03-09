// Mode.cpp : implementation file
//

#include "stdafx.h"
#include "eastdraw.h"
#include "Mode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMode property page

IMPLEMENT_DYNCREATE(CMode, CPropertyPage)

CMode::CMode() : CPropertyPage(CMode::IDD)
{
	//{{AFX_DATA_INIT(CMode)
	m_Edit_Detail = _T("");
	m_bMode = FALSE;
	//}}AFX_DATA_INIT
	m_DrawingMode=R2_BLACK; 
	//m_StringtoIntmap=new CMap<CString,CString&,int,int&>(1000);
    m_StringtoString=new CMapStringToString(1000);
	m_BKMode=OPAQUE;
	m_bMode=false;
	
}

CMode::~CMode()
{
}

void CMode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMode)
	DDX_Control(pDX, IDC_CHECK_ModeCheck, m_Check_mode);
	DDX_Control(pDX, IDC_COMBO_BackgroundMode, m_Combo_BackgroundMode);
	DDX_Control(pDX, IDC_TREE_DrawingMode, m_Tree_DrawingMode);
	DDX_Text(pDX, IDC_EDIT_Detail, m_Edit_Detail);
	DDX_Check(pDX, IDC_CHECK_ModeCheck, m_bMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMode, CPropertyPage)
	//{{AFX_MSG_MAP(CMode)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DrawingMode, OnSelchangedTREEDrawingMode)
	ON_CBN_SELCHANGE(IDC_COMBO_BackgroundMode, OnSelchangeCOMBOBackgroundMode)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_ModeCheck, OnCHECKModeCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMode message handlers

BOOL CMode::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
  CString str[16]=
 {
	 "R2_BLACK","R2_WHITE","R2_NOP","R2_NOT", 
	 "R2_COPYPEN", "R2_NOTCOPYPEN","R2_MERGEPENNOT", "R2_MASKPENNOT", 
	 "R2_MERGENOTPEN" ,"R2_MASKNOTPEN" ,"R2_MERGEPEN", "R2_NOTMERGEPEN",
	 "R2_MASKPEN","R2_NOTMASKPEN","R2_XORPEN","R2_NOTXORPEN"
 };
int  mode[16]=
 {
	 R2_BLACK,R2_WHITE,R2_NOP,R2_NOT, 
	 R2_COPYPEN, R2_NOTCOPYPEN,R2_MERGEPENNOT, R2_MASKPENNOT, 
	 R2_MERGENOTPEN ,R2_MASKNOTPEN ,R2_MERGEPEN, R2_NOTMERGEPEN,
	 R2_MASKPEN,R2_NOTMASKPEN,R2_XORPEN, R2_NOTXORPEN
 };

CString strInfo[16]=
{
 "Pixel is always black.",
 "Pixel is always white.",
 "Pixel remains unchanged.",
 "Pixel is the inverse of the screen color.",
 "Pixel is the pen color.",
 "Pixel is the inverse of the pen color.",
 "Pixel is a combination of the pen color and the inverse of the screen color (final pixel = (NOT screen pixel) OR pen).",
 "Pixel is a combination of the colors common to both the pen and the inverse of the screen (final pixel = (NOT screen pixel) AND pen).",
 "Pixel is a combination of the screen color and the inverse of the pen color (final pixel = (NOT pen) OR screen pixel).",
 "Pixel is a combination of the colors common to both the screen and the inverse of the pen (final pixel = (NOT pen) AND screen pixel).",
 "Pixel is a combination of the pen color and the screen color (final pixel = pen OR screen pixel).",
 "Pixel is the inverse of the R2_MERGEPEN color (final pixel = NOT(pen OR screen pixel)).",
 "Pixel is a combination of the colors common to both the pen and the screen (final pixel = pen AND screen pixel).",
 "Pixel is the inverse of the R2_MASKPEN color (final pixel = NOT(pen AND screen pixel)).",
 "Pixel is a combination of the colors that are in the pen or in the screen, but not in both (final pixel = pen XOR screen pixel).",
 "Pixel is the inverse of the R2_XORPEN color (final pixel = NOT(pen XOR screen pixel)). "
};

for(int i=0;i<16;i++)
{
//(*m_StringtoIntmap)[_T(str[i])]=mode[i];
//(*m_StringtoString)[_T(str[i])]=_T(strInfo[i]);
}
	
 HTREEITEM hItem;
 hItem=m_Tree_DrawingMode.InsertItem("DrawingMode");

HTREEITEM hItemSon=m_Tree_DrawingMode.InsertItem(str[0]);
 for(i=1;i<16;i++)
{
 m_Tree_DrawingMode.InsertItem(_T(str[i]));
}

m_Tree_DrawingMode.Expand(hItem,TVE_EXPAND);
m_Tree_DrawingMode.Select(hItemSon,TVGN_CARET);
m_Combo_BackgroundMode.SetCurSel(0);
m_BKMode=OPAQUE;
m_Edit_Detail="Pixel is always black.";
this->UpdateData(false);
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CMode::OnSelchangedTREEDrawingMode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem=m_Tree_DrawingMode.GetSelectedItem();
    CString str_Item=m_Tree_DrawingMode.GetItemText(hItem);
   // m_StringtoIntmap->Lookup(_T(str_Item),m_DrawingMode);

    //m_StringtoString->Lookup(_T(str_Item),m_Edit_Detail);
	this->UpdateData(false);
	*pResult = 0;



}

void CMode::OnSelchangeCOMBOBackgroundMode() 
{

	m_BKMode=m_Combo_BackgroundMode.GetCurSel()==0?OPAQUE:TRANSPARENT;
	
	// TODO: Add your control notification handler code here
	
}

HBRUSH CMode::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	
	pDC->SetTextColor(RGB(0,100,255));
   pDC->SetBkColor(RGB(100,200,200));
   pDC->SelectObject(&brush);
   pDC->SetBkMode(TRANSPARENT);
	return hbr;
}

void CMode::OnCHECKModeCheck() 
{
	// TODO: Add your control notification handler code here
 m_bMode=!m_bMode;
	
 m_Check_mode.SetCheck(m_bMode?1:0);
	
}
