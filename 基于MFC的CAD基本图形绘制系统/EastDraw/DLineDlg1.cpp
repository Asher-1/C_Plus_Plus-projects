// DLineDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "EastDraw.h"
#include "DLineDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLineDlg1 property page

IMPLEMENT_DYNCREATE(CDLineDlg1, CPropertyPage)

CDLineDlg1::CDLineDlg1() : CPropertyPage(CDLineDlg1::IDD)
{
	//{{AFX_DATA_INIT(CDLineDlg1)
	m_Edit_LineWidth = 1;
	m_radio_BrushHatch = 0;
	m_radio_BrushStyle = 0;
	//}}AFX_DATA_INIT
    m_PenStyle=PS_SOLID;
	m_BrushStyle=BS_HOLLOW;
     m_BKMode=OPAQUE;  
	
}

CDLineDlg1::~CDLineDlg1()
{
	
}

void CDLineDlg1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLineDlg1)
	DDX_Control(pDX, IDC_COMBO_BackgroundMode, m_Combo_BackgroundMode);
	DDX_Control(pDX, IDC_RADIO6_bs_hollow, m_radio_bs_Hollow);
	DDX_Control(pDX, IDC_RADIO_Ps_Solid, m_radio_ps_Solid);
	DDX_Control(pDX, IDC_RADIO_HS_VERTICAL, m_radio_vertical);
	DDX_Control(pDX, IDC_RADIO14_HS_HORIZONTAL, m_radio_horizontal);
	DDX_Control(pDX, IDC_RADIO13_HS_FDIAGONAL, m_radio_fdiagonal);
	DDX_Control(pDX, IDC_RADIO11_HS_DIAGCROSS, m_radio_diagcross);
	DDX_Control(pDX, IDC_RADIO9_HS_BDIAGONAL, m_radio_bdiagonal);
	DDX_Control(pDX, IDC_RADIO10_HS_CROSS, m_radio_corss);
	DDX_Control(pDX, IDC_STATIC_ShowBrushResult, m_Static_ShowBrushResult);
	DDX_Control(pDX, IDC_STATIC_ShowPenStyle, m_Static_ShowPenStyle);
	DDX_Control(pDX, IDC_SPIN_DDLineWidth, m_Spin_DLine_Width);
	DDX_Text(pDX, IDC_EDIT_LineWidth, m_Edit_LineWidth);
	DDV_MinMaxUInt(pDX, m_Edit_LineWidth, 1, 10000);
	DDX_Radio(pDX, IDC_RADIO9_HS_BDIAGONAL, m_radio_BrushHatch);
	DDX_Radio(pDX, IDC_RADIO6_bs_hollow, m_radio_BrushStyle);
	//}}AFX_DATA_MAP
	m_Combo_BackgroundMode.SetCurSel(m_BKMode==OPAQUE?0:1);
}


BEGIN_MESSAGE_MAP(CDLineDlg1, CPropertyPage)
	//{{AFX_MSG_MAP(CDLineDlg1)
	ON_BN_CLICKED(IDC_RADIO_Ps_Solid, OnRADIOPsSolid)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO15, OnRadio15)
	ON_BN_CLICKED(IDC_RADIO16, OnRadio16)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO6_bs_hollow, OnRADIO6bshollow)
	ON_BN_CLICKED(IDC_RADIO7_bs_solid, OnRADIO7bssolid)
	ON_BN_CLICKED(IDC_RADIO8_hatched, OnRADIO8hatched)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DDLineWidth, OnDeltaposSPINDDLineWidth)
	ON_EN_CHANGE(IDC_EDIT_LineWidth, OnChangeEDITLineWidth)
	ON_BN_CLICKED(IDC_RADIO9_HS_BDIAGONAL, OnRadioHs)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO11_HS_DIAGCROSS, OnRadioHs)
	ON_BN_CLICKED(IDC_RADIO13_HS_FDIAGONAL, OnRadioHs)
	ON_BN_CLICKED(IDC_RADIO10_HS_CROSS, OnRadioHs)
	ON_BN_CLICKED(IDC_RADIO14_HS_HORIZONTAL, OnRadioHs)
	ON_BN_CLICKED(IDC_RADIO_HS_VERTICAL, OnRadioHs)
	ON_CBN_SELCHANGE(IDC_COMBO_BackgroundMode, OnSelchangeCOMBOBackgroundMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLineDlg1 message handlers
#include "DLineProperty.h"
BOOL CDLineDlg1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Spin_DLine_Width.SetBase(1);
	m_Spin_DLine_Width.SetPos(1);
	m_Spin_DLine_Width.SetRange(1,1000);
	
	m_radio_bdiagonal.SetCheck(2);
    m_radio_ps_Solid.SetCheck(2);
	m_radio_bs_Hollow.SetCheck(2);
    m_BrushHatch=HS_BDIAGONAL;
	m_Combo_BackgroundMode.SetCurSel(0);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLineDlg1::OnRADIOPsSolid() 
{
	// TODO: Add your control notification handler code here
  m_PenStyle=PS_SOLID;
  OnChangePenStyle();
}

void CDLineDlg1::OnRadio2() 
{
	// TODO: Add your control notification handler code here

  m_PenStyle=PS_DASH;
  OnChangePenStyle();	

}

void CDLineDlg1::OnRadio3() 
{
	// TODO: Add your control notification handler code here
  m_PenStyle=PS_DOT;
  OnChangePenStyle();	
	

}

void CDLineDlg1::OnRadio4() 
{
	// TODO: Add your control notification handler code here
  m_PenStyle=PS_DASHDOT;
 
  OnChangePenStyle();	
}

void CDLineDlg1::OnRadio5() 
{
	// TODO: Add your control notification handler code here
  m_PenStyle=PS_DASHDOTDOT;
  OnChangePenStyle();   	
}

void CDLineDlg1::OnRadio15() 
{
	// TODO: Add your control notification handler code here
  m_PenStyle=PS_NULL;
  OnChangePenStyle();
}

void CDLineDlg1::OnRadio16() 
{
	// TODO: Add your control notification handler code here
  m_PenStyle=PS_INSIDEFRAME;
  OnChangePenStyle();	
}

void CDLineDlg1::OnPaint() 
{
  CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
  OnChangePenStyle();
  OnChangeBrushStyle(m_BrushStyle==BS_HATCHED);
  

	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CDLineDlg1::OnChangePenStyle()
{
  UpdateData();
  CDC * pDC; 
  CRect rect;
  CPen pen;
  CBrush brush;

  CDLineProperty*parent=(CDLineProperty*)this->GetParent();
  this->m_PenColor=parent->m_DLineDlg2->m_PenColor;

  brush.CreateSolidBrush(RGB(255,255,255));
  pDC=m_Static_ShowPenStyle.GetDC();
  pen.CreatePen(m_PenStyle,m_Edit_LineWidth>10?10:m_Edit_LineWidth,m_PenColor);
  CPen*oldpen=pDC->SelectObject(&pen);
  
  m_Static_ShowPenStyle.GetClientRect(&rect);
  pDC->FillRect(&rect,&brush);
  
  brush.DeleteObject();
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;
  brush.CreateBrushIndirect(&brushlog);
  CBrush*OldBrush=pDC->SelectObject(&brush);
  int oldBKMode=pDC->SetBkMode(m_BKMode);
  int oldBkColor=pDC->SetBkColor(parent->m_DLineDlg2->m_BackgroundColor);

  pDC->MoveTo(rect.left+5,rect.Height()/2);
  pDC->LineTo(rect.right-5,rect.Height()/2);
  
  pDC->SelectObject(oldpen);
  pDC->SetBkMode(oldBKMode);
  pDC->SelectObject(OldBrush);
  m_Static_ShowPenStyle.ReleaseDC(pDC);
}

void CDLineDlg1::OnChangeBrushStyle(BOOL isHatched)
{

    m_radio_vertical.EnableWindow(isHatched);
	m_radio_horizontal.EnableWindow(isHatched);
	m_radio_fdiagonal.EnableWindow(isHatched);
	m_radio_diagcross.EnableWindow(isHatched);
	m_radio_bdiagonal.EnableWindow(isHatched);
	m_radio_corss.EnableWindow(isHatched);


  UpdateData();
  CDC * pDC; 
  CRect rect;
  CBrush brush;
  
  pDC=m_Static_ShowBrushResult.GetDC();
  m_Static_ShowBrushResult.GetClientRect(&rect);
  
  CDLineProperty*parent=(CDLineProperty*)this->GetParent();
  this->m_BrushColor=parent->m_DLineDlg2->m_BrushColor;
  
  brush.CreateSolidBrush(RGB(255,255,255));
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  
  
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;
  brush.CreateBrushIndirect(&brushlog);
  int oldBKMode=pDC->SetBkMode(m_BKMode);
  COLORREF oldBkColor=pDC->SetBkColor(parent->m_DLineDlg2->m_BackgroundColor);
  pDC->SelectObject(&brush);

  pDC->Rectangle(rect);
  m_Static_ShowBrushResult.ReleaseDC(pDC);




}

void CDLineDlg1::OnRADIO6bshollow() 
{
	// TODO: Add your control notification handler code here
  m_BrushStyle=BS_HOLLOW;
  OnChangeBrushStyle(false);
	
}

void CDLineDlg1::OnRADIO7bssolid() 
{
	// TODO: Add your control notification handler code here
  m_BrushStyle=BS_SOLID;
  OnChangeBrushStyle(false);	
}

void CDLineDlg1::OnRADIO8hatched() 
{
	// TODO: Add your control notification handler code here
  
  //m_radio_BDIAGONAL.EnableTypeLib();
  m_BrushStyle=BS_HATCHED;
  OnChangeBrushStyle(true);	
}

void CDLineDlg1::OnDeltaposSPINDDLineWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	OnChangePenStyle();

	*pResult = 0;
}














void CDLineDlg1::OnChangeEDITLineWidth() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
  if(this->IsWindowVisible())
  {
	UpdateData();
	OnChangePenStyle();
  }
}

void CDLineDlg1::OnRadioHs() 
{
	// TODO: Add your control notification handler code here
UpdateData();

switch(m_radio_BrushHatch)
{
    case 0:m_BrushHatch=HS_BDIAGONAL;  break;
    case 1:m_BrushHatch=HS_DIAGCROSS;  break;
    case 2:m_BrushHatch=HS_FDIAGONAL;  break;
    case 3:m_BrushHatch=HS_CROSS;      break;
	case 4:m_BrushHatch=HS_HORIZONTAL; break;
	case 5:m_BrushHatch=HS_VERTICAL;   break;
}

OnChangeBrushStyle(true);

}

HBRUSH CDLineDlg1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
  // brush.CreateSolidBrush(RGB(230,254,255));
   pDC->SetTextColor(RGB(0,100,255));
   pDC->SetBkColor(RGB(118,143,255));
   pDC->SelectObject(&brush);
   pDC->SetBkMode(TRANSPARENT);
  // hbr=(HBRUSH)brush.m_hObject;
   return hbr;
}



void CDLineDlg1::OnSelchangeCOMBOBackgroundMode() 
{
	// TODO: Add your control notification handler code here
m_BKMode=m_Combo_BackgroundMode.GetCurSel()==0?OPAQUE:TRANSPARENT;
this->OnChangePenStyle();
this->OnChangeBrushStyle(true);		
}
