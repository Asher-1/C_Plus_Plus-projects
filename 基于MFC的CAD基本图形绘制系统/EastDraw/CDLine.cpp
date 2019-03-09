#include "stdafx.h"
#include"CDLine.h"
#include"EastDrawView.h"
IMPLEMENT_SERIAL(CDLine,CUnit,1)
void CDLine::DrawStatic(CDC*pDC)
{
 
 CPen m_pen;
 m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
 CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
 int oldDrawingMode=pDC->SetROP2(m_DrawingMode);
 COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
 int OldBkMode=pDC->SetBkMode(m_BkMode);

 pDC->MoveTo(m_FirstPoint);
 pDC->LineTo(m_SecondPoint);
 pDC->SelectObject(pen);
 pDC->SetROP2(oldDrawingMode);
 pDC->SetBkMode(m_BkMode);
 pDC->SetBkColor(OldColor);
}

CDLine::CDLine()
{ 
	CUnit::Initial();
   

}

CRgn* CDLine::GetRgn()
{
  if(m_Rgn==NULL)
  
   m_Rgn=new CRgn; 
   else m_Rgn->DeleteObject();
   {
   CPoint point[4];
   CPoint FirstPoint;
   CPoint SecondPoint;
  if((m_FirstPoint.x-m_SecondPoint.x)*(m_FirstPoint.y-m_SecondPoint.y)<0)
  {
   if(m_FirstPoint.x>m_SecondPoint.x)
   { 
	   
	   FirstPoint=m_SecondPoint;
       SecondPoint=m_FirstPoint;   
   }
   else
   {
       FirstPoint=m_FirstPoint;
       SecondPoint=m_SecondPoint;
   
   }
   FirstPoint.Offset(-4-m_PenWidth,4+m_PenWidth);
   SecondPoint.Offset(4+m_PenWidth,4+m_PenWidth);
  point[0].x=FirstPoint.x-3-m_PenWidth;
  point[0].y=FirstPoint.y-4-m_PenWidth;

  point[1].x=FirstPoint.x+3+m_PenWidth;
  point[1].y=FirstPoint.y+4+m_PenWidth;

  point[2].x=SecondPoint.x+3+m_PenWidth;
  point[2].y=SecondPoint.y+4+m_PenWidth;

  point[3].x=SecondPoint.x-3-m_PenWidth;
  point[3].y=SecondPoint.y-4-m_PenWidth;
  
  }

  if((m_FirstPoint.x-m_SecondPoint.x)*(m_FirstPoint.y-m_SecondPoint.y)>0)
  {
   if(m_FirstPoint.x>m_SecondPoint.x)
   { 
	   
	   FirstPoint=m_FirstPoint;
       SecondPoint=m_SecondPoint;   
   }

   else
   {
	   FirstPoint=m_SecondPoint;
       SecondPoint=m_FirstPoint; 
   }
    FirstPoint.Offset(-4-m_PenWidth,-4-m_PenWidth);
	SecondPoint.Offset(4+m_PenWidth,4+m_PenWidth);
  point[0].x=FirstPoint.x+3+m_PenWidth;
  point[0].y=FirstPoint.y-4-m_PenWidth;

  point[1].x=FirstPoint.x-3-m_PenWidth;
  point[1].y=FirstPoint.y+4+m_PenWidth;

  point[2].x=SecondPoint.x-3-m_PenWidth;
  point[2].y=SecondPoint.y+4+m_PenWidth;

  point[3].x=SecondPoint.x+3+m_PenWidth;
  point[3].y=SecondPoint.y-4-m_PenWidth;
  
  }

if((m_FirstPoint.x-m_SecondPoint.x)==0)
  {
  point[0].x=m_FirstPoint.x-3-m_PenWidth;
  point[0].y=m_FirstPoint.y-4-m_PenWidth;

  point[1].x=m_FirstPoint.x+3+m_PenWidth;
  point[1].y=m_FirstPoint.y-4-m_PenWidth;

  point[2].x=m_SecondPoint.x+3+m_PenWidth;
  point[2].y=m_SecondPoint.y+4+m_PenWidth;

  point[3].x=m_SecondPoint.x-3-m_PenWidth;
  point[3].y=m_SecondPoint.y+4+m_PenWidth;
  
  }

if((m_FirstPoint.y-m_SecondPoint.y)==0)
  {
  point[0].x=m_FirstPoint.x-4-m_PenWidth;
  point[0].y=m_FirstPoint.y-3-m_PenWidth;

  point[1].x=m_FirstPoint.x-4-m_PenWidth;
  point[1].y=m_FirstPoint.y+3+m_PenWidth;

  point[2].x=m_SecondPoint.x+4+m_PenWidth;
  point[2].y=m_SecondPoint.y+3+m_PenWidth;

  point[3].x=m_SecondPoint.x+4+m_PenWidth;
  point[3].y=m_SecondPoint.y-3-m_PenWidth;
  
  }
  m_Rgn->CreatePolygonRgn(point,4,ALTERNATE);
  }
  return m_Rgn;
}

void CDLine::Initial()
{
  m_FirstPoint=0;
  m_SecondPoint=0;
  m_PenColor=RGB(0,0,0);
  m_PenWidth=1;
  m_PenStyle=PS_SOLID;
  
 



}

int CDLine::IsOnMarginPoint(CPoint point)
{
        CRect L_Rect1;
		CRect L_Rect2;
		CRect L_Rect3;
		L_Rect1.SetRect(m_FirstPoint,m_FirstPoint);
        L_Rect2.SetRect(m_SecondPoint,m_SecondPoint);
		L_Rect3.SetRect((m_SecondPoint.x+m_FirstPoint.x)/2,(m_SecondPoint.y+m_FirstPoint.y)/2,(m_SecondPoint.x+m_FirstPoint.x)/2,(m_SecondPoint.y+m_FirstPoint.y)/2);
        L_Rect1.InflateRect(5+m_PenWidth,5+m_PenWidth);
		L_Rect2.InflateRect(5+m_PenWidth,5+m_PenWidth);
		L_Rect3.InflateRect(5+m_PenWidth,5+m_PenWidth); 
		if(L_Rect1.PtInRect(point))
		{   
		    m_FoundPoint=m_FirstPoint;
			return 1;
		}
		 if(L_Rect2.PtInRect(point))
		 {   
			 m_FoundPoint=m_SecondPoint;
			 return 2;
		 }
		 if(L_Rect3.PtInRect(point))
		 {
			 m_FoundPoint=CPoint((m_SecondPoint.x+m_FirstPoint.x)/2,(m_SecondPoint.y+m_FirstPoint.y)/2);
			 return 3;
		 }
		 
        return 0;

}

void CDLine::Circumrotate(CPoint first,CPoint second)
{
	double k;
	int x;
if(m_FirstPoint.x-m_SecondPoint.x==0)
      k=99999999999;
else  k=double(first.y-second.y)/(first.x-second.x);


   x=(k*(first.y+second.y)+(first.x+second.x)+(k*k-1)*m_FirstPoint.x-2*k*m_FirstPoint.y)/(k*k+1);
   m_FirstPoint.y=k*(x-this->m_FirstPoint.x)+this->m_FirstPoint.y;
   m_FirstPoint.x=x;
   x=(k*(first.y+second.y)+(first.x+second.x)+(k*k-1)*m_SecondPoint.x-2*k*m_SecondPoint.y)/(k*k+1);
   m_SecondPoint.y=k*(x-this->m_SecondPoint.x)+this->m_SecondPoint.y;
   m_SecondPoint.x=x;
 }



void CDLine::ShowSelectPoint(CDC*pDC)
{
 
 CBrush brush;
brush.CreateSolidBrush(RGB(0,0,255));
CPen m_pen;
m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
CPen *OldPen=pDC->SelectObject(&m_pen);
int oldBkMode=pDC->SetBkMode(OPAQUE); 
CBrush *OldBrush=pDC->SelectObject(&brush);
int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
 CRect rect(m_FirstPoint,m_FirstPoint);
 rect.InflateRect(3,3);
 pDC->Rectangle(rect);
 
 rect=CRect(m_SecondPoint,m_SecondPoint);
 rect.InflateRect(3,3);
 pDC->Rectangle(rect);
 pDC->SelectObject(OldBrush);
 brush.DeleteObject();
 brush.CreateSolidBrush(RGB(255,0,0));
 OldBrush=pDC->SelectObject(&brush);
 rect=CRect((m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2,(m_FirstPoint.x+m_SecondPoint.x)/2,(m_FirstPoint.y+m_SecondPoint.y)/2);
 rect.InflateRect(3,3);
 pDC->Rectangle(rect);

 

pDC->SelectObject(OldPen);
pDC->SetBkMode(oldBkMode); 
pDC->SelectObject(OldBrush);
pDC->SetROP2(oldDrawingMode);
}

BOOL CDLine::IsInRgn(CPoint point)
{
   this->GetRgn();
   return(m_Rgn->PtInRegion(point));
}

void CDLine::DrawActive(CDC *pDC,CPoint point)
{
 CPen m_pen;
 m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
 CPen* pen=(CPen*)pDC->SelectObject(&m_pen);

 COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
 int OldBkMode=pDC->SetBkMode(m_BkMode);

 pDC->MoveTo(m_FirstPoint);
 pDC->LineTo(m_SecondPoint);
 pDC->SelectObject(pen);

 pDC->SetBkMode(m_BkMode);
 pDC->SetBkColor(OldColor);
}

void CDLine::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
      if(m_HaveFindFirst)
	   {
	   pView->L_iPointCount=IsOnMarginPoint(point);
	    if(pView->L_iPointCount==1)
		 { 
		   pView->m_bHaveFindSecond=true;
		   pView->m_CurrentDrawTool=DLine_Tool;
		   pView->m_CurrentDrawStatus=Change_Status;
		   pView->m_FirstPoint=m_SecondPoint;
		   pView->m_SecondPoint=point;
		 }
         if(pView->L_iPointCount==2)
		 {   
			pView->m_bHaveFindSecond=true;
			pView->m_CurrentDrawTool=DLine_Tool;
		    pView->m_CurrentDrawStatus=Change_Status;
			pView->m_FirstPoint=m_FirstPoint;
			pView->m_SecondPoint=point;
		 }
		 if(pView->L_iPointCount==3)
		 {
		    pView->m_bHaveFindSecond=true;
            pView->m_CurrentDrawTool=DLine_Tool;
			pView->m_CurrentDrawStatus=Drag_Status;
			pView->m_FirstPoint=m_FirstPoint;
			pView->m_SecondPoint=m_SecondPoint;
		 }
	   }//**********if(L_pDLine->m_HaveFindFirst)
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
			  this->m_HaveFindFirst=false;
			  pView->m_bHaveFindFirst=true;
			  this->ShowSelectPoint(pDC);
			 }
		 }
	
        if(pView->m_bHaveFindSecond)
		{   
	        pView->m_pCurrentUnit=this;
		    m_DrawingMode=pDC->GetROP2();
			ShowSelectPoint(pDC);
		    DrawStatic(pDC);
			DrawOldReferencePoint(pDC,m_FoundPoint);
		}
		}

void CDLine::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
    if(pView->m_CurrentDrawStatus==Drag_Status)
	{
	m_FirstPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
    m_SecondPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
	}
	else
	{
	  pDC->SetROP2(R2_NOTXORPEN);
	  //m_DrawingMode=pDC->GetROP2();
	  this->DrawActive(pDC,point);
      pView->m_SecondPoint=point;
	  //m_pCurrentUnit->m_FirstPoint=m_FirstPoint;
	  m_SecondPoint=pView->m_SecondPoint;
      this->DrawActive(pDC,point);
	}
	}
