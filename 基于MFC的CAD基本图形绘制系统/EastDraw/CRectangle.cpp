#include "stdafx.h"
#include "CRectangle.h"
#include"EastDrawView.h"
IMPLEMENT_SERIAL(CRectangle,CUnit,1)
void CRectangle::DrawStatic(CDC*pDC)
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
 
 
 pDC->Rectangle(CRect(m_FirstPoint,m_SecondPoint)); 
 
 
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 pDC->SetROP2(oldDrawingMode);
}

void CRectangle::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
    if(ar.IsStoring())
	{
	  ar<<m_BrushStyle<<m_BrushHatch<<m_BrushColor;
	}
	else
	{
	  ar>>m_BrushStyle>>m_BrushHatch>>m_BrushColor;
	}
	
}



CRectangle::CRectangle()
{
  Initial();
 
  m_BrushStyle=BS_HOLLOW;
  m_BrushHatch=HS_CROSS;

}

int CRectangle::IsOnMarginPoint(CPoint point)
{
 /*CPoint FirstPoint;
 CPoint SecondPoint;
 FirstPoint.x=m_FirstPoint.x<m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 FirstPoint.y=m_FirstPoint.y<m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 SecondPoint.x=m_FirstPoint.x>m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 SecondPoint.y=m_FirstPoint.y>m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 m_FirstPoint=FirstPoint;
 m_SecondPoint=SecondPoint;
 */
 CRect rect(point,point);
 rect.InflateRect(CSize(10,10));
 if(rect.PtInRect(m_FirstPoint))
 {   
	 m_FoundPoint=m_FirstPoint;
	 return 1;
 }
 if(rect.PtInRect(CPoint(m_SecondPoint.x,m_FirstPoint.y)))
 {   m_FoundPoint=(CPoint(m_SecondPoint.x,m_FirstPoint.y));
	 return 2;
 }
 if(rect.PtInRect(m_SecondPoint))
 {	 
	 m_FoundPoint=m_SecondPoint;
	 return 3;
 }
 if(rect.PtInRect(CPoint(m_FirstPoint.x,m_SecondPoint.y)))
 {   m_FoundPoint=CPoint(m_FirstPoint.x,m_SecondPoint.y);
	 return 4;
 }
 if(rect.PtInRect(CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2)))
 {  
     m_FoundPoint=CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2);
	 return 5;
 }
  return 0;

}

BOOL CRectangle::IsInRgn(CPoint point)
{
 CPoint FirstPoint;
 CPoint SecondPoint;
 FirstPoint.x=m_FirstPoint.x<m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 FirstPoint.y=m_FirstPoint.y<m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 SecondPoint.x=m_FirstPoint.x>m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 SecondPoint.y=m_FirstPoint.y>m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 m_FirstPoint=FirstPoint;
 m_SecondPoint=SecondPoint;
 
  CRect rect1(m_FirstPoint,m_SecondPoint);
  rect1.InflateRect(4,4);
  CRect rect2(m_FirstPoint,m_SecondPoint);
  rect2.DeflateRect(4,4);
  return (rect1.PtInRect(point)&&!rect2.PtInRect(point));

}

void CRectangle::ShowSelectPoint(CDC *pDC)
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

rect.SetRect(this->m_FirstPoint,this->m_FirstPoint);
rect.InflateRect(3,3);
pDC->Rectangle(rect);

rect.SetRect(this->m_SecondPoint,this->m_SecondPoint);
rect.InflateRect(3,3);
pDC->Rectangle(rect);

rect.SetRect(this->m_FirstPoint.x,this->m_SecondPoint.y,this->m_FirstPoint.x,this->m_SecondPoint.y);
rect.InflateRect(3,3);
pDC->Rectangle(rect);

rect.SetRect(this->m_SecondPoint.x,this->m_FirstPoint.y,this->m_SecondPoint.x,this->m_FirstPoint.y);
rect.InflateRect(3,3);
pDC->Rectangle(rect);

  pDC->SelectObject(OldBrush);
  brush.DeleteObject();
  brush.CreateSolidBrush(RGB(255,0,0));
  pDC->SelectObject(&brush);
rect.SetRect((this->m_SecondPoint.x+this->m_FirstPoint.x)/2,(this->m_SecondPoint.y+this->m_FirstPoint.y)/2,(this->m_SecondPoint.x+this->m_FirstPoint.x)/2,(this->m_SecondPoint.y+this->m_FirstPoint.y)/2);
rect.InflateRect(3,3);
pDC->Rectangle(rect);
pDC->SelectObject(OldPen);
pDC->SetBkMode(oldBkMode); 
pDC->SelectObject(OldBrush);
pDC->SetROP2(oldDrawingMode);



}

void CRectangle::DrawActive(CDC *pDC,CPoint point)
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
 
 
 pDC->Rectangle(CRect(m_FirstPoint,m_SecondPoint)); 
 
 
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 
}

void CRectangle::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
    if(m_HaveFindFirst)
	{
	pView->L_iPointCount=IsOnMarginPoint(point);
	if(pView->L_iPointCount!=0)
	{
		pView->m_bHaveFindSecond=true;
        pView->m_CurrentDrawTool=Rectangle_Tool;
        pView->m_CurrentDrawStatus=Change_Status;
	  switch(pView->L_iPointCount)
	  {
	  case 1:
	   pView->m_FirstPoint=m_SecondPoint;
	   pView->m_SecondPoint=point;
	  break;
	  case 2:
	  pView->m_FirstPoint=CPoint(m_FirstPoint.x,m_SecondPoint.y);
	  pView->m_SecondPoint=point;
	  break;
	  case 3:
	   pView->m_FirstPoint=m_FirstPoint;
	   pView->m_SecondPoint=point;
	  break;
	  case 4:
	   pView->m_FirstPoint=CPoint(m_SecondPoint.x,m_FirstPoint.y);
	   pView->m_SecondPoint=point;
	  break;
	  case 5:
	  pView->m_FirstPoint=m_FirstPoint;
	  pView->m_SecondPoint=m_SecondPoint;
      pView->m_CurrentDrawTool=Rectangle_Tool;
      pView->m_CurrentDrawStatus=Drag_Status;
	  break;
	  }
	 }//******if(L_iPointCount!=0)
	}//********if(L_pRectangle->m_HaveFindFirst)
   
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
			ShowSelectPoint(pDC);
			DrawOldReferencePoint(pDC,m_FoundPoint);
	        DrawStatic(pDC);
	    }

}

void CRectangle::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
     if(pView->m_CurrentDrawStatus==Drag_Status)
	   {
	    m_FirstPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
       m_SecondPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);   
	   }
	   
	 else
	   {
	      
		  this->DrawActive(pDC,point);
	      pView->m_SecondPoint=point;
	      m_SecondPoint=pView->m_SecondPoint;
		  
		  this->DrawActive(pDC,point);

       }

}
