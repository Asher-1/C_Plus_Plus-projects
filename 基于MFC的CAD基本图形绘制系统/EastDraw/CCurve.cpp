#include "stdafx.h"
#include "CCurve.h"
#include"EastDrawView.h"
IMPLEMENT_SERIAL(CCurve,CObject,1)
void CCurve::DrawStatic(CDC*pDC)
{
 CPen m_pen;
 m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
 CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
 int oldDrawingMode=pDC->SetROP2(m_DrawingMode);
 
    
m_FirstPoint=m_PointList->GetAt(0);
 pDC->MoveTo(m_FirstPoint);
 for(int i=0;i<m_PointList->GetSize();i++)
 {
   pDC->LineTo(m_PointList->GetAt(i));
   pDC->LineTo(m_PointList->GetAt(i));
 }
   pDC->LineTo(m_PointList->GetAt(i-1));
   pDC->SelectObject(pen);
   pDC->SetROP2(oldDrawingMode);
  
}

int CCurve::IsOnMarginPoint(CPoint point)
{
  CRect rect;
  for(int i=0;i<m_PointList->GetSize();i=i+10)
  {
   rect=CRect(m_PointList->GetAt(i),m_PointList->GetAt(i));
   rect.InflateRect(4,4);
   if(rect.PtInRect(point))
   {
	   m_FoundPoint=m_PointList->GetAt(i);
	   return i+i;
   }
  }

   return -1;
}

void CCurve::Initial()
{
 CUnit::Initial();
 
 m_PointList=new CArray<CPoint,CPoint>;
}

void CCurve::Serialize(CArchive &ar)
{
if(ar.IsStoring())
  {
   ar<<m_PenColor<<m_PenStyle<<m_PenWidth;
  }
  else
  {
  ar>>m_PenColor>>m_PenStyle>>m_PenWidth;
 
  }
   m_PointList->Serialize(ar);
}

CCurve::CCurve()
{
Initial();
}

void CCurve::DrawMask(CDC*pDC,CPoint first,CPoint second)
{
 CPen m_pen;
 m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
 CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
 
 pDC->MoveTo(first);
 pDC->LineTo(second);
 pDC->SelectObject(pen);
}

BOOL CCurve::IsInRgn(CPoint point)
{
 CRect rect;
  for(int i=0;i<m_PointList->GetSize();i++)
  {
   rect=CRect(m_PointList->GetAt(i),m_PointList->GetAt(i));
   rect.InflateRect(9,9);
   if(rect.PtInRect(point))
	   return i;
  }
return -1;

}

void CCurve::ShowSelectPoint(CDC *pDC)
{
  CRect rect;
  CBrush brush;
brush.CreateSolidBrush(RGB(255,0,0));
CPen m_pen;
m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
CPen *OldPen=pDC->SelectObject(&m_pen);
int oldBkMode=pDC->SetBkMode(OPAQUE); 
CBrush *OldBrush=pDC->SelectObject(&brush);
int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
  for(int i=0;i<m_PointList->GetSize();i=i+10)
  {
   rect=CRect(m_PointList->GetAt(i),m_PointList->GetAt(i));
   rect.InflateRect(3,3);
   pDC->Rectangle(rect);
  }
    pDC->SelectObject(OldPen);
    pDC->SetBkMode(oldBkMode); 
    pDC->SelectObject(OldBrush);
    pDC->SetROP2(oldDrawingMode);
}

CRect CCurve::GetBoundingRect()
{
 m_MinPoint=m_MaxPoint=m_PointList->GetAt(0);
 for(int i=0;i<m_PointList->GetSize();i++)
  {
   m_MinPoint.x=min(m_MinPoint.x,m_PointList->GetAt(i).x);
   m_MinPoint.y=min(m_MinPoint.y,m_PointList->GetAt(i).y);
   m_MaxPoint.x=max(m_MaxPoint.x,m_PointList->GetAt(i).x);
   m_MaxPoint.y=max(m_MaxPoint.y,m_PointList->GetAt(i).y);
 }
 
 this->m_BoundingRect.SetRect(this->m_MinPoint,this->m_MaxPoint);
 return this->m_BoundingRect;
}

void CCurve::DrawActive(CDC *pDC,CPoint point)
{
 this->DrawStatic(pDC);
  
   
}

void CCurve::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
    if(pView->m_CurrentDrawStatus==Drag_Status)
	  {
	    for(int i=0;i<m_PointList->GetSize();i++)
		{
	   CPoint point=m_PointList->GetAt(i);
	   point.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
	   m_PointList->SetAt(i,point);
	   }
	  }
 else
	  {
	  pDC->SetROP2(R2_COPYPEN);
	  pView->m_FirstPoint=pView->m_SecondPoint;
	  pView->m_SecondPoint=point;
	  m_PointList->Add(point);
	  //m_DrawingMode=pDC->GetROP2();
      DrawMask(pDC,pView->m_FirstPoint,pView->m_SecondPoint);
	  }
	  
}

void CCurve::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
	if(m_HaveFindFirst)
	{   
		pView->L_iPointCount=IsOnMarginPoint(point);
        if(pView->L_iPointCount!=-1)
		{
        pView->m_bHaveFindSecond=true;
		pView->m_CurrentDrawStatus=Drag_Status;
		pView->m_CurrentDrawTool=Curve_Tool;
		pView->m_pCurrentUnit=this;
		m_DrawingMode=pDC->GetROP2();
        ShowSelectPoint(pDC);
		DrawStatic(pDC);
		DrawOldReferencePoint(pDC,m_FoundPoint);
		}
	}	
	if((!pView->m_bHaveFindSecond)&&IsInRgn(point)!=-1)
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
	
}
