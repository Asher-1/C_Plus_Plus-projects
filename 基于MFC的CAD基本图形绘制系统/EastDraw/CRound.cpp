#include "stdafx.h"
#include"CRound.h"
#include"EastDrawView.h"
IMPLEMENT_SERIAL(CRound,CUnit,1)
void CRound::DrawStatic(CDC*pDC)
{
  CPen m_pen;
  CBrush m_brush;
  int oldDrawingMode=pDC->SetROP2(m_DrawingMode);
  m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
  CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;

  m_brush.CreateBrushIndirect(&brushlog);
  
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
  SetBrushOrg(pDC,&m_brush);
 int OldBkMode=pDC->SetBkMode(m_BkMode);
 COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);

// CPoint lpoint=m_SecondPoint.x-m_FirstPoint.x>m_SecondPoint.y-m_FirstPoint.y?CPoint(m_SecondPoint.x,m_FirstPoint.y+m_SecondPoint.x-m_FirstPoint.x):CPoint(m_FirstPoint.x+m_SecondPoint.y-m_FirstPoint.y,m_SecondPoint.y);
 //m_SecondPoint=lpoint;
 pDC->Ellipse(CRect(m_FirstPoint,m_SecondPoint)); 
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 pDC->SetROP2(oldDrawingMode);
}

CRound::CRound()
{
  Initial();
 
  m_BrushStyle=BS_HOLLOW;
  m_BrushHatch=HS_CROSS;

}





int CRound::ComputRadious(CPoint centerPoint, CPoint movingPoint)
{

m_Radious=int(sqrt((centerPoint.x-movingPoint.x)*(centerPoint.x-movingPoint.x)+(centerPoint.y-movingPoint.y)*(centerPoint.y-movingPoint.y)));
CPoint cpoint;
cpoint=centerPoint;
cpoint.Offset(-m_Radious,-m_Radious);
this->m_FirstPoint=cpoint;
cpoint=centerPoint;
cpoint.Offset(m_Radious,m_Radious);
this->m_SecondPoint=cpoint;
return 0;

}

BOOL CRound::IsInRgn(CPoint point)
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

int CRound::IsOnMarginPoint(CPoint point)
{
 CPoint FirstPoint;
 CPoint SecondPoint;
 FirstPoint.x=m_FirstPoint.x<m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 FirstPoint.y=m_FirstPoint.y<m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 SecondPoint.x=m_FirstPoint.x>m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
 SecondPoint.y=m_FirstPoint.y>m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
 m_FirstPoint=FirstPoint;
 m_SecondPoint=SecondPoint;
 CRect rect(point,point);
 rect.InflateRect(CSize(4,4));
 if(rect.PtInRect(CPoint(m_FirstPoint.x+(m_SecondPoint.x-m_FirstPoint.x)/2,m_FirstPoint.y)))
	{
	 m_FoundPoint=CPoint(m_FirstPoint.x+(m_SecondPoint.x-m_FirstPoint.x)/2,m_FirstPoint.y);
	 return 1;
    }   
 if(rect.PtInRect(CPoint(m_SecondPoint.x,m_FirstPoint.y+(m_SecondPoint.y-m_FirstPoint.y)/2)))
 {
	 m_FoundPoint=CPoint(m_SecondPoint.x,m_FirstPoint.y+(m_SecondPoint.y-m_FirstPoint.y)/2);
	 return 2;
 }
 if(rect.PtInRect(CPoint(m_SecondPoint.x+(m_FirstPoint.x-m_SecondPoint.x)/2,m_SecondPoint.y)))
 {
	 m_FoundPoint=CPoint(m_SecondPoint.x+(m_FirstPoint.x-m_SecondPoint.x)/2,m_SecondPoint.y);
	 return 3;
 }
 if(rect.PtInRect(CPoint(m_FirstPoint.x,m_SecondPoint.y+(m_FirstPoint.y-m_SecondPoint.y)/2)))
 {
	 m_FoundPoint=CPoint(m_FirstPoint.x,m_SecondPoint.y+(m_FirstPoint.y-m_SecondPoint.y)/2);
	 return 4;
 }
 if(rect.PtInRect(this->m_CenterPoint))
 {
	 m_FoundPoint=m_CenterPoint;
	 return 5;
 }
  return 0;
}

void CRound::Serialize(CArchive &ar)
{
    CUnit::Serialize(ar);
    if(ar.IsStoring())
	{
	  ar<<m_BrushStyle<<m_BkMode<<m_BrushHatch<<m_BrushColor<<m_CenterPoint;
	}
	else
	{
	  ar>>m_BrushStyle>>m_BkMode>>m_BrushHatch>>m_BrushColor>>m_CenterPoint;
	}


}

void CRound::ShowSelectPoint(CDC *pDC)
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

void CRound::DrawActive(CDC *pDC,CPoint point)
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

// CPoint lpoint=m_SecondPoint.x-m_FirstPoint.x>m_SecondPoint.y-m_FirstPoint.y?CPoint(m_SecondPoint.x,m_FirstPoint.y+m_SecondPoint.x-m_FirstPoint.x):CPoint(m_FirstPoint.x+m_SecondPoint.y-m_FirstPoint.y,m_SecondPoint.y);
 //m_SecondPoint=lpoint;
 pDC->Ellipse(CRect(m_FirstPoint,m_SecondPoint)); 
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 
}

void CRound::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
  if(m_HaveFindFirst)
	{
	 pView->L_iPointCount=IsOnMarginPoint(point);
	if(pView->L_iPointCount!=0)
	{
		pView->m_bHaveFindSecond=true;
        pView->m_CurrentDrawTool=Round_Tool;
        pView->m_CurrentDrawStatus=Change_Status;
	    pView->m_FirstPoint=m_FirstPoint;
	    pView->m_SecondPoint=m_SecondPoint;
	    
	}//******if(L_iPointCount!=0)
	
  if(pView->L_iPointCount==5)
	{
	 pView->m_FirstPoint=m_FirstPoint;
	 pView->m_SecondPoint=m_SecondPoint;
     pView->m_CurrentDrawTool=Round_Tool;
     pView->m_CurrentDrawStatus=Drag_Status;
	}
	}//********if(L_pRound->m_HaveFindFirst)
  
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
           ShowSelectPoint(pDC);
		}

	 }

   
   if(pView->m_bHaveFindSecond)
		{   
	        pView->m_pCurrentUnit=this;
			m_MovingPoint=point;
	        m_DrawingMode=pDC->GetROP2();
			//m_pCurrentUnit->m_MovingPoint=point;
			ShowSelectPoint(pDC);
		    DrawOldReferencePoint(pDC,m_FoundPoint);
	        DrawStatic(pDC);
	    }
 }//******for(i=0;!m_bHaveFindFirst&&i<pDoc->m_RoundArray.GetSize();i++)
 

void CRound::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
          if(pView->m_CurrentDrawStatus==Drag_Status)
		  {
			 m_FirstPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
             m_SecondPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
	         m_CenterPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
		  
		  }
		  else
		  {
	      
		  ShowMovingLine(pDC,m_CenterPoint,pView->m_SecondPoint);
		   this->DrawActive(pDC,point);
		  ComputRadious(m_CenterPoint,point);
	      pView->m_SecondPoint=point;
		  
		 this->DrawActive(pDC,point);
		  ShowMovingLine(pDC,m_CenterPoint,pView->m_SecondPoint);

		  }
}