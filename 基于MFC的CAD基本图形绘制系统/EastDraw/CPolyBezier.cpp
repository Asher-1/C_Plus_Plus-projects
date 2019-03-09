#include "stdafx.h"
#include "CPolyBezier.h"
#include "EastDrawView.h"
IMPLEMENT_SERIAL(CPolyBezier,CUnit,1)

CPolyBezier::CPolyBezier()
{
	CUnit::Initial();
	m_HaveDrawFirst=false;
}

void CPolyBezier::DrawActive(CDC *pDC, CPoint point)
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
  m_PointList=new CPoint[4];
  for(int i=3;i>0;i--)
  m_PointList[3-i]=m_PointArray.GetAt(m_PointArray.GetSize()-i);
  m_PointList[3]=point;


pDC->PolyBezier(m_PointList,4);

delete m_PointList;

}

void CPolyBezier::OnLButtonDown(CDC*pDC,CEastDrawView*pView,CPoint point)
{
   if(pView->m_CurrentDrawStatus==Draw_Status)
   {
    switch(pView->m_LBDownTimes%3)
	{
	case 0:
		DrawActive(pDC,point);
		m_PointArray.SetAt(m_PointArray.GetSize()-1,point);
        DrawMaskPoint(pDC);
		break;
	case 1:
        m_PointArray.Add(point);
		m_PointArray.Add(point);
		m_PointArray.Add(point);
	    break;
	case 2:
		m_PointArray.SetAt(m_PointArray.GetSize()-2,point);
		m_PointArray.SetAt(m_PointArray.GetSize()-1,point);
	    break;
	}
   
   }//******if(pView->m_CurrentDrawStatus==Draw_Status)
      
   if(pView->m_CurrentDrawStatus==Select_Status)
   {  
	  if(this->m_HaveFindFirst)
	   { 
	    pView->L_iPointCount=this->IsOnMarginPoint(point);
	    if(pView->L_iPointCount!=0)
		{
	        pView->m_CurrentDrawStatus=Change_Status;
	        pView->m_bHaveFindSecond=true;
		    pView->m_CurrentDrawTool=PolyBezier_Tool;
		    pView->m_pCurrentUnit=this;
		    this->DrawStatic(pDC);
		    this->ShowSelectPoint(pDC);
		
			this->DrawOldReferencePoint(pDC,this->m_FoundPoint);

		
		}
	  }
	  if(!pView->m_bHaveFindSecond&&this->IsInRgn(point))
	  {
		  if(this->m_HaveFindFirst)
		  {
		   this->m_HaveFindFirst=false;
		   pView->m_bHaveFindFirst=true;
		  }
		  else
		  {
			  this->m_HaveFindFirst=true;
		      pView->m_bHaveFindFirst=true;
		  }
		  this->ShowSelectPoint(pDC);
	  }
      
   }

}

void CPolyBezier::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{ 
	static int m_Pre=-1;
  if(pView->m_CurrentDrawStatus==Draw_Status)
  {
 
  if(!m_HaveDrawFirst)
  {
  DrawActive(pDC,pView->m_LastDownPoint);
  m_HaveDrawFirst=true;
  }

  if(pView->m_LBDownTimes%3==0&&pView->m_LBDownTimes/3!=m_Pre)
  {
   m_HaveDrawFirst=false;
   m_Pre=pView->m_LBDownTimes/3;
  } 
   this->DrawActive(pDC,pView->m_FirstPoint);
   pView->m_FirstPoint=point;
   this->DrawActive(pDC,point);
  }//*******if(pView->m_CurrentDrawStatus==Draw_Status)
  
  if(pView->m_CurrentDrawStatus==Change_Status)
  {   
	this->DrawStatic(pDC);
	pView->m_SecondPoint=point;
	m_PointArray.SetAt(pView->L_iPointCount-1,point);
	this->DrawStatic(pDC);
  }
  
}



void CPolyBezier::DrawStatic(CDC *pDC)
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

m_PointList=new CPoint[m_PointArray.GetSize()];
for(int i=0;i<m_PointArray.GetSize();i++)
   m_PointList[i]=m_PointArray.GetAt(i);
   pDC->PolyBezier(m_PointList,m_PointArray.GetSize());

delete m_PointList;
}

void CPolyBezier::ShowSelectPoint(CDC *pDC)
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
for(int i=0;i<m_PointArray.GetSize();i=i+3)
{
  rect.SetRect(m_PointArray.GetAt(i),m_PointArray.GetAt(i));
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);
}
  pDC->SelectObject(OldBrush);
  brush.DeleteObject();
  brush.CreateSolidBrush(RGB(255,0,0));
  pDC->SelectObject(&brush);

  for(i=1;i<m_PointArray.GetSize()-1;i=i+3)
  {
  rect.SetRect(m_PointArray.GetAt(i),m_PointArray.GetAt(i));
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);
  
  rect.SetRect(m_PointArray.GetAt(i+1),m_PointArray.GetAt(i+1));
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);

  }
 
pDC->SelectObject(OldPen);
pDC->SetBkMode(oldBkMode); 
pDC->SelectObject(OldBrush);
pDC->SetROP2(oldDrawingMode);
}

void CPolyBezier::DrawMaskPoint(CDC *pDC)
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

  
  rect.SetRect(m_PointArray.GetAt(m_PointArray.GetSize()-3),m_PointArray.GetAt(m_PointArray.GetSize()-3));
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);

  pDC->SelectObject(OldBrush);
  brush.DeleteObject();
  brush.CreateSolidBrush(RGB(255,0,0));
  pDC->SelectObject(&brush);
  rect.SetRect(m_PointArray.GetAt(m_PointArray.GetSize()-2),m_PointArray.GetAt(m_PointArray.GetSize()-2));
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);
  rect.SetRect(m_PointArray.GetAt(m_PointArray.GetSize()-1),m_PointArray.GetAt(m_PointArray.GetSize()-1));
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);

pDC->SelectObject(OldPen);
pDC->SetBkMode(oldBkMode); 
pDC->SelectObject(OldBrush);
pDC->SetROP2(oldDrawingMode);


}



int CPolyBezier::IsOnMarginPoint(CPoint point)
{
CRect rect;
rect.SetRect(point,point);
rect.InflateRect(4,4);
for(int i=0;i<m_PointArray.GetSize();i++)
{
 if(rect.PtInRect(m_PointArray.GetAt(i)))
	 
 {
	 m_FoundPoint=m_PointArray.GetAt(i);
	 return i+1;
 }
 } 
 
  return 0;
}

BOOL CPolyBezier::IsInRgn(CPoint point)
{
CRect rect;
rect.SetRect(point,point);
rect.InflateRect(20,20);
for(int i=0;i<m_PointArray.GetSize()-2;i=i+3)
{
 if(rect.PtInRect(m_PointArray.GetAt(i)))
	 return true;
} 
 
  return false;
}

void CPolyBezier::OnMenuitemOk(CDC *pDC, CEastDrawView *pView)
{
     //pView->m_LBDownTimes=0;
     pView->m_CurrentDrawTool=PolyBezier_Tool;
	 pView->m_CurrentDrawStatus=Select_Status;
	 this->m_PointArray.Add(pView->m_ChangeingPoint);
	 this->m_HaveFindFirst=true;
	 pView->m_bHaveFindFirst=true;
	 pView->ExchangeSave();
	 


CBrush brush;
brush.CreateSolidBrush(RGB(0,0,255));
CPen m_pen;
m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
CPen *OldPen=pDC->SelectObject(&m_pen);
int oldBkMode=pDC->SetBkMode(OPAQUE); 
CBrush *OldBrush=pDC->SelectObject(&brush);
int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
CRect rect;

  
  rect.SetRect(pView->m_ChangeingPoint,pView->m_ChangeingPoint);
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);
  pDC->SelectObject(OldBrush);
 


}

void CPolyBezier::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
	this->m_PointArray.Serialize(ar);

}

void CPolyBezier::OnContextMenu(CWnd *pWnd, CPoint point)
{
CEastDrawView*pView=(CEastDrawView*)pWnd;
if(pView->m_CurrentDrawStatus==Draw_Status)
{
CMenu menuTextEdit;
menuTextEdit.LoadMenu(IDR_MENU_OK);
menuTextEdit.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,pWnd);
}
}
