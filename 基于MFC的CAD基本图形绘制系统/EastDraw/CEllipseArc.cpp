#include"stdafx.h"
#include"CEllipseArc.h"
#include "EastDrawView.h"
IMPLEMENT_SERIAL(CEllipseArc,CUnit,1)
CEllipseArc::CEllipseArc()
{
CUnit::Initial();
m_IsDrawingArc=false;
m_IsCirCu=false;
}


void CEllipseArc::DrawActiveStepOne(CDC *pDC,CPoint movingPoint)
{
m_MovingPoint=movingPoint;
this->ShowMovingLine(pDC,this->m_FirstPoint,this->m_SecondPoint);
if(m_Xr!=0)
this->ShowMovingLine(pDC,movingPoint,m_CenterPoint);

this->ellipseMidpoint(pDC,this->m_CenterPoint.x,this->m_CenterPoint.y,this->m_Xr,this->m_Yr);

}

int CEllipseArc::ComputRadiusX(CPoint firstPoint, CPoint secondPoint)
{

 m_Xr=sqrt((firstPoint.x-secondPoint.x)*(firstPoint.x-secondPoint.x)+(firstPoint.y-secondPoint.y)*(firstPoint.y-secondPoint.y));

 return 0;
}

int CEllipseArc::ComputRadiusY(CPoint firstPoint, CPoint secondPoint)
{
	m_Yr=sqrt((firstPoint.x-secondPoint.x)*(firstPoint.x-secondPoint.x)+(firstPoint.y-secondPoint.y)*(firstPoint.y-secondPoint.y));
    m_Xr=0;
	return 0;
}

float CEllipseArc::ComputSloap(CPoint firstPoint, CPoint secondPoint)
{
m_SecondPoint=secondPoint;
	double sloap;
	if(firstPoint.x==secondPoint.x)
	{
	  sloap=9999999999999.0;
	}
	else
	{
	sloap=double((firstPoint.y-secondPoint.y))/double((firstPoint.x-secondPoint.x));
	}
   
  if(sloap<=0)
   {
   m_Cos=fabs(sloap/sqrt(sloap*sloap+1.0));
   m_Sin=1.0/sqrt(sloap*sloap+1.0);
   
   }
   else
   {
   m_Cos=fabs(sloap/sqrt(sloap*sloap+1.0));
   m_Sin=-1.0/sqrt(sloap*sloap+1.0);
  }

  
   return 0;
}

void CEllipseArc::ellipseMidpoint(CDC *pDC, int xCenter, int yCenter, int Rx, int Ry)
{
  int Rx2=Rx*Rx;
int Ry2=Ry*Ry;
int twoRx2=2*Rx2;
int twoRy2=2*Ry2;
int p;
int x=0;
int y=Ry;
int px=0;
int py=twoRx2*y;
ellipsePlotPoints(pDC,xCenter,yCenter,x,y);
p=ROUND(Ry2-(Rx2*Ry)+(0.25*Rx2));
while(px<py)
{
	x++;
	px+=twoRy2;
	if(p<0)
		p+=Ry2+px;
	else
	{
		y--;
		py-=twoRx2;
		p+=Ry2+px-py;
	}
	ellipsePlotPoints(pDC,xCenter,yCenter,x,y);
}
p=ROUND(Ry2*(x+0.5)*(x+0.5)+Rx2*(y-1)*(y-1)-Rx2*Ry2);
while(y>0)
{
	y--;
	py-=twoRx2;
	if(p>0)
		p+=Rx2-py;
	else
	{
		x++;
		px+=twoRy2;
		p+=Rx2-py+px;
	}
	ellipsePlotPoints(pDC,xCenter,yCenter,x,y);
}

}

void CEllipseArc::ellipsePlotPoints(CDC *pDC, int xCenter, int yCenter, int x, int y)
{

PdcSetPixel(pDC,xCenter+x,yCenter+y,m_PenColor);

PdcSetPixel(pDC,xCenter-x,yCenter+y,m_PenColor);

PdcSetPixel(pDC,xCenter+x,yCenter-y,m_PenColor);

PdcSetPixel(pDC,xCenter-x,yCenter-y,m_PenColor);
}

void CEllipseArc::PdcSetPixel(CDC *pDC, int xCenter, int yCenter, COLORREF m_PenColor)
{

  double vh[3][3]={m_Cos,-m_Sin,m_CenterPoint.x*(1-m_Cos)+m_CenterPoint.y*m_Sin,
		               m_Sin, m_Cos,m_CenterPoint.y*(1-m_Cos)-m_CenterPoint.x*m_Sin,
					   0,0,1
	                  };       
  
     
       
	   double xy[3]={xCenter,yCenter,1};
	   double x[3]={0,0,0};
	  x[0]=vh[0][0]*xy[0]+vh[0][1]*xy[1]+vh[0][2]*xy[2];
	  x[1]=vh[1][0]*xy[0]+vh[1][1]*xy[1]+vh[1][2]*xy[2];
	   if(x[0]-int(x[0])>=0.5)
		  x[0]+=1;
	  if(x[1]-int(x[1])>=0.5)
		  x[1]+=1;
 
	  
  if(!m_IsDrawingArc)
	 {
	  for(int i=0;i<this->m_PenWidth;i++)
	  pDC->SetPixel(x[0]+i,x[1]+i,m_PenColor);
	}   
  if(m_IsDrawingArc)
  {
	double sloap;
	if(x[0]-m_CenterPoint.x==0)
	{
	  sloap=999999999999999999.0;
	}
	else
	{
	sloap=double(x[1]-m_CenterPoint.y)/double(x[0]-m_CenterPoint.x);
	}

    double angle;
    angle=atan(fabs(sloap));
    double degree;
    degree=angle*180/3.1415927;
 if(x[0]-m_CenterPoint.x>=0&&x[1]-m_CenterPoint.y>=0)
     degree=degree;
 if(x[0]-m_CenterPoint.x>0&&x[1]-m_CenterPoint.y<=0)
     degree=360.0-degree;
 if(x[0]-m_CenterPoint.x<=0&&x[1]-m_CenterPoint.y>0)
     degree=180-degree;
 if(x[0]-m_CenterPoint.x<0&&x[1]-m_CenterPoint.y<0)
     degree=180+degree;
 if(fabs(degree-m_AngleFirst)<1)
	 this->m_IntersectionPointFirst=CPoint(x[0],x[1]);
 if(fabs(degree-m_AngleSecond)<1)
	 this->m_IntersectionPointSecond=CPoint(x[0],x[1]);
    
 if((m_AngleFirst<=m_AngleSecond)&&(degree>=m_AngleFirst&&degree<=m_AngleSecond))
  {
  
        for(int i=0;i<this->m_PenWidth;i++)
	    pDC->SetPixel(x[0]+i,x[1]+i,m_PenColor);
	
 }
 if((m_AngleFirst>=m_AngleSecond)&&(degree>=m_AngleFirst||degree<=m_AngleSecond))	 
 {
        
        for(int i=0;i<this->m_PenWidth;i++)
	    pDC->SetPixel(x[0]+i,x[1]+i,m_PenColor);
 }
 /*CRect rect(this->m_IntersectionPointFirst,this->m_IntersectionPointFirst);
 rect.InflateRect(9,9);
 pDC->Rectangle(rect);
 rect.SetRect(this->m_IntersectionPointSecond,this->m_IntersectionPointSecond);
 rect.InflateRect(9,9);
 pDC->Rectangle(rect);
*/
 
}

}

void CEllipseArc::DrawStaticStepOne(CDC *pDC,CPoint movingPoint)
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
 
 ellipseMidpoint(pDC,m_CenterPoint.x,m_CenterPoint.y,this->m_Xr,this->m_Yr);
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 pDC->SetROP2(oldDrawingMode);
}

void CEllipseArc::DrawActive(CDC *pDC,CPoint point)
{
  this->DrawActiveStepOne(pDC,point);
}

void CEllipseArc::DrawStatic(CDC *pDC)
{
  DrawStaticStepOne(pDC,this->m_FifthPoint);
}

CPoint CEllipseArc::ComputeIntersectionPointFirst(CPoint point)
{
m_IsDrawingArc=true;

double sloap;
	if(point.x==m_CenterPoint.x)
	{
	  sloap=999999999999999999.0;
	}
	else
	{
	sloap=double(point.y-m_CenterPoint.y)/double(point.x-m_CenterPoint.x);
	}

double angle;
angle=atan(fabs(sloap));

double degree;
degree=angle*180/3.1415927;
if((point.y-m_CenterPoint.y)>=0&&(point.x-m_CenterPoint.x)>=0)
     degree=degree;
if((point.y-m_CenterPoint.y)<0&&(point.x-m_CenterPoint.x)>=0)
     degree=360.0-degree;
if((point.y-m_CenterPoint.y)>=0&&(point.x-m_CenterPoint.x)<0)
     degree=180-degree;
if((point.y-m_CenterPoint.y)<0&&(point.x-m_CenterPoint.x)<0)
     degree=180+degree;

this->m_AngleFirst=degree;

return 0;
}

CPoint CEllipseArc::ComputeIntersectionPointSecond(CPoint point)
{

double sloap;
	if(point.x==m_CenterPoint.x)
	{
	  sloap=999999999999999999.0;
	}
	else
	{
	sloap=double(point.y-m_CenterPoint.y)/double(point.x-m_CenterPoint.x);
	}

double angle;
angle=atan(fabs(sloap));

double degree;
degree=angle*180/3.1415927;
if((point.y-m_CenterPoint.y)>=0&&(point.x-m_CenterPoint.x)>=0)
     degree=degree;
if((point.y-m_CenterPoint.y)<0&&(point.x-m_CenterPoint.x)>=0)
     degree=360.0-degree;
if((point.y-m_CenterPoint.y)>=0&&(point.x-m_CenterPoint.x)<0)
     degree=180-degree;
if((point.y-m_CenterPoint.y)<0&&(point.x-m_CenterPoint.x)<0)
     degree=180+degree;

this->m_AngleSecond=degree;



return 0;
}

BOOL CEllipseArc::IsInRgn(CPoint point)
{
  
	double vh[3][3]={m_Cos,-m_Sin,m_CenterPoint.x*(1-m_Cos)+m_CenterPoint.y*m_Sin,
		               m_Sin, m_Cos,m_CenterPoint.y*(1-m_Cos)-m_CenterPoint.x*m_Sin,
					   0,0,1
	                  };       
  
     
       
  double xy[3];
  CPoint point1=CPoint(m_CenterPoint.x-m_Xr,m_CenterPoint.y+m_Yr);
  CPoint point2=CPoint(m_CenterPoint.x+m_Xr,m_CenterPoint.y+m_Yr);
  CPoint point3=CPoint(m_CenterPoint.x+m_Xr,m_CenterPoint.y-m_Yr);
  CPoint point4=CPoint(m_CenterPoint.x-m_Xr,m_CenterPoint.y-m_Yr);
  CPoint point11=CPoint(m_CenterPoint.x-m_Xr/2,m_CenterPoint.y+m_Yr/2);
  CPoint point22=CPoint(m_CenterPoint.x+m_Xr/2,m_CenterPoint.y+m_Yr/2);
  CPoint point33=CPoint(m_CenterPoint.x+m_Xr/2,m_CenterPoint.y-m_Yr/2);
  CPoint point44=CPoint(m_CenterPoint.x-m_Xr/2,m_CenterPoint.y-m_Yr/2);
	   
  CPoint pointList1[]={point1,point2,point3,point4};
  CPoint pointList2[]={point11,point22,point33,point44};
  
  for(int i=0;i<4;i++)
  {
  xy[0]=pointList1[i].x;
  xy[1]=pointList1[i].y;
  xy[2]=1;
  pointList1[i].x=vh[0][0]*xy[0]+vh[0][1]*xy[1]+vh[0][2]*xy[2];
  pointList1[i].y=vh[1][0]*xy[0]+vh[1][1]*xy[1]+vh[1][2]*xy[2];
  
  xy[0]=pointList2[i].x;
  xy[1]=pointList2[i].y;
  xy[2]=1;
  pointList2[i].x=vh[0][0]*xy[0]+vh[0][1]*xy[1]+vh[0][2]*xy[2];
  pointList2[i].y=vh[1][0]*xy[0]+vh[1][1]*xy[1]+vh[1][2]*xy[2];
  }

  CRgn rgn1;
  rgn1.CreatePolygonRgn(pointList1,4,ALTERNATE);
  CRgn rgn2;
  rgn2.CreatePolygonRgn(pointList2,4,ALTERNATE);
  if(rgn1.PtInRegion(point)&&!rgn2.PtInRegion(point))
  return true;
  
  return false;
}



int CEllipseArc::IsOnMarginPoint(CPoint point)
{
 CRect rect(point,point);
 rect.InflateRect(4,4);
 if(rect.PtInRect(this->m_IntersectionPointFirst))
 {
	 this->m_FoundPoint=this->m_IntersectionPointFirst;
	 return 1;
 }

 if(rect.PtInRect(this->m_IntersectionPointSecond))
 {
	 this->m_FoundPoint=this->m_IntersectionPointSecond;
	 return 2;
 }
 if(rect.PtInRect(this->m_CenterPoint))
 { 
	 this->m_FoundPoint=this->m_CenterPoint;
	 return 3;
 }
  return 0;
}

void CEllipseArc::ShowSelectPoint(CDC *pDC)
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
rect.SetRect(this->m_IntersectionPointFirst,this->m_IntersectionPointFirst);
rect.InflateRect(3,3);
pDC->Rectangle(rect);
rect.SetRect(this->m_IntersectionPointSecond,this->m_IntersectionPointSecond);
rect.InflateRect(3,3);
pDC->Rectangle(rect);
rect.SetRect(this->m_CenterPoint,this->m_CenterPoint);
pDC->SelectObject(OldBrush);
brush.DeleteObject();
brush.CreateSolidBrush(RGB(255,0,0));
pDC->SelectObject(&brush);
rect.InflateRect(3,3);

pDC->Rectangle(rect);
pDC->SelectObject(OldPen);
pDC->SetBkMode(oldBkMode); 
pDC->SelectObject(OldBrush);
pDC->SetROP2(oldDrawingMode);
}

void CEllipseArc::OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point)
{
 if(m_HaveFindFirst)
	  {
	   pView->L_iPointCount=IsOnMarginPoint(point);
	   if(pView->L_iPointCount!=0)
	   {
		pView->m_CurrentDrawStatus=Change_Status;
		pView->m_CurrentDrawTool=EllipseArc_Tool;
		pView->m_pCurrentUnit=this;
        pView->m_bHaveFindSecond=true;
       
		if(pView->L_iPointCount==3)
			pView->m_CurrentDrawStatus=Drag_Status;
	        pView->m_pCurrentUnit=this;
	        ShowSelectPoint(pDC);
	        m_DrawingMode=pDC->GetROP2();
	        DrawStatic(pDC);
			DrawOldReferencePoint(pDC,m_FoundPoint);
	   }
	  }
     if(!pView->m_bHaveFindSecond&&IsInRgn(point))
	 {
	   if(m_HaveFindFirst)
	   {
	    m_HaveFindFirst=false;
		pView->m_bHaveFindFirst=true;
	   }
       else 
	   {  
		   pView->m_bHaveFindFirst=true;
		   m_HaveFindFirst=true;
	   }
	   ShowSelectPoint(pDC);
	 }
	 
}

void CEllipseArc::OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point)
{
    if(pView->m_CurrentDrawStatus==Drag_Status)
	{
	   DrawStatic(pDC);
	   if(!m_IsCirCu)
	   m_CenterPoint.Offset(pView->m_SecondPoint.x-pView->m_FirstPoint.x,pView->m_SecondPoint.y-pView->m_FirstPoint.y);
       
       if(m_IsCirCu)
	   {
	    ComputSloap(pView->m_LastDownPoint,point);
	
        ComputRadiusX(pView->m_LastDownPoint,point);
	   }
       DrawStatic(pDC);

	}
	else
	   {
	    //pDC->SetROP2(R2_NOTXORPEN); 
	    if(pView->m_LBDownTimes==1&&pView->m_CurrentDrawStatus==Change_Status)
		{ 
		 DrawStatic(pDC);
	    if(!m_IsCirCu)
		{
			if(pView->L_iPointCount==1)
			ComputeIntersectionPointFirst(point);
		    if(pView->L_iPointCount==2)
		    ComputeIntersectionPointSecond(point);
		}
		if(m_IsCirCu)
		{
		ComputSloap(this->m_CenterPoint,point);
		}
		pView->m_SecondPoint=point;
		DrawStatic(pDC);
		}
	    if(pView->m_LBDownTimes==3&&pView->m_CurrentDrawStatus!=Change_Status)
		{
	      DrawActive(pDC,pView->m_SecondPoint);
		  ComputeIntersectionPointSecond(point);
		  m_DrawingMode=pDC->GetROP2();
		  m_FifthPoint=point;
		  pView->m_SecondPoint=point;
		  DrawActive(pDC,point);
		}
	   if(pView->m_LBDownTimes==2&&pView->m_CurrentDrawStatus!=Change_Status)
		  {
		 // m_pCurrentUnit->ComputRadiusX(m_pCurrentUnit->m_CenterPoint,point);
		  DrawActiveStepOne(pDC,pView->m_SecondPoint);
	      ComputRadiusX(m_CenterPoint,point);
		  m_DrawingMode=pDC->GetROP2();
		  pView->m_SecondPoint=point;
		  DrawActiveStepOne(pDC,point);
		 }

       if(pView->m_LBDownTimes==1&&pView->m_CurrentDrawStatus!=Change_Status)
		 {
          ShowMovingLine(pDC,pView->m_FirstPoint,pView->m_SecondPoint);
		  pView->m_SecondPoint=point;
		  ShowMovingLine(pDC,pView->m_FirstPoint,point);
		 }
	   }
}

void CEllipseArc::Serialize(CArchive &ar)
{
    CUnit::Serialize(ar);
    if(ar.IsStoring())
	{
	  ar<<m_BrushStyle<<m_BkMode<<m_BrushHatch<<m_BrushColor;
	  ar<<m_Sin<<m_Cos;
	  ar<<m_Xr<<m_Yr;
	  ar<<m_CenterPoint;
	  ar<<m_IntersectionPointFirst<<m_IntersectionPointSecond;
	  ar<<m_AngleFirst<<m_AngleSecond;
	} 
	else
	{
	  ar>>m_BrushStyle>>m_BkMode>>m_BrushHatch>>m_BrushColor;
	  ar>>m_Sin>>m_Cos;
	  ar>>m_Xr>>m_Yr;
	  ar>>m_CenterPoint;
	  ar>>m_IntersectionPointFirst>>m_IntersectionPointSecond;
	  ar>>m_AngleFirst>>m_AngleSecond;
	  this->m_IsDrawingArc=true;
	}
}

void CEllipseArc::OnMenuitemCirCu(CDC *pDC, CEastDrawView *pView)
{
  m_IsCirCu=!m_IsCirCu;
}

void CEllipseArc::OnContextMenu(CWnd *pWnd, CPoint point)
{
CMenu menuTextEdit;
menuTextEdit.LoadMenu(IDR_MENU_Canvas_Edit);
menuTextEdit.CheckMenuItem(ID_CirCu,m_IsCirCu?MF_CHECKED:MF_UNCHECKED);
menuTextEdit.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,pWnd);
}
