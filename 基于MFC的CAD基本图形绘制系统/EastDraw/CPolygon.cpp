#include"stdafx.h"
#include"CPolygon.h"
#include"EastDrawView.h"
IMPLEMENT_SERIAL(CPolygon,CUnit,1)
void CPolygon::Serialize(CArchive &ar)
{
 
 CUnit::Serialize(ar);
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
 
void CPolygon::Initial()
{
  CUnit::Initial();
  m_PointList=new CArray<CPoint,CPoint>;
}

void CPolygon::DrawStatic(CDC *pDC)
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
 m_FirstPoint=m_PointList->GetAt(0);
 pDC->MoveTo(m_FirstPoint);
 for(int i=0;i<m_PointList->GetSize();i++)
 {
   pDC->LineTo(m_PointList->GetAt(i));
 }
   pDC->LineTo(m_PointList->GetAt(0));
   pDC->SelectObject(brush);
   pDC->SelectObject(pen);
   pDC->SetBkMode(OldBkMode);
   pDC->SetBkColor(OldColor);
   pDC->SetROP2(oldDrawingMode);
}

void CPolygon::DrawMask(CDC *pDC, CPoint point)
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
 pDC->MoveTo(m_PointList->GetAt(m_PointList->GetSize()-1));
 pDC->LineTo(point);
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 //pDC->SetROP2(oldDrawingMode);

}

CPolygon::CPolygon()
{
Initial();
}

void CPolygon::DrawEnd(CDC*pDC,CPoint point)
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
 pDC->MoveTo(point);
 pDC->LineTo(m_PointList->GetAt(0));
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 pDC->SetROP2(oldDrawingMode);
}

int CPolygon::IsOnMarginPoint(CPoint point)
{
  CRect rect;
  CPoint firstPoint;
  CPoint secondPoint;
  firstPoint=secondPoint=m_PointList->GetAt(0);
  for(int i=0;i<m_PointList->GetSize();i++)
  {
   secondPoint=m_PointList->GetAt(i);
   rect=CRect(m_PointList->GetAt(i),m_PointList->GetAt(i));
   rect.InflateRect(4,4);
   if(rect.PtInRect(point))
	  { 
	   m_FoundIndex=i;
	   m_FoundPoint=m_PointList->GetAt(i);
	   return i;
      } 
   rect.SetRect(CPoint((firstPoint.x+secondPoint.x)/2,(firstPoint.y+secondPoint.y)/2),CPoint((firstPoint.x+secondPoint.x)/2,(firstPoint.y+secondPoint.y)/2));
   rect.InflateRect(4,4);
   if(rect.PtInRect(point))
      { 
	   m_FoundPoint=CPoint((firstPoint.x+secondPoint.x)/2,(firstPoint.y+secondPoint.y)/2);      
       return -i-10;
      }   
   firstPoint=secondPoint;

  }
  return -1;
}

BOOL CPolygon::IsInRgn(CPoint point)
{
   CPoint point4[4];
   CPoint FirstPoint;
   CPoint SecondPoint;
   m_FirstPoint=m_PointList->GetAt(m_PointList->GetSize()-1);
   CRgn rgn;
for(int i=0;i<m_PointList->GetSize();i++)
{
    m_SecondPoint=m_PointList->GetAt(i);
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
  point4[0].x=FirstPoint.x-3-m_PenWidth;
  point4[0].y=FirstPoint.y-4-m_PenWidth;

  point4[1].x=FirstPoint.x+3+m_PenWidth;
  point4[1].y=FirstPoint.y+4+m_PenWidth;

  point4[2].x=SecondPoint.x+3+m_PenWidth;
  point4[2].y=SecondPoint.y+4+m_PenWidth;

  point4[3].x=SecondPoint.x-3-m_PenWidth;
  point4[3].y=SecondPoint.y-4-m_PenWidth;
  
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
  point4[0].x=FirstPoint.x+3+m_PenWidth;
  point4[0].y=FirstPoint.y-4-m_PenWidth;

  point4[1].x=FirstPoint.x-3-m_PenWidth;
  point4[1].y=FirstPoint.y+4+m_PenWidth;

  point4[2].x=SecondPoint.x-3-m_PenWidth;
  point4[2].y=SecondPoint.y+4+m_PenWidth;

  point4[3].x=SecondPoint.x+3+m_PenWidth;
  point4[3].y=SecondPoint.y-4-m_PenWidth;
  
  }

if((m_FirstPoint.x-m_SecondPoint.x)==0)
  {
  point4[0].x=m_FirstPoint.x-3-m_PenWidth;
  point4[0].y=m_FirstPoint.y-4-m_PenWidth;

  point4[1].x=m_FirstPoint.x+3+m_PenWidth;
  point4[1].y=m_FirstPoint.y-4-m_PenWidth;

  point4[2].x=m_SecondPoint.x+3+m_PenWidth;
  point4[2].y=m_SecondPoint.y+4+m_PenWidth;

  point4[3].x=m_SecondPoint.x-3-m_PenWidth;
  point4[3].y=m_SecondPoint.y+4+m_PenWidth;
  
  }

if((m_FirstPoint.y-m_SecondPoint.y)==0)
  {
  point4[0].x=m_FirstPoint.x-4-m_PenWidth;
  point4[0].y=m_FirstPoint.y-3-m_PenWidth;

  point4[1].x=m_FirstPoint.x-4-m_PenWidth;
  point4[1].y=m_FirstPoint.y+3+m_PenWidth;

  point4[2].x=m_SecondPoint.x+4+m_PenWidth;
  point4[2].y=m_SecondPoint.y+3+m_PenWidth;

  point4[3].x=m_SecondPoint.x+4+m_PenWidth;
  point4[3].y=m_SecondPoint.y-3-m_PenWidth;
  }
  m_FirstPoint=m_SecondPoint;  
  rgn.CreatePolygonRgn(point4,4,ALTERNATE);
  if(rgn.PtInRegion(point))
	  return true;
    rgn.DeleteObject();
}


  return false;
}

void CPolygon::ExchangeDraw(CDC *pDC, CPoint point)
{
 //m_FoundIndex
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
  
 m_SecondPoint=m_PointList->GetAt((m_FoundIndex+1)%m_PointList->GetSize());
 m_FirstPoint=m_PointList->GetAt(m_FoundIndex==0? m_PointList->GetSize()-1:m_FoundIndex-1);
 
 if(m_FoundIndex!=0)
 {
 pDC->MoveTo(m_FirstPoint);
 pDC->LineTo(point);
 pDC->MoveTo(point);
 pDC->LineTo(m_SecondPoint);
 }
 if(m_FoundIndex==0)
 {
  pDC->MoveTo(m_SecondPoint);
  pDC->LineTo(point);
  pDC->MoveTo(point);
  pDC->LineTo(m_FirstPoint);
 }

 m_PointList->SetAt(m_FoundIndex,point);
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 pDC->SetROP2(oldDrawingMode);

}



void CPolygon::ShowSelectPoint(CDC *pDC)
{
  CRect rect;
  CPoint firstPoint;
  CPoint secondPoint;
  firstPoint=secondPoint=m_PointList->GetAt(0);
  CBrush brush;
  brush.CreateSolidBrush(RGB(0,0,255));
  CPen m_pen;
  m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
  CPen *OldPen=pDC->SelectObject(&m_pen);
  int oldBkMode=pDC->SetBkMode(OPAQUE); 
  CBrush *OldBrush=pDC->SelectObject(&brush);
  int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
  for(int i=0;i<m_PointList->GetSize();i++)
  {
   rect=CRect(m_PointList->GetAt(i),m_PointList->GetAt(i));
   rect.InflateRect(3,3);
   pDC->Rectangle(rect);
  }
   
  pDC->SelectObject(OldBrush);
  brush.DeleteObject();
  brush.CreateSolidBrush(RGB(255,0,0));
  pDC->SelectObject(&brush);
   firstPoint=m_PointList->GetAt(0);
   //secondPoint=m_PointList->GetAt(1);
  for(i=1;i<m_PointList->GetSize();i++)
  {
   secondPoint=m_PointList->GetAt(i);
   rect.SetRect(CPoint((firstPoint.x+secondPoint.x)/2,(firstPoint.y+secondPoint.y)/2),CPoint((firstPoint.x+secondPoint.x)/2,(firstPoint.y+secondPoint.y)/2));
   rect.InflateRect(3,3);
   pDC->Rectangle(rect);
   firstPoint=secondPoint;
  }
  
  pDC->SelectObject(OldPen);
  pDC->SetBkMode(oldBkMode); 
  pDC->SelectObject(OldBrush);
  pDC->SetROP2(oldDrawingMode);



}

void CPolygon::DrawActive(CDC *pDC,CPoint point)
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
 
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
 
 int OldBkMode=pDC->SetBkMode(m_BkMode);
 COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
 m_FirstPoint=m_PointList->GetAt(0);
 pDC->MoveTo(m_FirstPoint);
 for(int i=0;i<m_PointList->GetSize();i++)
 {
   pDC->LineTo(m_PointList->GetAt(i));
 }
   pDC->LineTo(m_PointList->GetAt(0));
   pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 
}

void CPolygon::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
  if(m_HaveFindFirst)
  {
  pView->L_iPointCount=IsOnMarginPoint(point);
  if(pView->L_iPointCount>=0)
  {     
        pView->m_bHaveFindSecond=true;
        pView->m_CurrentDrawTool=Polygon_Tool;
        pView->m_CurrentDrawStatus=Change_Status;
	
  }
  if(pView->L_iPointCount<-1)
  {  
	 pView->m_bHaveFindSecond=true;
     pView->m_CurrentDrawTool=Polygon_Tool;
     pView->m_CurrentDrawStatus=Drag_Status;
  }
}//*******if(L_pPolygon->m_HaveFindFirst)
  
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
			DrawOldReferencePoint(pDC,m_FoundPoint);
			DrawStatic(pDC);
 }
}

void CPolygon::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
   if(pView->m_CurrentDrawStatus==Draw_Status)
   {
      if(pView->m_LBDownTimes!=0)
	  {
	  pDC->SetROP2(R2_NOTXORPEN);
	  //m_DrawingMode=pDC->GetROP2();
	  DrawMask(pDC,pView->m_SecondPoint);
      pView->m_SecondPoint=point;
     pDC->SetROP2(this->m_DrawingMode);
	  DrawMask(pDC,pView->m_SecondPoint);
	  }
	}
     if(pView->m_CurrentDrawStatus==Change_Status)
	 {
	   pDC->SetROP2(R2_NOTXORPEN);
       ExchangeDraw(pDC,pView->m_SecondPoint);
       pView->m_SecondPoint=point;
       ExchangeDraw(pDC,pView->m_SecondPoint);
     }
  if(pView->m_CurrentDrawStatus==Drag_Status)
  {
	  for(int i=0;i<m_PointList->GetSize();i++)
	  {
	   CPoint point=m_PointList->GetAt(i);
	   point.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
	   m_PointList->SetAt(i,point);
	   }
  
  
  }

}
