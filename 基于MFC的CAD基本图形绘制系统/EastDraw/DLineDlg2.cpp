// DLineDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "EastDraw.h"
#include "DLineDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLineDlg2 property page

IMPLEMENT_DYNCREATE(CDLineDlg2, CPropertyPage)

#include "CUnit.h"
#include <afxdlgs.h>
CDLineDlg2::CDLineDlg2() : CPropertyPage(CDLineDlg2::IDD)
{
	//{{AFX_DATA_INIT(CDLineDlg2)
	m_Blue = 0;
	m_Green = 0;
	m_Red = 0;
	//}}AFX_DATA_INIT
m_Color=RGB(0,0,0);
m_PenColor=RGB(0,0,0);
m_BrushColor=RGB(255,255,255);
m_BackgroundColor=RGB(255,255,255);

}

CDLineDlg2::~CDLineDlg2()
{
}

void CDLineDlg2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLineDlg2)
	DDX_Control(pDX, IDC_CHECK_Pen, m_Check_Pen);
	DDX_Control(pDX, IDC_CHECK_Brush, m_Check_Brush);
	DDX_Control(pDX, IDC_CHECK_Background, m_Check_Background);
	DDX_Control(pDX, IDC_ShowBrushColor, m_Static_ShowBrushColor);
	DDX_Control(pDX, IDC_ShowBackground, m_Static_ShowBackgroundColor);
	DDX_Control(pDX, IDC_ShowPenColor, m_Static_ShowPenColor);
	DDX_Control(pDX, IDC_SPIN_Red, m_Spin_Red);
	DDX_Control(pDX, IDC_SPIN_Green, m_Spin_Green);
	DDX_Control(pDX, IDC_SPIN_Blue, m_Spin_Blue);
	DDX_Control(pDX, IDC_EDIT_Red, m_Edit_Red);
	DDX_Control(pDX, IDC_EDIT_Green, m_Edit_Green);
	DDX_Control(pDX, IDC_EDIT_Blue, m_Edit_Blue);
	DDX_Control(pDX, IDC_SLIDER_Red, m_Slider_Red);
	DDX_Control(pDX, IDC_SLIDER_Green, m_Slider_Green);
	DDX_Control(pDX, IDC_SLIDER_Blue, m_Slider_Blue);
	DDX_Control(pDX, IDC_STATIC_ShowColor, m_CStatic_ShowColor);
	DDX_Text(pDX, IDC_EDIT_Blue, m_Blue);
	DDX_Text(pDX, IDC_EDIT_Green, m_Green);
	DDX_Text(pDX, IDC_EDIT_Red, m_Red);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLineDlg2, CPropertyPage)
	//{{AFX_MSG_MAP(CDLineDlg2)
	ON_BN_CLICKED(IDC_STATIC_Black, OnSTATICBlack)
	ON_BN_CLICKED(IDC_STATIC_Red, OnSTATICRed)
	ON_BN_CLICKED(IDC_STATIC_DarkGray, OnSTATICDarkGray)
	ON_BN_CLICKED(IDC_STATIC_LightRray, OnSTATICLightRray)
	ON_BN_CLICKED(IDC_STATIC_White, OnSTATICWhite)
	ON_BN_CLICKED(IDC_STATIC_DrakBlue, OnSTATICDrakBlue)
	ON_BN_CLICKED(IDC_STATIC_Blue, OnSTATICBlue)
	ON_BN_CLICKED(IDC_STATIC_Green, OnSTATICGreen)
	ON_BN_CLICKED(IDC_STATIC_DrakGreen, OnSTATICDrakGreen)
	ON_BN_CLICKED(IDC_STATIC_Yellow, OnSTATICYellow)
	ON_BN_CLICKED(IDC_STATIC_RarkRed, OnSTATICRarkRed)
	ON_BN_CLICKED(IDC_STATIC_BITMAP6, OnStaticBitmap6)
	ON_BN_CLICKED(IDC_STATIC_BITMAP5, OnStaticBitmap5)
	ON_BN_CLICKED(IDC_STATIC_DarkYellow, OnSTATICDarkYellow)
	ON_BN_CLICKED(IDC_STATIC_BITMAP10, OnStaticBitmap10)
	ON_BN_CLICKED(IDC_STATIC_BITMAP9, OnStaticBitmap9)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_Select, OnBUTTONSelect)
	ON_BN_CLICKED(IDC_BUTTON_ColorDlg, OnBUTTONColorDlg)
	ON_BN_CLICKED(IDC_CHECK_Pen, OnCHECKPen)
	ON_BN_CLICKED(IDC_CHECK_Brush, OnCHECKBrush)
	ON_BN_CLICKED(IDC_CHECK_Background, OnCHECKBackground)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLineDlg2 message handlers









void CDLineDlg2::OnSTATICBlack() 
{
	// TODO: Add your control notification handler code here
	
  m_Color=RGB(0,0,0);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }




  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);
  UpdateData(false);
}

void CDLineDlg2::OnSTATICRed() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(255,0,0);	
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }

  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);


}

void CDLineDlg2::OnSTATICDarkGray() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(80,80,80);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }
  
  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
}

void CDLineDlg2::OnSTATICLightRray() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(180,180,180);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }
  

  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
}

void CDLineDlg2::OnSTATICWhite() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(255,255,255);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }
	
  
  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
}

void CDLineDlg2::OnSTATICDrakBlue() 
{
	// TODO: Add your control notification handler code here

  m_Color=RGB(0,0,128);
 CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }



  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
		
}

void CDLineDlg2::OnSTATICBlue() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(0,0,255);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }
  

  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
}

void CDLineDlg2::OnSTATICGreen() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(0,255,0);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }
  

  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
}

void CDLineDlg2::OnSTATICDrakGreen() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(0,128,0);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }
  

  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
}

void CDLineDlg2::OnSTATICYellow() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(255,255,0);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }


  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
}

void CDLineDlg2::OnSTATICRarkRed() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(128,0,0);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }
  


  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);

  UpdateData(false);
}

void CDLineDlg2::OnStaticBitmap6() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(0,255,255);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }


  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);
  UpdateData(false);
}

void CDLineDlg2::OnStaticBitmap5() 
{
	// TODO: Add your control notification handler code here

  m_Color=RGB(0,170,170);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }



  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);
  UpdateData(false);
}

void CDLineDlg2::OnSTATICDarkYellow() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(128,128,0);
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }


  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);
  UpdateData(false);
}

void CDLineDlg2::OnStaticBitmap10() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(255,0,255);	
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }


  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);
  UpdateData(false);
}

void CDLineDlg2::OnStaticBitmap9() 
{
	// TODO: Add your control notification handler code here
  m_Color=RGB(128,0,128);	
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }


  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);
  UpdateData(false);
}

void CDLineDlg2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
  CDC * pDC;
  CRect rect;
  CBrush brush;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);

  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 

  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);

  
  
  
  m_Blue=GetBValue(m_Color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(m_Color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(m_Color);
  m_Slider_Green.SetPos(m_Green);
  
  
  UpdateData(false);
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

BOOL CDLineDlg2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Slider_Blue.SetRange(0,255);
	m_Slider_Blue.SetPos(0);
	m_Slider_Blue.SetTicFreq(26);
    m_Slider_Red.SetRange(0,255);
	m_Slider_Red.SetPos(0);
	m_Slider_Red.SetTicFreq(26);
    m_Slider_Green.SetRange(0,255);
	m_Slider_Green.SetPos(0);
	m_Slider_Green.SetTicFreq(26);

     

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





void CDLineDlg2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
    if(pScrollBar->GetDlgCtrlID()==IDC_SLIDER_Red)
    {
		m_Red=m_Slider_Red.GetPos();
	}

   if(pScrollBar->GetDlgCtrlID()==IDC_SLIDER_Green)
    {
		m_Green=m_Slider_Green.GetPos();
	}
   if(pScrollBar->GetDlgCtrlID()==IDC_SLIDER_Blue)
    {
		m_Blue=m_Slider_Blue.GetPos();
	}
   
  m_Color=RGB(m_Red,m_Green,m_Blue); 
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }

	UpdateData(false);
 

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDLineDlg2::OnBUTTONSelect() 
{
	// TODO: Add your control notification handler code here
  m_Draw_Status=Select_Color_Status;	

}

void CDLineDlg2::OnChange(COLORREF color)
{
  CDC * pDC; 
  CBrush brush;
  CRect rect;
  m_Color=color;
  pDC=m_CStatic_ShowColor.GetDC();
  m_CStatic_ShowColor.GetClientRect(&rect);
  brush.CreateSolidBrush(m_Color);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_CStatic_ShowColor.ReleaseDC(pDC);
  
  
  if(m_Check_Pen.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);
  }
  if(m_Check_Brush.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC); 
  }
  if(m_Check_Background.GetCheck()==BST_CHECKED)
  {
  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);
  }

	

  m_Blue=GetBValue(color);
  m_Slider_Blue.SetPos(m_Blue);
  
  m_Red=GetRValue(color);
  m_Slider_Red.SetPos(m_Red);
  
  m_Green=GetGValue(color);
  m_Slider_Green.SetPos(m_Green);
  UpdateData(false);

}

void CDLineDlg2::OnBUTTONColorDlg() 
{
	// TODO: Add your control notification handler code here

CColorDialog dlg;
if(dlg.DoModal()==IDOK)
{
  
	m_Color=dlg.m_cc.rgbResult;
	this->OnChange(m_Color);
}	

}

void CDLineDlg2::OnCHECKPen() 
{

	// TODO: Add your control notification handler code here
  CDC * pDC; 
  CBrush brush;
  CRect rect;
	
  pDC=m_Static_ShowPenColor.GetDC();
  m_Static_ShowPenColor.GetClientRect(&rect);
  m_PenColor=m_Color;
  brush.CreateSolidBrush(m_PenColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowPenColor.ReleaseDC(pDC);	


}

void CDLineDlg2::OnCHECKBrush() 
{
	// TODO: Add your control notification handler code here
  CDC * pDC; 
  CBrush brush;
  CRect rect;

  pDC=m_Static_ShowBrushColor.GetDC();
  m_Static_ShowBrushColor.GetClientRect(&rect);
  m_BrushColor=m_Color;
  brush.CreateSolidBrush(m_BrushColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBrushColor.ReleaseDC(pDC);

	
}

void CDLineDlg2::OnCHECKBackground() 
{
	// TODO: Add your control notification handler code here
   
  CDC * pDC; 
  CBrush brush;
  CRect rect;

  pDC=m_Static_ShowBackgroundColor.GetDC();
  m_Static_ShowBackgroundColor.GetClientRect(&rect);
  m_BackgroundColor=m_Color;
  brush.CreateSolidBrush(m_BackgroundColor);
  pDC->FillRect(&rect,&brush);
  brush.DeleteObject();
  m_Static_ShowBackgroundColor.ReleaseDC(pDC);

}

HBRUSH CDLineDlg2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
   pDC->SetTextColor(RGB(0,100,255));
   pDC->SetBkColor(RGB(255,200,200));
   
   pDC->SetBkMode(TRANSPARENT);
	
	return hbr;
}


