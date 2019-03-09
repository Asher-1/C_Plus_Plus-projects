#include"stdafx.h"
#include"CRoundArc.h"
#include"EastDrawView.h"
IMPLEMENT_SERIAL(CRoundArc,CUnit,1)
CRoundArc::CRoundArc()
{

}

void CRoundArc::DrawStatic(CDC *pDC)
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
 
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
 
  int OldBkMode=pDC->SetBkMode(m_BkMode);
  COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
  CRect rect;
  rect.SetRect(m_CenterPoint.x-m_Xr,m_CenterPoint.y-m_Xr,m_CenterPoint.x+m_Xr,m_CenterPoint.y+m_Xr);
  pDC->Arc(rect,m_FirstPoint,m_ThirdPoint);

  pDC->SelectObject(brush);
  pDC->SelectObject(pen);
  pDC->SetBkMode(OldBkMode);
  pDC->SetBkColor(OldColor);
  pDC->SetROP2(oldDrawingMode);

}

double CRoundArc::ComputeSloap1(CPoint firstpoint, CPoint secondpoint)
{
    if(firstpoint.y==secondpoint.y)
	{
	  m_Sloap1=-9999999999999.0;
	}
	else
	{
	  m_Sloap1=-double((firstpoint.x-secondpoint.x))/double((firstpoint.y-secondpoint.y));
	}
   m_Sloap2=m_Sloap1;
   m_CenterPoint=CPoint((firstpoint.x+secondpoint.x)/2,(firstpoint.y+secondpoint.y)/2);
   m_Xr=0;//sqrt((firstpoint.x-secondpoint.x)*(firstpoint.x-secondpoint.x)+(firstpoint.y-firstpoint.y)*(firstpoint.y-firstpoint.y));
   return m_Sloap1;
}

double CRoundArc::ComputeSloap2(CPoint firstpoint, CPoint secondpoint)
{
if(firstpoint.y==secondpoint.y)
	{
	  m_Sloap2=-999999999.0;
	}
	else
	{
	 m_Sloap2=-double((firstpoint.x-secondpoint.x))/double((firstpoint.y-secondpoint.y));
	}
    
    


this->m_CenterPoint.x=((m_FirstPoint.y-m_ThirdPoint.y)/2-m_Sloap1*(m_FirstPoint.x+m_SecondPoint.x)/2+m_Sloap2*(m_SecondPoint.x+m_ThirdPoint.x)/2)/(m_Sloap2-m_Sloap1);
this->m_CenterPoint.y=((m_FirstPoint.y+m_SecondPoint.y)/2*m_Sloap2-(m_SecondPoint.y+m_ThirdPoint.y)/2*m_Sloap1+m_Sloap1*m_Sloap2*(m_ThirdPoint.x-m_FirstPoint.x)/2)/(m_Sloap2-m_Sloap1);

m_Xr=sqrt((m_CenterPoint.x-m_ThirdPoint.x)*(m_CenterPoint.x-m_ThirdPoint.x)+(m_CenterPoint.y-m_ThirdPoint.y)*(m_CenterPoint.y-m_ThirdPoint.y));	

return m_Sloap2;
}


void CRoundArc::DrawActive(CDC *pDC,CPoint point)
{
  
ShowMovingLine(pDC,m_FirstPoint,m_SecondPoint);
if(m_Xr==0)
	  return;
ShowMovingLine(pDC,m_FirstPoint,m_CenterPoint);

ShowMovingLine(pDC,m_SecondPoint,m_CenterPoint);

ShowMovingLine(pDC,m_SecondPoint,m_ThirdPoint);


  CPen m_pen;
  CBrush m_brush;
  
  m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
  CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;

  m_brush.CreateBrushIndirect(&brushlog);
 
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
 
  int OldBkMode=pDC->SetBkMode(m_BkMode);
  COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
  CRect rect;
  rect.SetRect(m_CenterPoint.x-m_Xr,m_CenterPoint.y-m_Xr,m_CenterPoint.x+m_Xr,m_CenterPoint.y+m_Xr);
  pDC->Arc(rect,m_FirstPoint,m_ThirdPoint);

  pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);


}

void CRoundArc::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
	if(ar.IsStoring())
	{
	 ar<<m_ThirdPoint<<m_Xr<<m_CenterPoint;
	}
	else
	{
	 ar>>m_ThirdPoint>>m_Xr>>m_CenterPoint;
	}

}

BOOL CRoundArc::IsInRgn(CPoint point)
{
 
	
   if(abs(sqrt((point.x-m_CenterPoint.x)*(point.x-m_CenterPoint.x)+(point.y-m_CenterPoint.y)*(point.y-m_CenterPoint.y))-m_Xr)<=2)
      return true;
 
   return false;

}

void CRoundArc::ShowSelectPoint(CDC *pDC)
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
rect.SetRect(m_FirstPoint,m_FirstPoint);
rect.InflateRect(3,3);
pDC->Rectangle(rect);

rect.SetRect(m_SecondPoint,m_SecondPoint);
rect.InflateRect(3,3);
pDC->Rectangle(rect);

rect.SetRect(m_ThirdPoint,m_ThirdPoint);
rect.InflateRect(3,3);
pDC->Rectangle(rect);


  pDC->SelectObject(OldBrush);
  brush.DeleteObject();
  brush.CreateSolidBrush(RGB(255,0,0));
  pDC->SelectObject(&brush);
  rect.SetRect(this->m_CenterPoint,this->m_CenterPoint);
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);
  pDC->SelectObject(OldPen);

  pDC->SetBkMode(oldBkMode); 
  pDC->SelectObject(OldBrush);
  pDC->SetROP2(oldDrawingMode);

}

int CRoundArc::IsOnMarginPoint(CPoint point)
{
CRect rect;
rect.SetRect(point,point);
rect.InflateRect(4,4);
if(rect.PtInRect(m_FirstPoint))
{   this->m_FoundPoint=m_FirstPoint;
	return 1;
}
if(rect.PtInRect(m_SecondPoint))
{   this->m_FoundPoint=m_SecondPoint;
	return 2;
}
if(rect.PtInRect(m_ThirdPoint))
{   this->m_FoundPoint=m_ThirdPoint;
	return 3;
}
if(rect.PtInRect(m_CenterPoint))
{   this->m_FoundPoint=m_CenterPoint;
	return 4;
}
return 0;
}

void CRoundArc::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
   
  if(m_HaveFindFirst)
   { 
	  pView->L_iPointCount=IsOnMarginPoint(point);
     if(pView->L_iPointCount!=0)
	 {   
		 pView->m_bHaveFindSecond=true;
         pView->m_pCurrentUnit=this;
	     m_DrawingMode=pDC->GetROP2();
	     DrawActive(pDC,point); 
         ShowSelectPoint(pDC);
		 DrawOldReferencePoint(pDC,m_FoundPoint);
		 m_HaveFindFirst=true;
		 pView->m_CurrentDrawTool=RoundArc_Tool;
         pView->m_CurrentDrawStatus=Change_Status;
		 pView->m_FirstPoint=m_FirstPoint;
	     pView->m_SecondPoint=m_SecondPoint;
		 
	   if(pView->L_iPointCount==1)
	   {
	    
	   }
	   if(pView->L_iPointCount==2)
	   {
	   
	   }
	   if(pView->L_iPointCount==3)
	   {
	   }
	   if(pView->L_iPointCount==4)
	   {
	    pView->m_CurrentDrawTool=RoundArc_Tool;
        pView->m_CurrentDrawStatus=Drag_Status;
	   }
	 }//***** if(L_iPointCount!=0)
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
  
}

void CRoundArc::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
	  if(pView->m_CurrentDrawStatus==Drag_Status)
	  {
	  m_FirstPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
      m_SecondPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
	  m_ThirdPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
      m_CenterPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
	  }
	  else
	  {
	  pDC->SetROP2(R2_NOTXORPEN);
      if(pView->m_LBDownTimes==1&&pView->m_CurrentDrawStatus!=Change_Status)
	  {
		  ShowMovingLine(pDC,pView->m_FirstPoint,pView->m_SecondPoint);
		  pView->m_SecondPoint=point;
		  ShowMovingLine(pDC,pView->m_FirstPoint,point);
	  }
	  if(pView->m_LBDownTimes==2&&pView->m_CurrentDrawStatus!=Change_Status)
	  {
	      DrawActive(pDC,point);
		  m_ThirdPoint=point;
		  ComputeSloap2(m_SecondPoint,m_ThirdPoint);
          DrawActive(pDC,point);
	  }
   if(pView->m_CurrentDrawStatus==Change_Status)
	  {
	    DrawActive(pDC,point);
	    if(pView->L_iPointCount==1)
		{
		  m_FirstPoint=point;
		  ComputeSloap1(m_FirstPoint,m_SecondPoint);
          ComputeSloap2(m_SecondPoint,m_ThirdPoint);
		}
	    if(pView->L_iPointCount==2)
		{
		  m_SecondPoint=point;
          ComputeSloap1(m_FirstPoint,m_SecondPoint);
		  ComputeSloap2(m_SecondPoint,m_ThirdPoint);
		}
		if(pView->L_iPointCount==3)
		{
		 m_ThirdPoint=point;
		 ComputeSloap2(m_SecondPoint,m_ThirdPoint);
		}
	     DrawActive(pDC,point);
		 pView->m_SecondPoint=point;
   }//******if(m_CurrentDrawStatus==Exchange_Status)
	  }
	  }
