#include "stdafx.h"
#include"CUnit.h"
#include "EastDrawView.h"

IMPLEMENT_SERIAL(CUnit,CObject,1)
CUnit::CUnit()
{
Initial();
}
CUnit::CUnit(CUnit&unit)
{
  m_FirstPoint=unit.m_FirstPoint;
  m_SecondPoint=unit.m_SecondPoint;
  m_PenColor=unit.m_PenColor;
  m_PenWidth=unit.m_PenWidth;
  m_PenStyle=unit.m_PenStyle;
  

}

CUnit CUnit::operator =(CUnit&unit)
{
  m_FirstPoint=unit.m_FirstPoint;
  m_SecondPoint=unit.m_SecondPoint;
  m_PenColor=unit.m_PenColor;
  m_PenWidth=unit.m_PenWidth;
  m_PenStyle=unit.m_PenStyle;
  
  
  return *this;

}
void CUnit::Initial()
{
  m_BkMode=TRANSPARENT;
  m_Rgn=NULL;
  m_FirstPoint=0;
  m_SecondPoint=0;
  m_PenColor=RGB(0,0,0);
  m_BrushColor=RGB(0,0,0);
  m_BackgroundColor=RGB(255,255,255);
  m_PenWidth=1;
  m_PenStyle=PS_SOLID;
  
  m_DrawingMode=R2_COPYPEN;
  m_HaveLBUp=false;
  m_HaveFindFirst=false;
  m_DrawStatus=Draw_Status;

}

void CUnit::Serialize(CArchive &ar)
{
  if(ar.IsStoring())
  {
   ar<<m_PenColor<<m_PenStyle<<m_PenWidth<<m_DrawingMode<<m_BkMode<<m_BackgroundColor;
   ar<<m_FirstPoint<<m_SecondPoint;
  }
  else
  {
  ar>>m_PenColor>>m_PenStyle>>m_PenWidth>>m_DrawingMode>>m_BkMode>>m_BackgroundColor;
  ar>>m_FirstPoint>>m_SecondPoint;
  }

}

CRgn* CUnit::GetRgn()
{
  if(m_Rgn==NULL)
  {
  m_Rgn=new CRgn;
  CPoint point[2];

  point[0]=m_FirstPoint;
  point[1]=m_SecondPoint;
  m_Rgn->CreatePolygonRgn(point,2,ALTERNATE);
  }
  return m_Rgn;
}

BOOL CUnit::IsInRgn(CPoint point)
{
   this->GetRgn();
   return (m_Rgn->PtInRegion(point));
 
}

int CUnit::IsOnMarginPoint(CPoint point)
{
 return 0;
}

void CUnit::Circumrotate(CPoint first,CPoint second)
{

}

void CUnit::OnLButtonDown(CDC*pDC,CEastDrawView*pView,CPoint point)
{

}

void CUnit::DrawMask(CDC*pDC,CPoint first,CPoint second)
{

}





void CUnit::DrawMask(CDC *pDC, CPoint point)
{

}

void CUnit::DrawEnd(CDC*pDC,CPoint point)
{

}

void CUnit::ExchangeDraw(CDC *pDC, CPoint point)
{

}

int CUnit::ComputRadious(CPoint centerPoint, CPoint movingPoint)
{
return 0;
}

void CUnit::ellipseMidpoint(CDC*pDC,int xCenter, int yCenter, int Rx, int Ry)
{

}

float CUnit::ComputSloap(CPoint firstPoint, CPoint secondPoint)
{
return 0;
}

int CUnit::ComputRadiusY(CPoint firstPoint,CPoint secondPoint)
{
return 0;
}

int CUnit::ComputRadiusX(CPoint firstPoint,CPoint secondPoint)
{
return 0;
}

void CUnit::ShowMovingLine(CDC*pDC,CPoint firstPoint, CPoint secondPoint)
{
CPen m_pen;
m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
CPen *OldPen=pDC->SelectObject(&m_pen);
int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
int oldBkMode=pDC->SetBkMode(OPAQUE); 
pDC->MoveTo(firstPoint);
pDC->LineTo(secondPoint.x,secondPoint.y);
pDC->SelectObject(OldPen);
pDC->SetROP2(oldDrawingMode);
pDC->SetBkMode(oldBkMode);
}

void CUnit::ShowSelectPoint(CDC*pDC)
{

}

void CUnit::DrawOldReferencePoint(CDC *pDC, CPoint point)
{

CBrush brush;
brush.CreateSolidBrush(RGB(250,0,250));
CPen m_pen;
m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
CPen *OldPen=pDC->SelectObject(&m_pen);
int oldBkMode=pDC->SetBkMode(OPAQUE); 
CBrush *OldBrush=pDC->SelectObject(&brush);
int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
CRect rect;
rect.SetRect(point,point);
rect.InflateRect(3,3);
pDC->Rectangle(rect);
pDC->SelectObject(OldPen);
pDC->SelectObject(OldBrush);
pDC->SetROP2(oldDrawingMode);
pDC->SetBkMode(oldBkMode);

}

CRect CUnit::GetBoundingRect()
{
return 0;
}

double CUnit::ComputeSloap1(CPoint firstpoint, CPoint secondpoint)
{
return 0;
}

double CUnit::ComputeSloap2(CPoint firstpoint, CPoint secondpoint)
{
return 0;
}

void CUnit::DrawActive(CDC *pDC,CPoint point)
{

}

void CUnit::DrawActiveStepOne(CDC *pDC,CPoint movingPoint)
{

}

void CUnit::DrawStaticStepOne(CDC *pDC,CPoint movingPoint)
{

}

CPoint CUnit::ComputeIntersectionPointFirst(CPoint point)
{
 return 0;
}

CPoint CUnit::ComputeIntersectionPointSecond(CPoint point)
{
return 0;
}

void CUnit::OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point)
{

}

void CUnit::OnLButtonDblClk(CDC *pDC,CEastDrawView *pView, CPoint point)
{

}

void CUnit::SetFont(ENUMLOGFONT *lpelf)
{

}

void CUnit::ChangeFont(ENUMLOGFONT *lpelf)
{

}

void CUnit::ChangeFontSize(ENUMLOGFONT *lpelf)
{

}

void CUnit::OnContextMenu(CWnd *pWnd, CPoint point)
{


}

void CUnit::OnMenuitemOk(CDC *pDC, CEastDrawView *pView)
{

}

void CUnit::OnMENUITEMSize()
{

}

void CUnit::OnMenuitemCirCu(CDC *pDC, CEastDrawView *pView)
{

}

void CUnit::SetBrushOrg(CDC *pDC,CBrush*brush)
{
    if(pDC!=NULL&&brush!=NULL)
	{
	if(pDC->m_hDC!=NULL&&brush->m_hObject!=NULL)
	{
	CPoint pointB(0,0);
    pDC->LPtoDP(&pointB);
	pointB.x%=8;
	pointB.y%=8;
	brush->UnrealizeObject();
   
	pDC->SetBrushOrg(pointB);
	}
	}
}