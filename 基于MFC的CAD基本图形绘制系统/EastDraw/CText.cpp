#include"stdafx.h"
#include"CText.h"
#include "EastDrawView.h"
#include"TextEdit.h"
IMPLEMENT_SERIAL(CText,CUnit,1)
CText::CText()
{
 CUnit::Initial();
 m_Font=new CFont;
 m_Font->CreatePointFont (100,_T("Arial"));
 m_pTextEdit = new CTextEdit;
 m_HaveFinish=false;
}
CText::~CText()
{
delete m_Font;
delete m_pTextEdit;
}
void CText::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
  
    LOGFONT m_lf; 
  
 	
if(ar.IsStoring())
   {
     m_Font->GetLogFont(&m_lf);

    ar<<m_Hight<<m_Width<<m_PositionPoint<<m_String;
	    ar<<m_lf.lfCharSet;
		ar<<m_lf.lfClipPrecision;
		ar<<m_lf.lfEscapement;
		ar<<m_lf.lfHeight;
		ar<<m_lf.lfItalic;
		ar<<m_lf.lfOrientation;
		ar<<m_lf.lfOutPrecision;
		ar<<m_lf.lfPitchAndFamily;
		ar<<m_lf.lfQuality;	
	    ar<<m_lf.lfWidth;
		ar<<m_lf.lfWeight;
		ar<<m_lf.lfUnderline;
		ar<<m_lf.lfStrikeOut;
        CString str=m_lf.lfFaceName;
        ar<<str;
   }
else
   {
       m_HaveFinish=true;
       ar>>m_Hight>>m_Width>>m_PositionPoint>>m_String;

	    ar>>m_lf.lfCharSet;
		ar>>m_lf.lfClipPrecision;
		ar>>m_lf.lfEscapement;
		ar>>m_lf.lfHeight;
		ar>>m_lf.lfItalic;
		ar>>m_lf.lfOrientation;
		ar>>m_lf.lfOutPrecision;
		ar>>m_lf.lfPitchAndFamily;
		ar>>m_lf.lfQuality;	
	    ar>>m_lf.lfWidth;
		ar>>m_lf.lfWeight;
		ar>>m_lf.lfUnderline;
		ar>>m_lf.lfStrikeOut;
		CString str;
		ar>>str;
		::lstrcpy(m_lf.lfFaceName,str);
		m_Font->DeleteObject();
		m_Font->CreateFontIndirect(&m_lf);

		m_bHaveCreat=false;
   }
}

void CText::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
 if(pView->m_CurrentDrawStatus==Draw_Status)
 {
   if(pView->m_LBDownTimes==1&&!m_pTextEdit->m_IsMoving)
   {
	CPoint point1(10,10);
	CPoint point2(200,200);
	m_pTextEdit->Create(WS_VSCROLL|WS_HSCROLL|ES_MULTILINE | ES_WANTRETURN | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
    CRect(point1,point2), pView, 1);
	this->SetTextEditPosition(pDC,pView,point);
	m_pTextEdit->ShowWindow(true);
	pView->ShowFontBar(true);
	pView->SetBoxFont();
	
   }

  if(pView->m_LBDownTimes==2&&!m_pTextEdit->m_IsMoving)
  {
   pView->ShowFontBar(false);
   m_pTextEdit->UpdateData();
   m_pTextEdit->ShowWindow(false);
   m_HaveFinish=true; 
   m_pTextEdit->GetWindowText(m_String);
   pView->m_LBDownTimes=0;
   pView->ExchangeSave();
   
   
  if(this->m_DrawStatus==Change_Status)
   {
    this->m_DrawStatus=Draw_Status;
	pView->m_CurrentDrawStatus=Select_Status;
   }
   
   int nLineCount = m_pTextEdit->GetLineCount();
   CString strText;
   CFont *pOldFond=pDC->SelectObject(m_Font);
   int pOldBkMode=pDC->SetBkMode(this->m_BkMode);
   COLORREF oldBKColor=pDC->SetBkColor(this->m_BackgroundColor);
   COLORREF oldPenColor=pDC->SetTextColor(this->m_PenColor);
   CBrush brush;
   LOGBRUSH brushlog;
   brushlog.lbColor=m_BrushColor;
   brushlog.lbHatch=m_BrushHatch;
   brushlog.lbStyle=m_BrushStyle;
   brush.CreateBrushIndirect(&brushlog);
   CBrush*pOldBrush=pDC->SelectObject(&brush);
   TEXTMETRIC tm;
   pDC->GetTextMetrics(&tm);
   int maxLength=0;
   int maxLineLong;
   
  for(int i=0;i<nLineCount;i++)
  {
   maxLength=maxLength>m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i))?maxLength:m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i));
   m_pTextEdit->GetLine(0,strText.GetBuffer(m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i))),m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i)));
   
   strText.ReleaseBuffer();
   pDC->TextOut(m_PositionPoint.x,m_PositionPoint.y+i*(tm.tmHeight+tm.tmExternalLeading),strText.Left(m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i))));
   
   strText.Empty();
   
  }
  
  this->m_Width=maxLength*(tm.tmMaxCharWidth);
  this->m_Hight=(nLineCount+1)*(tm.tmHeight+tm.tmExternalLeading);
  CRect rect;
  m_pTextEdit->GetWindowRect(&rect);
  m_Width=m_Width>rect.Width()?m_Width:rect.Width();
  m_Hight=m_Hight>rect.Height()?m_Hight:rect.Height();
   
   pDC->SetBkMode(pOldBkMode);
   pDC->SelectObject(pOldFond);
   pDC->SetBkColor(oldBKColor);
   pDC->SetTextColor(oldPenColor);
   pDC->SelectObject(pOldBrush);
  }
 if(pView->m_LBDownTimes==2&&m_pTextEdit->m_IsMoving)
 {
	pView->ShowFontBar(true);
	pView->m_LBDownTimes=1;

	pView->ExchangeSave();
    

	m_PositionPoint=point;
	m_pTextEdit->m_PositionPoint=point;
	CRect rect;
    m_pTextEdit->GetWindowRect(&rect);
    pDC->DPtoLP(&rect);
	pDC->DrawFocusRect(CRect(m_PositionPoint.x+1,m_PositionPoint.y+1,m_PositionPoint.x+rect.Width()-1,m_PositionPoint.y+rect.Height()-1));
	this->SetTextEditPosition(pDC,pView,m_PositionPoint);
	this->m_pTextEdit->m_IsMoving=false;
 }
 
}

 if(pView->m_CurrentDrawStatus==Change_Status)
 {
    pView->m_CurrentDrawStatus=Draw_Status;
	pView->m_LBDownTimes=1;
	
 }//********if(pView->m_CurrentDrawStatus==Exchange_Status)

 if(pView->m_CurrentDrawStatus==Select_Status)
 {
   if(IsOnMarginPoint(point))
   {
    if(m_HaveFindFirst)
    {
	 pView->ShowFontBar(true);
     this->m_HaveFindFirst=false;
	 pView->m_bHaveFindSecond=true;
     this->ShowSelectPoint(pDC);
	 CPoint point=m_PositionPoint;
	 pDC->LPtoDP(&point);
	 
      CRect rect;
      m_pTextEdit->GetWindowRect(&rect);
      m_Width=m_Width>rect.Width()?m_Width:rect.Width();
      m_Hight=m_Hight>rect.Height()?m_Hight:rect.Height();
	 this->m_pTextEdit->MoveWindow(point.x,point.y,m_Width,m_Hight);
     this->m_pTextEdit->ShowWindow(true);
	 this->m_pTextEdit->SetFont(m_Font);
	 pView->m_CurrentDrawTool=Text_Tool;
	 pView->m_CurrentDrawStatus=Change_Status;
	 pView->m_pCurrentUnit=this;
	 pView->m_LBDownTimes=0;
	 m_HaveFinish=false;
	 this->m_DrawStatus=Change_Status;
	 }
   }
    if(!pView->m_bHaveFindSecond&&IsInRgn(point))
	 
	{
	 if(!m_HaveFindFirst)
	 {
		 m_HaveFindFirst=true;
	     pView->m_bHaveFindFirst=true;
	     ShowSelectPoint(pDC);
	 }
	else
	{
	     m_HaveFindFirst=false;
	     pView->m_bHaveFindFirst=true;
	     ShowSelectPoint(pDC);
	}
	
	}
  }//******if(pView->m_CurrentDrawStatus==Select_Status)
 }


void CText::SetTextEditPosition(CDC *pDC, CEastDrawView *pView, CPoint point)
{
  CRect rect;
  m_pTextEdit->GetWindowRect(&rect);
  
 
 
  m_PositionPoint=point;
  m_pTextEdit->m_PositionPoint=point;
  this->m_Width=rect.Width();
  this->m_Hight=rect.Height();
  pDC->LPtoDP(&point);
  m_pTextEdit->m_PositionPoint=point;
  m_pTextEdit->MoveWindow(point.x,point.y,rect.Width(),rect.Height());
}

void CText::SetFont(ENUMLOGFONT *lpelf)
{
 m_pTextEdit->SetMyFont(m_Font);
}

void CText::ChangeFont(ENUMLOGFONT *lpelf)
{
 
 m_Font->DeleteObject();
 m_Font->CreateFontIndirect(&lpelf->elfLogFont);
 SetFont(lpelf);
}

void CText::ChangeFontSize(ENUMLOGFONT *lpelf)
{
 m_Font->DeleteObject();
 m_Font->CreateFontIndirect(&lpelf->elfLogFont);
 SetFont(lpelf);
}

void CText::DrawStatic(CDC *pDC)
{
 if(this->m_HaveFinish)
 {
  CMDIChildWnd*pChild=((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
  CEastDrawView*pView=(CEastDrawView*)pChild->GetActiveView();
  if(!m_bHaveCreat)
  {
   m_pTextEdit->DestroyWindow();
   m_pTextEdit->Create(WS_VSCROLL|WS_HSCROLL|ES_MULTILINE | ES_WANTRETURN | WS_CHILD |WS_TABSTOP | WS_BORDER,
   CRect(m_PositionPoint,CPoint(m_PositionPoint.x+m_Width,m_PositionPoint.y+m_Hight)), pView, 1);
   m_bHaveCreat=true;
  }
 m_pTextEdit->SetWindowText(m_String);
 int nLineCount = m_pTextEdit->GetLineCount();
 CString strText;
 CFont *pOldFond=pDC->SelectObject(m_Font);
 int pOldBkMode=pDC->SetBkMode(this->m_BkMode);
 COLORREF oldBKColor=pDC->SetBkColor(this->m_BackgroundColor);
 COLORREF oldPenColor=pDC->SetTextColor(this->m_PenColor);
 CBrush brush;
 LOGBRUSH brushlog;
 brushlog.lbColor=m_BrushColor;
 brushlog.lbHatch=m_BrushHatch;
 brushlog.lbStyle=m_BrushStyle;
 brush.CreateBrushIndirect(&brushlog);
 CBrush*pOldBrush=pDC->SelectObject(&brush);

 TEXTMETRIC tm;
 pDC->GetTextMetrics(&tm);
 
 for(int i=0;i<nLineCount;i++)
  {
  m_pTextEdit->GetLine(i,strText.GetBuffer(m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i))),m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i)));
  strText.ReleaseBuffer();
  pDC->TextOut(m_PositionPoint.x,m_PositionPoint.y+i*(tm.tmHeight+tm.tmExternalLeading),strText.Left(m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i))));
  strText.Empty();
  }
  pDC->SetBkMode(pOldBkMode);
  pDC->SelectObject(pOldFond);
  pDC->SetBkColor(oldBKColor);
  pDC->SetTextColor(oldPenColor);
  pDC->SelectObject(pOldBrush);
 }
 
}

void CText::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
  CRect rect;
  m_pTextEdit->GetWindowRect(&rect);
  pDC->DPtoLP(&rect);
  CPen pen;
  pen.CreatePen(PS_SOLID,2,RGB(255,0,0));
  CPen*pOldPen=pDC->SelectObject(&pen);
  if(m_pTextEdit->m_IsMoving)
  {
    //this->SetTextEditPosition(pDC,pView,point);
    //pDC->Rectangle(m_PositionPoint.x+1,m_PositionPoint.y+1,m_PositionPoint.x+rect.Width()-1,m_PositionPoint.y+rect.Height()-1);
    pDC->DrawFocusRect(CRect(m_PositionPoint.x+1,m_PositionPoint.y+1,m_PositionPoint.x+rect.Width()-1,m_PositionPoint.y+rect.Height()-1));
	  m_PositionPoint=point;
//	pDC->Rectangle(m_PositionPoint.x+1,m_PositionPoint.y+1,m_PositionPoint.x+rect.Width()-1,m_PositionPoint.y+rect.Height()-1);
   pDC->DrawFocusRect(CRect(m_PositionPoint.x+1,m_PositionPoint.y+1,m_PositionPoint.x+rect.Width()-1,m_PositionPoint.y+rect.Height()-1));
  }
  pDC->SelectObject(pOldPen);
}

void CText::OnMenuitemOk(CDC *pDC, CEastDrawView *pView)
{
  if(m_pTextEdit->m_IsMoving)
  {
    CRect rect;
    m_pTextEdit->GetWindowRect(&rect);
    pDC->DPtoLP(&rect);
	pDC->DrawFocusRect(CRect(m_PositionPoint.x+1,m_PositionPoint.y+1,m_PositionPoint.x+rect.Width()-1,m_PositionPoint.y+rect.Height()-1));
	SetTextEditPosition(pDC,pView,m_PositionPoint);
	m_pTextEdit->m_IsMoving=false;
  }

}

BOOL CText::IsInRgn(CPoint point)
{
CRect rect1;
CRect rect2;
rect1.SetRect(m_PositionPoint.x-5,m_PositionPoint.y-5,m_PositionPoint.x+m_Width+5,m_PositionPoint.y+m_Hight+5);
rect2.SetRect(m_PositionPoint.x+10,m_PositionPoint.y+10,m_PositionPoint.x+m_Width-10,m_PositionPoint.y+m_Hight-10);
return rect1.PtInRect(point)&&!rect2.PtInRect(point);
}

int CText::IsOnMarginPoint(CPoint point)
{
CRect rect1;
CRect rect2;
rect1.SetRect(m_PositionPoint.x-3,m_PositionPoint.y-3,m_PositionPoint.x+m_Width+3,m_PositionPoint.y+m_Hight+3);
rect2.SetRect(m_PositionPoint.x+3,m_PositionPoint.y+3,m_PositionPoint.x+m_Width-3,m_PositionPoint.y+m_Hight-3);
if(rect1.PtInRect(point)&&!rect2.PtInRect(point))
return 1;
return 0;

}

void CText::ShowSelectPoint(CDC *pDC)
{

CPen m_pen;
m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
CPen *OldPen=pDC->SelectObject(&m_pen);
int oldBkMode=pDC->SetBkMode(TRANSPARENT);
int OldRop=pDC->SetROP2(R2_NOTXORPEN);   

CRect rect;
rect.SetRect(m_PositionPoint.x,m_PositionPoint.y,m_PositionPoint.x+m_Width,m_PositionPoint.y+m_Hight);

pDC->Rectangle(rect);
pDC->SelectObject(OldPen);
pDC->SetBkMode(oldBkMode);
pDC->SetROP2(OldRop);

}
