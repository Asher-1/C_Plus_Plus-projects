#include "stdafx.h"
#include"CRoundRectangle.h"
#include"EastDrawView.h"
IMPLEMENT_SERIAL(CRoundRectangle,CUnit,1)
void CRoundRectangle::DrawStatic(CDC*pDC)
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
 pDC->RoundRect(CRect(m_FirstPoint,m_SecondPoint),CPoint(abs(m_FirstPoint.x-m_SecondPoint.x)/3,abs(m_FirstPoint.y-m_SecondPoint.y)/3)); 
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 pDC->SetROP2(oldDrawingMode);

}
CRoundRectangle::CRoundRectangle()
{
  Initial();
  //m_Shape=0;
  m_BrushStyle=BS_HOLLOW;
  m_BrushHatch=HS_CROSS;

}
void CRoundRectangle::Serialize(CArchive &ar)
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
BOOL CRoundRectangle::IsInRgn(CPoint point)
{
 CPoint FirstPoint;
 CPoint SecondPoint;
 FirstPoint.x=m_FirstPoint.x<m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 FirstPoint.y=m_FirstPoint.y<m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 SecondPoint.x=m_FirstPoint.x>m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 SecondPoint.y=m_FirstPoint.y>m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 m_FirstPoint=FirstPoint;
 m_SecondPoint=SecondPoint;
	
	
	
	CRect rgn1;
	rgn1.SetRect(m_FirstPoint.x,m_FirstPoint.y,m_SecondPoint.x,m_SecondPoint.y);
	rgn1.InflateRect(4,4);
    CRect rgn2;
	rgn2.SetRect(m_FirstPoint.x,m_FirstPoint.y,m_SecondPoint.x,m_SecondPoint.y);
    rgn2.DeflateRect(4,4); 
	return rgn1.PtInRect(point)&&!rgn2.PtInRect(point);
	
}
int CRoundRectangle::IsOnMarginPoint(CPoint point)
{

 
 CRect rect(point,point);
 rect.InflateRect(CSize(4,4));
 

 CPoint lpoint=m_FirstPoint;
        //lpoint.Offset(abs(m_FirstPoint.x-m_SecondPoint.x)/1,abs(m_FirstPoint.y-m_SecondPoint.y)/1);
 if(rect.PtInRect(lpoint))
 {
	 m_FoundPoint=lpoint;
	 return 1;
 }      
        lpoint=CPoint(m_SecondPoint.x,m_FirstPoint.y);
	//	lpoint.Offset(-abs(m_FirstPoint.x-m_SecondPoint.x)/1,abs(m_FirstPoint.y-m_SecondPoint.y)/1);
 if(rect.PtInRect(lpoint))
 {
	 m_FoundPoint=lpoint;
	 return 2;
 }      
        lpoint=m_SecondPoint;
        //lpoint.Offset(-abs(m_FirstPoint.x-m_SecondPoint.x)/1,-abs(m_FirstPoint.y-m_SecondPoint.y)/1);
 if(rect.PtInRect(lpoint))
 { 
	 m_FoundPoint=lpoint;
	 return 3;
 }      
        lpoint=CPoint(m_FirstPoint.x,m_SecondPoint.y);
        //lpoint.Offset(abs(m_FirstPoint.x-m_SecondPoint.x)/1,-abs(m_FirstPoint.y-m_SecondPoint.y)/1);
 if(rect.PtInRect(lpoint))
 {
	 m_FoundPoint=lpoint;
	 return 4;
 }
 if(rect.PtInRect(CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2)))
 {
	 m_FoundPoint=CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2);
	 return 5;
 }
  return 0;


	
}

void CRoundRectangle::ShowSelectPoint(CDC *pDC)
{

 CRect rect;
 CBrush brush;
brush.CreateSolidBrush(RGB(0,0,255));
CPen m_pen;
m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
CPen *OldPen=pDC->SelectObject(&m_pen);
int oldBkMode=pDC->SetBkMode(OPAQUE); 
CBrush *OldBrush=pDC->SelectObject(&brush);
int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);

 CPoint lpoint=m_FirstPoint;
        //lpoint.Offset(abs(m_FirstPoint.x-m_SecondPoint.x)/1,abs(m_FirstPoint.y-m_SecondPoint.y)/1);
         rect.SetRect(lpoint,lpoint);
		 rect.InflateRect(3,3);
		 pDC->Rectangle(rect);
        
        lpoint=CPoint(m_SecondPoint.x,m_FirstPoint.y);
	//	lpoint.Offset(-abs(m_FirstPoint.x-m_SecondPoint.x)/1,abs(m_FirstPoint.y-m_SecondPoint.y)/1);
 rect.SetRect(lpoint,lpoint);
		 rect.InflateRect(3,3);
		 pDC->Rectangle(rect);
        
        lpoint=m_SecondPoint;
        //lpoint.Offset(-abs(m_FirstPoint.x-m_SecondPoint.x)/1,-abs(m_FirstPoint.y-m_SecondPoint.y)/1);
 rect.SetRect(lpoint,lpoint);
		 rect.InflateRect(3,3);
		 pDC->Rectangle(rect);
        
        lpoint=CPoint(m_FirstPoint.x,m_SecondPoint.y);
        ///lpoint.Offset(abs(m_FirstPoint.x-m_SecondPoint.x)/1,-abs(m_FirstPoint.y-m_SecondPoint.y)/1);
 rect.SetRect(lpoint,lpoint);
		 rect.InflateRect(3,3);
		 pDC->Rectangle(rect);


		 pDC->SelectObject(OldBrush);
  brush.DeleteObject();
  brush.CreateSolidBrush(RGB(255,0,0));
  pDC->SelectObject(&brush);
    lpoint=CPoint((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2);
     rect.SetRect(lpoint,lpoint);
		 rect.InflateRect(3,3);
		 pDC->Rectangle(rect);


pDC->SelectObject(OldPen);

pDC->SetBkMode(oldBkMode); 
pDC->SelectObject(OldBrush);
pDC->SetROP2(oldDrawingMode);

}

void CRoundRectangle::DrawActive(CDC *pDC,CPoint point)
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
 pDC->RoundRect(CRect(m_FirstPoint,m_SecondPoint),CPoint(abs(m_FirstPoint.x-m_SecondPoint.x)/3,abs(m_FirstPoint.y-m_SecondPoint.y)/3)); 
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);

}

void CRoundRectangle::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
    if(m_HaveFindFirst)
	 {
	  pView->L_iPointCount=IsOnMarginPoint(point);
      if(pView->L_iPointCount!=0)
	  {
	   
		pView->m_bHaveFindSecond=true;
        pView->m_CurrentDrawTool=RoundRectangle_Tool;
        pView->m_CurrentDrawStatus=Change_Status;

	  if(pView->L_iPointCount==1)
	  {
	   pView->m_FirstPoint=m_SecondPoint;
	   pView->m_SecondPoint=point;
	  }
	  if(pView->L_iPointCount==2)
	  {
	  pView->m_FirstPoint=CPoint(m_FirstPoint.x,m_SecondPoint.y);
	  pView->m_SecondPoint=point;
	  }
      if(pView->L_iPointCount==3)
	  {
	   pView->m_FirstPoint=m_FirstPoint;
	   pView->m_SecondPoint=point;
	  }
	  if(pView->L_iPointCount==4)
	  {
	   pView->m_FirstPoint=CPoint(m_SecondPoint.x,m_FirstPoint.y);
	   pView->m_SecondPoint=point;
	  }
	  if(pView->L_iPointCount==5)
	  {
	   pView->m_FirstPoint=m_FirstPoint;
	   pView->m_SecondPoint=m_SecondPoint;
       pView->m_CurrentDrawTool=RoundRectangle_Tool;
       pView->m_CurrentDrawStatus=Drag_Status;
	   
	  }
	 }//******if(L_iPointCount!=0)
	 }//******if(L_pRoundRectangle->m_HaveFindFirst)
    
	if((!pView->m_bHaveFindSecond)&&IsInRgn(point))
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
	        ShowSelectPoint(pDC);
	        m_DrawingMode=pDC->GetROP2();
	        DrawStatic(pDC);
		    DrawOldReferencePoint(pDC,m_FoundPoint);
	    }
		
}

void CRoundRectangle::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
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
