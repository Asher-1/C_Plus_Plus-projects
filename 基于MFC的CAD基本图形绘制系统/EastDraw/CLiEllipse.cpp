#include "stdafx.h"
#include"CLiEllipse.h"
#include"EastDrawView.h"
IMPLEMENT_SERIAL(CLiEllipse,CUnit,1)
void CLiEllipse::DrawStatic(CDC*pDC)
{  
  CPen m_pen;
  CBrush m_brush;
  int oldDrawingMode=pDC->SetROP2(this->m_DrawingMode);
  m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
  CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;

  m_brush.CreateBrushIndirect(&brushlog);
  SetBrushOrg(pDC,&m_brush);
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
 
 int OldBkMode=pDC->SetBkMode(m_BkMode);
 COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
 pDC->Ellipse(CRect(m_FirstPoint,m_SecondPoint)); 
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 pDC->SetROP2(oldDrawingMode);

}
CLiEllipse::CRoundRectangle()
{
  Initial();
  //m_Shape=0;
  m_BrushStyle=BS_HOLLOW;
  m_BrushHatch=HS_CROSS;

}
void CLiEllipse::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
    if(ar.IsStoring())
	{
	  ar<<m_BrushStyle<<m_BkMode<<m_BrushHatch<<m_BrushColor;
	}
	else
	{
	  ar>>m_BrushStyle>>m_BkMode>>m_BrushHatch>>m_BrushColor;
	}
	
}
BOOL CLiEllipse::IsInRgn(CPoint point)
{
 
CPoint FirstPoint;
 CPoint SecondPoint;
 FirstPoint.x=m_FirstPoint.x<m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 FirstPoint.y=m_FirstPoint.y<m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 SecondPoint.x=m_FirstPoint.x>m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 SecondPoint.y=m_FirstPoint.y>m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 m_FirstPoint=FirstPoint;
 m_SecondPoint=SecondPoint;
	
	CRgn rgn1;
	rgn1.CreateEllipticRgn(m_FirstPoint.x-4,m_FirstPoint.y-4,m_SecondPoint.x+4,m_SecondPoint.y+4);
    CRgn rgn2;
	rgn2.CreateEllipticRgn(m_FirstPoint.x+4,m_FirstPoint.y+4,m_SecondPoint.x-4,m_SecondPoint.y-4);

	return rgn1.PtInRegion(point)&&!rgn2.PtInRegion(point);
	
}
int CLiEllipse::IsOnMarginPoint(CPoint point)
{


 CRect rect(point,point);
 rect.InflateRect(CSize(4,4));
 if(rect.PtInRect(CPoint(m_FirstPoint.x+(m_SecondPoint.x-m_FirstPoint.x)/2,m_FirstPoint.y)))
 {   
	 this->m_FoundPoint=CPoint(m_FirstPoint.x+(m_SecondPoint.x-m_FirstPoint.x)/2,m_FirstPoint.y);
	 return 1;
 }
 if(rect.PtInRect(CPoint(m_SecondPoint.x,m_FirstPoint.y+(m_SecondPoint.y-m_FirstPoint.y)/2)))
 {
	 this->m_FoundPoint=CPoint(m_SecondPoint.x,m_FirstPoint.y+(m_SecondPoint.y-m_FirstPoint.y)/2);
	 return 2;
 }
 if(rect.PtInRect(CPoint(m_SecondPoint.x+(m_FirstPoint.x-m_SecondPoint.x)/2,m_SecondPoint.y)))
 {    
	 this->m_FoundPoint=CPoint(m_SecondPoint.x+(m_FirstPoint.x-m_SecondPoint.x)/2,m_SecondPoint.y);
     return 3;
 }
 if(rect.PtInRect(CPoint(m_FirstPoint.x,m_SecondPoint.y+(m_FirstPoint.y-m_SecondPoint.y)/2)))
 {
	 this->m_FoundPoint=CPoint(m_FirstPoint.x,m_SecondPoint.y+(m_FirstPoint.y-m_SecondPoint.y)/2);
     return 4;
 }
 if(rect.PtInRect(CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2)))
 {   
	 this->m_FoundPoint=CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2);
     return 5;
 }
 
 return 0;
 
}

void CLiEllipse::ShowSelectPoint(CDC *pDC)
{
CBrush brush;
brush.CreateSolidBrush(RGB(0,0,255));
CPen m_pen;
m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
CPen *OldPen=pDC->SelectObject(&m_pen);
int oldBkMode=pDC->SetBkMode(OPAQUE); 
CBrush *OldBrush=pDC->SelectObject(&brush);
int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
CRect rect;
rect.SetRect(CPoint(m_FirstPoint.x+(m_SecondPoint.x-m_FirstPoint.x)/2,m_FirstPoint.y),CPoint(m_FirstPoint.x+(m_SecondPoint.x-m_FirstPoint.x)/2,m_FirstPoint.y));
rect.InflateRect(3,3);
pDC->Rectangle(rect);
rect.SetRect(CPoint(m_SecondPoint.x,m_FirstPoint.y+(m_SecondPoint.y-m_FirstPoint.y)/2),CPoint(m_SecondPoint.x,m_FirstPoint.y+(m_SecondPoint.y-m_FirstPoint.y)/2));
rect.InflateRect(3,3);
pDC->Rectangle(rect);
rect.SetRect(CPoint(m_SecondPoint.x+(m_FirstPoint.x-m_SecondPoint.x)/2,m_SecondPoint.y),CPoint(m_SecondPoint.x+(m_FirstPoint.x-m_SecondPoint.x)/2,m_SecondPoint.y));
rect.InflateRect(3,3);
pDC->Rectangle(rect);
rect.SetRect(CPoint(m_FirstPoint.x,m_SecondPoint.y+(m_FirstPoint.y-m_SecondPoint.y)/2),CPoint(m_FirstPoint.x,m_SecondPoint.y+(m_FirstPoint.y-m_SecondPoint.y)/2));
rect.InflateRect(3,3);
pDC->Rectangle(rect);
rect.SetRect(CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2),CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2));
rect.InflateRect(3,3);
 pDC->SelectObject(OldBrush);
 brush.DeleteObject();
 brush.CreateSolidBrush(RGB(255,0,0));
 pDC->SelectObject(&brush);
pDC->Rectangle(rect);
pDC->SelectObject(OldPen);
pDC->SetBkMode(oldBkMode); 
pDC->SelectObject(OldBrush);
pDC->SetROP2(oldDrawingMode);

}

void CLiEllipse::DrawActive(CDC *pDC,CPoint point)
{
  CPen m_pen;
  CBrush m_brush;
  
  m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
  CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;

  m_brush.CreateBrushIndirect(&brushlog);
  SetBrushOrg(pDC,&m_brush);
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
 
 int OldBkMode=pDC->SetBkMode(m_BkMode);
 COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
 pDC->Ellipse(CRect(m_FirstPoint,m_SecondPoint)); 
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 
}

void CLiEllipse::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
if(m_HaveFindFirst)
   {
   pView->L_iPointCount=IsOnMarginPoint(point);
	if(pView->L_iPointCount!=0)
	{
		pView->m_bHaveFindSecond=true;
        pView->m_CurrentDrawTool=LiEllipse_Tool;
        pView->m_CurrentDrawStatus=Change_Status;
	  
	switch(pView->L_iPointCount)
	{
	case 1:
	   pView->m_FirstPoint=m_SecondPoint;
	   pView->m_SecondPoint=m_FirstPoint;
	  break;
	case 2:
	  
	  pView->m_FirstPoint=m_FirstPoint;
	  pView->m_SecondPoint=m_SecondPoint;
	  break;
    case 3:
	  
	   pView->m_FirstPoint=m_FirstPoint;
	   pView->m_SecondPoint=m_SecondPoint;
	   break;
	  
	case 4:
	   
		pView->m_FirstPoint=m_SecondPoint;
	    pView->m_SecondPoint=m_FirstPoint;
	   break;
	  
	case 5:
	   pView->m_FirstPoint=m_FirstPoint;
	   pView->m_SecondPoint=m_SecondPoint;
       pView->m_CurrentDrawTool=LiEllipse_Tool;
       pView->m_CurrentDrawStatus=Drag_Status;
	  break;
	}
	 }//******if(L_iPointCount!=0)
   }//*************if(L_pLiEllipse->m_HaveFindFirst)
   
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
   
   if(pView->m_bHaveFindSecond)
		{   
	        pView->m_pCurrentUnit=this;
	        m_DrawingMode=pDC->GetROP2();
	        DrawStatic(pDC);
			ShowSelectPoint(pDC);
		    DrawOldReferencePoint(pDC,m_FoundPoint);
	    }
		
}

void CLiEllipse::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
    if(pView->m_CurrentDrawStatus==Drag_Status)
	   {
		m_FirstPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
        m_SecondPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);   
		}  
	else{
	     
		  this->DrawActive(pDC,point);
	      pView->m_SecondPoint=point;
	      m_SecondPoint=pView->m_SecondPoint;
		  this->DrawActive(pDC,point);
		}
}
