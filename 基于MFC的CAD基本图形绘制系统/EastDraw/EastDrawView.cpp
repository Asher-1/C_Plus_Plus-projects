// EastDrawView.cpp : implementation of the CEastDrawView class
//

#include "stdafx.h"
#include "EastDraw.h"

#include "EastDrawDoc.h"
#include "EastDrawView.h"
#include "ReginSet.h"
#include "EditBox.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEastDrawView

IMPLEMENT_DYNCREATE(CEastDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CEastDrawView, CScrollView)
	//{{AFX_MSG_MAP(CEastDrawView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BUTTON_DLine, OnBUTTONDLine)
	ON_COMMAND(ID_BUTTON32774_Move, OnBUTTON32774Move)
	ON_COMMAND(ID_BUTTON32775_CircumRotate, OnBUTTON32775CircumRotate)
	ON_COMMAND(ID_BUTTON32777_Curve, OnBUTTON32777Curve)
	ON_COMMAND(ID_BUTTON32776_Rectangle, OnBUTTON32776Rectangle)
	ON_COMMAND(ID_BUTTON32778_RoundRectangle, OnBUTTON32778RoundRectangle)
	ON_COMMAND(ID_BUTTON32779_Ellipse, OnBUTTON32779Ellipse)
	ON_COMMAND(ID_BUTTON32780_Round, OnBUTTON32780Round)
	ON_COMMAND(ID_BUTTON32791_Polygon, OnBUTTON32791Polygon)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_MENUITEM32794_SetRegin, OnMENUITEM32794SetRegin)
	ON_COMMAND(ID_BUTTON_LiEllipse, OnBUTTONLiEllipse)
	ON_COMMAND(ID_BUTTON32800_RoundArc, OnBUTTON32800RoundArc)
	ON_COMMAND(ID_BUTTON32805_EllipseArc, OnBUTTON32805EllipseArc)
	ON_COMMAND(ID_BUTTON32807_PolyBezier, OnBUTTON32807PolyBezier)
	ON_COMMAND(ID_BUTTON32809_Text, OnBUTTON32809Text)
	ON_COMMAND(ID_CHAR_BOLD, OnCharBold)
	ON_COMMAND(ID_CHAR_ITALIC, OnCharItalic)
	ON_COMMAND(ID_CHAR_UNDERLINE, OnCharUnderline)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENUITEM_OK, OnMenuitemOk)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_CirCu, OnCirCu)
	ON_UPDATE_COMMAND_UI(ID_CirCu, OnUpdateCirCu)
	ON_WM_KEYUP()
	ON_COMMAND(ID_Delete, OnDelete)
	ON_COMMAND(ID_BUTTON32830_Lock, OnBUTTON32830Lock)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32830_Lock, OnUpdateBUTTON32830Lock)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DLine, OnUpdateBUTTONDLine)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32776_Rectangle, OnUpdateBUTTON32776Rectangle)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32778_RoundRectangle, OnUpdateBUTTON32778RoundRectangle)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32791_Polygon, OnUpdateBUTTON32791Polygon)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32779_Ellipse, OnUpdateBUTTON32779Ellipse)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32780_Round, OnUpdateBUTTON32780Round)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32777_Curve, OnUpdateBUTTON32777Curve)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LiEllipse, OnUpdateBUTTONLiEllipse)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32800_RoundArc, OnUpdateBUTTON32800RoundArc)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32805_EllipseArc, OnUpdateBUTTON32805EllipseArc)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32807_PolyBezier, OnUpdateBUTTON32807PolyBezier)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32809_Text, OnUpdateBUTTON32809Text)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32774_Move, OnUpdateBUTTON32774Move)
	ON_COMMAND(ID_BUTTON32845_Axis, OnBUTTON32845Axis)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32845_Axis, OnUpdateBUTTON32845Axis)
	ON_COMMAND(ID_BUTTON32850_ReDraw, OnBUTTON32850ReDraw)
	ON_COMMAND(ID_BUTTON32856_DONothing, OnBUTTON32856DONothing)
	ON_COMMAND(ID_BUTTON32858_OnDo, OnBUTTON32858OnDo)
	ON_COMMAND(ID_BUTTON32859_ReDo, OnBUTTON32859ReDo)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32858_OnDo, OnUpdateBUTTON32858OnDo)
	ON_UPDATE_COMMAND_UI(ID_BUTTON32859_ReDo, OnUpdateBUTTON32859ReDo)
	ON_COMMAND(ID_MouseCap_On, OnMouseCapOn)
	ON_UPDATE_COMMAND_UI(ID_MouseCap_On, OnUpdateMouseCapOn)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEastDrawView construction/destruction

#include "MainFrm.h"
#include "CCurve.h"
#include "CDLine.h"
#include "CRectangle.h"
#include "CRoundRectangle.h"
#include "CEllipse.h"
#include "CRound.h"
#include "CPolygon.h"
#include"CLiEllipse.h"
#include"CRoundArc.h"
#include"CEllipseArc.h"
#include "CPolyBezier.h"
//#include "TextEdit.h"
#include "CText.h"
CEastDrawView::CEastDrawView()
{
	// TODO: add construction code here
m_IsLastMove=false;
m_Rtimes=0;
m_Index=0;
m_pCurrentUnit=new CUnit;
m_HaveAxis=true;
m_Bitmap=NULL;
m_IsLocked=false;
CMainFrame*pMainFrame=STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
m_DLineProperty=pMainFrame->m_DLineProperty;

m_IsMouseCap=true;
}

CEastDrawView::~CEastDrawView()
{
this->ReMoveTemp();
}

BOOL CEastDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
 
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEastDrawView drawing

void CEastDrawView::OnDraw(CDC* pDC)
{
	CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDC->SetWindowOrg(m_CenterPoint);

    this->DrawAll(pDC);
	
	if(this->m_HaveAxis)
		this->DrawAxis(pDC);


	
}

void CEastDrawView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy =2000;
	SetScrollSizes(MM_TEXT,pDoc->m_TotalSize);
    m_CenterPoint=pDoc->m_CenterPoint; 
	//SetScrollSizes(MM_HIENGLISH, sizeTotal);
    m_bHaveFindFirst=FALSE;
	m_bHaveFindSecond=false;
	m_LBDownTimes=0;

    m_CurrentDrawTool=0;
	m_CurrentDrawStatus=0;

    ScrollToPosition(CPoint(900,600));
  
	AddTempFile();
	AutoSave();
  }

/////////////////////////////////////////////////////////////////////////////
// CEastDrawView printing


void CEastDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEastDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEastDrawView diagnostics

#ifdef _DEBUG
void CEastDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CEastDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CEastDrawDoc* CEastDrawView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEastDrawDoc)));
	return (CEastDrawDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEastDrawView message handlers

//******************************************************************************
//******************************************************************************
//**********void CEastDrawView::OnLButtonDown(UINT nFlags, CPoint point)********************************************************************
//******************************************************************************
//******************************************************************************

void CEastDrawView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
      CEastDrawDoc*pDoc=this->GetDocument();  
	  CPoint Devpoint=point;
	  CClientDC dc(this);
	  this->OnPrepareDC(&dc);
      dc.SetWindowOrg(m_CenterPoint);
	  dc.DPtoLP(&point);
      dc.SetROP2(R2_NOTXORPEN);
	  this->ComputeMinandMaxPoint(point);
	  m_ChangeingPoint=point;
      this->SetMousePosText(point,Devpoint);
	  
    

	if(this->m_CurrentDrawStatus==Draw_Status)
	{
      if(this->m_CurrentDrawTool!=Polygon_Tool&&this->m_CurrentDrawTool!=PolyBezier_Tool)
	  {
	      if(this->m_CurrentDrawTool==EllipseArc_Tool)
		  {
		     if(m_LBDownTimes==3)
			 {
			   this->m_pCurrentUnit->DrawActive(&dc,point);
			   this->m_pCurrentUnit->DrawStatic(&dc);
               this->m_LBDownTimes=-1;
			   //if(m_DLineProperty->m_Mode->m_bMode)
			   //this->m_pCurrentUnit->DrawStatic(&dc);
			 }
		  
		  }
		  if(this->m_CurrentDrawTool==Ellipse_Tool||m_CurrentDrawTool==RoundArc_Tool)
		 {
	     	if(m_LBDownTimes==2)
			{
		       this->ComputeMinandMaxPoint(point);
			   this->m_pCurrentUnit->DrawActive(&dc,point);
			   this->m_pCurrentUnit->DrawStatic(&dc);
			   this->InvalidateMovingRect(m_pCurrentUnit);
				m_LBDownTimes=-1;
				//if(m_DLineProperty->m_Mode->m_bMode)
				//this->m_pCurrentUnit->DrawStatic(&dc);
			}
		 }
         if(this->m_CurrentDrawTool!=Text_Tool&&this->m_CurrentDrawTool!=Ellipse_Tool&&this->m_CurrentDrawTool!=RoundArc_Tool&&this->m_CurrentDrawTool!=EllipseArc_Tool)
		 { 
	       if(m_LBDownTimes==1)
		   {   
			   //this->ComputeMinandMaxPoint(point);
			   //this->InvalidateMovingRect(m_pCurrentUnit);
			   m_LBDownTimes=-1;
			   //if(m_DLineProperty->m_Mode->m_bMode)
			   //this->m_pCurrentUnit->DrawStatic(&dc);
		   }		 
		 }//*****if(this->m_CurrentDrawTool!=Ellipse_Tool)
	  }//*****if(this->m_CurrentDrawTool!=Polygon_Tool)
	}//*******if(this->m_CurrentDrawStatus==Draw_Status)
  if(this->m_CurrentDrawStatus==Select_Status)
   {
     m_LBDownTimes=0;
	 this->m_CurrentDrawTool=0;
   }

  
if(m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Drag_Status)
   {   
	   
		   if((m_CurrentDrawTool==Round_Tool||m_CurrentDrawTool==Ellipse_Tool)&&m_CurrentDrawStatus==Change_Status)
		   {
		  m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_CenterPoint,m_pCurrentUnit->m_FoundPoint);
		  m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_CenterPoint,point);
		   
		   }
		  
		   m_LBDownTimes=-1;
		   m_bHaveFindFirst=false;
		   m_CurrentDrawStatus=Select_Status;
		   m_pCurrentUnit->ShowSelectPoint(&dc);
		   m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_FoundPoint,point);
		   m_pCurrentUnit->DrawOldReferencePoint(&dc,m_pCurrentUnit->m_FoundPoint);
		   this->ComputeMinandMaxPoint(point);
		   this->InvalidateMovingRect(m_pCurrentUnit);
        if(m_CurrentDrawTool==RoundArc_Tool)
		{
		  m_pCurrentUnit->DrawActive(&dc,point);
		  m_pCurrentUnit->DrawStatic(&dc);
		}
	}
    
     if(m_LBDownTimes==-1||m_CurrentDrawTool==0||m_CurrentDrawTool==Text_Tool||m_CurrentDrawStatus==Select_Status)
	 {
		 if(m_IsMouseCap)
		 ::ReleaseCapture();
		 
	}
     if(m_LBDownTimes==-1)
	 {
		 ExchangeSave(); 
		 
	 }
      m_LBDownTimes++;
	  m_bHaveFindSecond=false;
	  m_bHaveFindFirst=false;
	  m_LastDownPoint=point;
	  L_iPointCount=-1;
      if(m_LBDownTimes==1&&m_CurrentDrawTool!=Text_Tool&&m_CurrentDrawStatus!=Select_Status)
	  { 
	  m_FirstPoint=point;
	  m_SecondPoint=point;
	  m_MinPoint=point;
	  this->m_MaxPoint=point;
	  if(m_IsMouseCap)
	  this->SetCapture();
	  }
	  this->ComputeMinandMaxPoint(point);
	  
if(m_DLineProperty->m_DLineDlg2->m_Draw_Status==Select_Color_Status)
  {
    
    m_DLineProperty->m_DLineDlg2->OnChange(dc.GetPixel(point));
    m_CurrentDrawStatus=Select_Color_Status;
	m_CurrentDrawTool=0;
  }	  
 
//******************************************************************************
//******************************************************************************

dc.SetROP2(R2_NOTXORPEN);


if((m_LBDownTimes==1)&&(m_CurrentDrawStatus==Select_Status))
{  

	for(int i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_PolyBezierArray.GetSize();i++)
	{
	 CPolyBezier*L_pPolyBezier=(CPolyBezier*)pDoc->m_PolyBezierArray.GetAt(i);
	 L_pPolyBezier->OnLButtonDown(&dc,this,point);
	}
    
	for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_TextArray.GetSize();i++)
	{
     CText*L_pText=(CText*)pDoc->m_TextArray.GetAt(i);
	 L_pText->OnLButtonDown(&dc,this,point);
	}

    for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_EllipseArcArray.GetSize();i++)
	{
	  CEllipseArc*L_pEllipseArc=(CEllipseArc*)pDoc->m_EllipseArcArray.GetAt(i);
	  L_pEllipseArc->OnLButtonDown(&dc,this,point);
	}//for(int i=0;!m_bHaveFindSecond&&pDoc->m_EllipseArcArray.GetSize();i++)
	
	for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_RoundRectangleArray.GetSize();i++)
    { 
     CRoundRectangle*L_pRoundRectangle=(CRoundRectangle*)pDoc->m_RoundRectangleArray.GetAt(i);
     L_pRoundRectangle->OnLButtonDown(&dc,this,point);   
	}//******for(int i=0;!l_bHaveFinf&&i<pDoc->m_RoundRectangleArray.GetSize();i++)  
 
   for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_RoundArcArray.GetSize();i++)
   {
    CRoundArc*L_pRoundArc=(CRoundArc*)pDoc->m_RoundArcArray.GetAt(i);
    L_pRoundArc->OnLButtonDown(&dc,this,point);
   }//******for(i=0;!m_bHaveFindSecond&&i<pDoc->m_RoundArcArray.GetSize();i++)
  
  for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_LiEllipseArray.GetSize();i++)
  {
   CLiEllipse*L_pLiEllipse=(CLiEllipse*)pDoc->m_LiEllipseArray.GetAt(i);
   L_pLiEllipse->OnLButtonDown(&dc,this,point);
  } //******for(i=0;!l_bHaveFind&&i<pDoc->m_LiEllipseArray.GetSize();i++)
  
for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_EllipseArray.GetSize();i++)
 {
   CEllipse*L_pEllipse=(CEllipse*)pDoc->m_EllipseArray.GetAt(i);
   L_pEllipse->OnLButtonDown(&dc,this,point);
 }//******for(i=0;!m_bHaveFindFirst&&i<pDoc->m_EllipseArray.GetSize();i++)
  


 for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_RoundArray.GetSize();i++)
 {
    CRound*L_pRound=(CRound*)pDoc->m_RoundArray.GetAt(i);
	L_pRound->OnLButtonDown(&dc,this,point);
 }//******for(i=0;!m_bHaveFindFirst&&i<pDoc->m_RoundArray.GetSize();i++)
 
 for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_DLineArray.GetSize();i++)
 { 

       CDLine*L_pDLine=(CDLine*)pDoc->m_DLineArray.GetAt(i);
       L_pDLine->OnLButtonDown(&dc,this,point);
 } //******for(int i=0;!m_bHaveFindFirst&&i<pDoc->m_DLineArray.GetSize();i++)

  
 for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_CurveArray.GetSize();i++)
  {
	CCurve*L_pCurve=(CCurve*)pDoc->m_CurveArray.GetAt(i);
	 L_pCurve->OnLButtonDown(&dc,this,point);

	
 }//*****for(i=0;!m_bHaveFindFirst&&i<pDoc->m_CurveArray.GetSize();i++)

 for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_RectangleArray.GetSize();i++)
 {
    CRectangle *L_pRectangle=(CRectangle*)pDoc->m_RectangleArray.GetAt(i);
	L_pRectangle->OnLButtonDown(&dc,this,point);
	
}//*****for(i=0;!m_bHaveFindFirst&&i<pDoc->m_RectangleArray.GetSize();i++)

for(i=0;!m_bHaveFindSecond&&!m_bHaveFindFirst&&i<pDoc->m_PolygonArray.GetSize();i++)
{
  CPolygon*L_pPolygon=(CPolygon*)pDoc->m_PolygonArray.GetAt(i);
  L_pPolygon->OnLButtonDown(&dc,this,point);

} //******for(i=0;!m_bHaveFindFirst&&i<pDoc->m_PolygonArray.GetSize();i++)

} //******if((m_CurrentDrawStatus==Select_Status))


 //***************************************************************************************
 //***************************************************************************************

if(m_CurrentDrawStatus==Draw_Status)
{    
	if(m_CurrentDrawTool==PolyBezier_Tool)
	{ 
	  if(this->m_LBDownTimes==1)
	  {   
		  m_pCurrentUnit=new CPolyBezier;
	      pDoc->m_PolyBezierArray.Add((CPolyBezier*)m_pCurrentUnit);
	  }
	  this->m_FirstPoint=this->m_SecondPoint=point;
	  m_pCurrentUnit->OnLButtonDown(&dc,this,point);
	}
    if(m_CurrentDrawTool==Text_Tool)
	{
	  if(this->m_LBDownTimes==1)
	  {   
		m_pCurrentUnit=new CText;
	    pDoc->m_TextArray.Add((CText*)m_pCurrentUnit);

	  }
      m_pCurrentUnit->OnLButtonDown(&dc,this,point);

	  }


	if(m_CurrentDrawTool==EllipseArc_Tool)
	{
      if(this->m_LBDownTimes==1)
	  {
		  m_pCurrentUnit=new CEllipseArc;
		  pDoc->m_EllipseArcArray.Add((CEllipseArc*)m_pCurrentUnit);
	  }
	 if(this->m_LBDownTimes==2)
	 {
	   m_pCurrentUnit->ComputSloap(m_pCurrentUnit->m_FirstPoint,point);
       m_pCurrentUnit->m_CenterPoint=CPoint((m_pCurrentUnit->m_FirstPoint.x+point.x)/2,(m_pCurrentUnit->m_FirstPoint.y+point.y)/2);
       m_pCurrentUnit->ComputRadiusY(m_pCurrentUnit->m_CenterPoint,point);
	 
	 }
	 if(this->m_LBDownTimes==3)
	 {
	     m_pCurrentUnit->m_FourthPoint=point;
		 m_pCurrentUnit->m_FifthPoint=point;
		 m_pCurrentUnit->ComputeIntersectionPointFirst(point);
		 m_pCurrentUnit->ComputeIntersectionPointSecond(point);
	 }
	}
	if(m_CurrentDrawTool==RoundArc_Tool)
	{
	  if(this->m_LBDownTimes==1)
	  {
	    m_pCurrentUnit=new CRoundArc;
		pDoc->m_RoundArcArray.Add((CRoundArc*)m_pCurrentUnit);
	  }
	  if(this->m_LBDownTimes==2)
	  {
		
	    m_pCurrentUnit->m_SecondPoint=point;
        m_pCurrentUnit->ComputeSloap1(m_pCurrentUnit->m_FirstPoint,m_pCurrentUnit->m_SecondPoint);
	    m_pCurrentUnit->m_ThirdPoint=m_pCurrentUnit->m_FirstPoint;
		
	  }
	
	}//******if(m_CurrentDrawTool==RoundArc_Tool)
     
	if(m_CurrentDrawTool==Polygon_Tool)
	 {
		 if(this->m_LBDownTimes==1)
		 {
          m_pCurrentUnit=new CPolygon;
          pDoc->m_PolygonArray.Add((CPolygon*)m_pCurrentUnit);
          m_pCurrentUnit->m_PointList->Add(point);
		 }
	    if(this->m_LBDownTimes!=0&&this->m_LBDownTimes!=1)
		 {
	       m_pCurrentUnit->m_PointList->Add(point);
	     }
	 }//******if(m_CurrentDrawTool==Polygon_Tool)
     if(m_CurrentDrawTool==Ellipse_Tool)
	 { 
	   if(m_LBDownTimes==1)
	   {
	   m_pCurrentUnit=new CEllipse;
	   pDoc->m_EllipseArray.Add((CEllipse*)m_pCurrentUnit);
       }
	   if(m_LBDownTimes==2)
	   {
	   m_pCurrentUnit->ComputSloap(m_pCurrentUnit->m_FirstPoint,point);
       m_pCurrentUnit->m_CenterPoint=CPoint((m_pCurrentUnit->m_FirstPoint.x+point.x)/2,(m_pCurrentUnit->m_FirstPoint.y+point.y)/2);
       m_pCurrentUnit->ComputRadiusX(m_pCurrentUnit->m_CenterPoint,point);
       m_pCurrentUnit->ComputRadiusY(m_pCurrentUnit->m_CenterPoint,point);
       m_pCurrentUnit->ellipseMidpoint(&dc,m_pCurrentUnit->m_CenterPoint.x,m_pCurrentUnit->m_CenterPoint.y,m_pCurrentUnit->m_Xr,m_pCurrentUnit->m_Yr);
       }
	   if(this->m_LBDownTimes==0)
	   {
	    m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_FirstPoint,m_pCurrentUnit->m_SecondPoint);
	   }
       
	 }//******if(m_CurrentDrawTool==Ellipse_Tool)
	 if(m_CurrentDrawTool==Round_Tool)
	 {
	  if(m_LBDownTimes==1)
	  {
	  m_pCurrentUnit=new CRound;
	  pDoc->m_RoundArray.Add((CRound*)m_pCurrentUnit);
	  m_pCurrentUnit->m_CenterPoint=point;
	  }
	  if(m_LBDownTimes==0)
	  {
	  m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_CenterPoint,point);
      }
	 }
  if(m_CurrentDrawTool==RoundRectangle_Tool)
	{
	 if(m_LBDownTimes==1)
	 {
	 m_pCurrentUnit=new CRoundRectangle;
     pDoc->m_RoundRectangleArray.Add((CRoundRectangle*)m_pCurrentUnit);
	 }
	 
	}

  if(m_CurrentDrawTool==LiEllipse_Tool)
  {
     if(m_LBDownTimes==1)
	 {
	  m_pCurrentUnit=new CLiEllipse;
	  pDoc->m_LiEllipseArray.Add((CLiEllipse*)m_pCurrentUnit);
	 }
  
  }
    if(m_CurrentDrawTool==DLine_Tool)
	{
		if(m_LBDownTimes==1)
		{
	     m_pCurrentUnit=new CDLine;
         pDoc->m_DLineArray.Add((CDLine*)m_pCurrentUnit);
		}
	} 

    if(m_CurrentDrawTool==Curve_Tool)
	{  
		if(m_LBDownTimes==1)
		{
	  m_pCurrentUnit=(CCurve*)new CCurve;
	  pDoc->m_CurveArray.Add((CCurve*)m_pCurrentUnit);
	  m_pCurrentUnit->m_PointList->Add(point);
	  m_pCurrentUnit->m_PointList->Add(point);
		}
	}//******if(m_CurrentDrawTool==Curve_Tool)

	if(m_CurrentDrawTool==Rectangle_Tool)
	{
	 if(m_LBDownTimes==1)
	 {
	  m_pCurrentUnit=(CRectangle*)new CRectangle;
	  pDoc->m_RectangleArray.Add((CRectangle*)m_pCurrentUnit);
     }
	}//******if(m_CurrentDrawTool==Rectangle_Tool)

 
}//*****if(m_CurrentDrawStatus==Draw_Status)
 
  if((m_CurrentDrawStatus==Draw_Status&&m_LBDownTimes==1)||(m_bHaveFindSecond&&!m_IsLocked))
	  {
	  m_DLineProperty->m_DLineDlg1->UpdateData();
      m_pCurrentUnit->m_FirstPoint=m_FirstPoint;
	  m_pCurrentUnit->m_SecondPoint=m_SecondPoint;
      m_pCurrentUnit->m_PenStyle=m_DLineProperty->m_DLineDlg1->m_PenStyle;
	  m_pCurrentUnit->m_PenWidth=m_DLineProperty->m_DLineDlg1->m_Edit_LineWidth;
	  m_pCurrentUnit->m_PenColor=m_DLineProperty->m_DLineDlg2->m_PenColor;
	  m_pCurrentUnit->m_BrushColor=m_DLineProperty->m_DLineDlg2->m_BrushColor;
	  m_pCurrentUnit->m_BrushHatch=m_DLineProperty->m_DLineDlg1->m_BrushHatch;
	  m_pCurrentUnit->m_BrushStyle=m_DLineProperty->m_DLineDlg1->m_BrushStyle;
	  m_pCurrentUnit->m_BackgroundColor=m_DLineProperty->m_DLineDlg2->m_BackgroundColor;
	  //if(m_DLineProperty->m_Mode->m_bMode)
	 
		 // m_pCurrentUnit->m_DrawingMode=m_DLineProperty->m_Mode->m_DrawingMode;
	      
	  //else
		  m_pCurrentUnit->m_DrawingMode=dc.GetROP2();
	  m_pCurrentUnit->m_BkMode=m_DLineProperty->m_DLineDlg1->m_BKMode;//m_DLineProperty->m_Mode->m_BKMode;
	  
	  }//**********if(this->m_LBDownTimes==1&&m_bHaveFindFirst)
     
    if(m_bHaveFindSecond&&m_IsLocked)
	{
	  m_DLineProperty->m_DLineDlg1->UpdateData();
      m_pCurrentUnit->m_FirstPoint=m_FirstPoint;
	  m_pCurrentUnit->m_SecondPoint=m_SecondPoint;
      m_DLineProperty->m_DLineDlg1->m_PenStyle=m_pCurrentUnit->m_PenStyle;
	  m_DLineProperty->m_DLineDlg1->m_Edit_LineWidth=m_pCurrentUnit->m_PenWidth;
	  m_DLineProperty->m_DLineDlg2->m_PenColor=m_pCurrentUnit->m_PenColor;
	  m_DLineProperty->m_DLineDlg2->m_BrushColor=m_pCurrentUnit->m_BrushColor;
	  m_DLineProperty->m_DLineDlg1->m_BrushHatch=m_pCurrentUnit->m_BrushHatch;
	  m_DLineProperty->m_DLineDlg1->m_BrushStyle=m_pCurrentUnit->m_BrushStyle;
	  m_DLineProperty->m_DLineDlg2->m_BackgroundColor=m_pCurrentUnit->m_BackgroundColor;
	  m_pCurrentUnit->m_DrawingMode=dc.GetROP2();
	  m_DLineProperty->m_DLineDlg1->m_BKMode=m_pCurrentUnit->m_BkMode;
	  // if(m_DLineProperty->m_Mode->m_bMode)
	  //m_DLineProperty->m_Mode->m_DrawingMode=m_pCurrentUnit->m_DrawingMode;
	  
	  m_DLineProperty->GetActivePage()->UpdateData(false);
	  m_DLineProperty->GetActivePage()->Invalidate();
    

	 }
  
    if(m_bHaveFindSecond)
	 {
	  m_pCurrentUnit->DrawStatic(&dc);
	 }

if(m_CurrentDrawStatus==Drag_Status)
 {    
	  m_FirstPoint=m_pCurrentUnit->m_FoundPoint;
	  m_SecondPoint=m_pCurrentUnit->m_FoundPoint;
 }
if(m_CurrentDrawStatus==Change_Status)//&&m_CurrentDrawTool==Ellipse_Tool)
{
      m_FirstPoint=m_pCurrentUnit->m_FoundPoint;
	  m_SecondPoint=m_pCurrentUnit->m_FoundPoint;
	  
	  if(this->m_CurrentDrawTool==Text_Tool)
	  {
	   m_pCurrentUnit->OnLButtonDown(&dc,this,point);
	   
	  }
}
  if(m_IsMouseCap&&m_bHaveFindSecond)
	  this->SetCapture();


CScrollView::OnLButtonDown(nFlags, point);
}


//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CEastDrawView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	  
	  CPoint Devpoint=point;
	  CClientDC dc(this);
	  this->OnPrepareDC(&dc);
      dc.SetWindowOrg(m_CenterPoint);
	  dc.DPtoLP(&point);
      dc.SetROP2(R2_NOTXORPEN);
	  this->ComputeMinandMaxPoint(point);
	  m_ChangeingPoint=point;
      this->SetMousePosText(point,Devpoint);

 if(m_DLineProperty->m_DLineDlg2->m_Draw_Status==Select_Color_Status)
  {
      m_DLineProperty->m_DLineDlg2->m_Color=dc.GetPixel(point);
	  m_DLineProperty->m_DLineDlg2->OnChange(dc.GetPixel(point));
      m_CurrentDrawStatus=0;
      m_DLineProperty->m_DLineDlg2->m_Draw_Status=0;
	  m_CurrentDrawTool=0;
 }
 if(m_IsMouseCap&&m_bHaveFindSecond)
	  this->SetCapture();
  CScrollView::OnLButtonUp(nFlags, point);
}

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

void CEastDrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	  CPoint Devpoint=point;
	  CClientDC dc(this);
	  this->OnPrepareDC(&dc);
      dc.SetWindowOrg(m_CenterPoint);
	  dc.DPtoLP(&point);
      dc.SetROP2(R2_NOTXORPEN);
	  this->ComputeMinandMaxPoint(point);
	  m_ChangeingPoint=point;
      this->SetMousePosText(point,Devpoint);

     
	  
 if(m_DLineProperty->m_DLineDlg2->m_Draw_Status==Select_Color_Status)
  {
    
    m_DLineProperty->m_DLineDlg2->OnChange(dc.GetPixel(point));
    m_CurrentDrawStatus=Select_Color_Status;
	m_CurrentDrawTool=0;
  }

if((m_LBDownTimes!=0)&&(m_CurrentDrawStatus==Draw_Status||m_CurrentDrawStatus==Change_Status))	
{     
    
	if(m_CurrentDrawStatus==Change_Status)
	{  
		m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_FoundPoint,m_SecondPoint);
	}
      
	m_pCurrentUnit->OnMouseMove(&dc,this,point);
	
 if(m_CurrentDrawStatus==Change_Status)
		  m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_FoundPoint,point);

}//*********if(m_IsDrawing&&(m_CurrentDrawStatus==Draw_Status||m_CurrentDrawStatus==Exchange_Status))  
 if(m_CurrentDrawStatus==Drag_Status)
 {     
	  m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_FoundPoint,m_SecondPoint);
	  dc.SetROP2(R2_NOTXORPEN);
	  
	  m_pCurrentUnit->m_DrawingMode=dc.GetROP2();
	 if(m_CurrentDrawTool!=EllipseArc_Tool)
	  m_pCurrentUnit->DrawActive(&dc,point);
	  
	  m_FirstPoint=m_SecondPoint;
      m_SecondPoint=point;

     m_pCurrentUnit->OnMouseMove(&dc,this,point);
     m_pCurrentUnit->m_DrawingMode=dc.GetROP2();
    
	 if(m_CurrentDrawTool!=EllipseArc_Tool)
	 m_pCurrentUnit->DrawActive(&dc,point);
	 
	m_pCurrentUnit->ShowMovingLine(&dc,m_pCurrentUnit->m_FoundPoint,point);

}
    CScrollView::OnMouseMove(nFlags, point);
}



void CEastDrawView::OnBUTTONDLine() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
    ResetSelectStatus();
   
    CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
    m_CurrentDrawTool=DLine_Tool;
	m_CurrentDrawStatus=Draw_Status;
	this->m_LBDownTimes=0;

}











void CEastDrawView::OnBUTTON32774Move() 
{   
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
   
	if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
    ResetSelectStatus();
    
    
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
	m_CurrentDrawTool=0;
	m_CurrentDrawStatus=Select_Status;
	m_LBDownTimes=0;
}

void CEastDrawView::OnBUTTON32775CircumRotate() 
{  
	
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
    //m_CurrentDrawStatus=Select_Status; 
	 if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
      ResetSelectStatus();
	
	 CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
     m_CurrentDrawTool=0;
	 m_CurrentDrawStatus=Circumrotate_Status;
}

void CEastDrawView::OnBUTTON32777Curve() 
{  
	CClientDC dc(this);
	OnPrepareDC(&dc);
    dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
    if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
    ResetSelectStatus();
	
	
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}

    m_CurrentDrawTool=Curve_Tool;
	m_LBDownTimes=0;
	m_CurrentDrawStatus=Draw_Status;
}

void CEastDrawView::OnBUTTON32776Rectangle() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);

	// TODO: Add your command handler code here
if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();
   
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}

    m_CurrentDrawTool=Rectangle_Tool;
    m_LBDownTimes=0;
	m_CurrentDrawStatus=Draw_Status;
}

void CEastDrawView::OnBUTTON32778RoundRectangle() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);

	// TODO: Add your command handler code here
if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
     ResetSelectStatus();
	
  CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}	
  m_CurrentDrawTool=RoundRectangle_Tool;
  m_CurrentDrawStatus=Draw_Status;
  m_LBDownTimes=0;
}

void CEastDrawView::OnBUTTON32779Ellipse() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
 if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();

   CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	} 
	m_CurrentDrawTool=Ellipse_Tool;
    m_CurrentDrawStatus=Draw_Status;
    m_LBDownTimes=0;

}

void CEastDrawView::OnBUTTON32780Round() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();
 
  CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
  m_CurrentDrawTool=Round_Tool;
  m_CurrentDrawStatus=Draw_Status;
  this->m_LBDownTimes=0;

}

void CEastDrawView::OnBUTTON32791Polygon() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
 if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();
 
 
 m_IsPolygonFirst=true;
 CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}

	m_CurrentDrawTool=Polygon_Tool;
    m_CurrentDrawStatus=Draw_Status;
	this->m_LBDownTimes=0;
}

void CEastDrawView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	  	
      CPoint Devpoint=point;
	  CClientDC dc(this);
	  this->OnPrepareDC(&dc);
      dc.SetWindowOrg(m_CenterPoint);
	  dc.DPtoLP(&point);
      dc.SetROP2(R2_NOTXORPEN);
	  this->ComputeMinandMaxPoint(point);
	  m_ChangeingPoint=point;
      this->SetMousePosText(point,Devpoint);

   if(m_CurrentDrawStatus==Draw_Status&&m_CurrentDrawTool==Polygon_Tool)
	{
	m_pCurrentUnit->DrawEnd(&dc,point);
	this->m_LBDownTimes=0;
	this->ExchangeSave();
	 if(m_IsMouseCap)
	::ReleaseCapture();
	}
 
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CEastDrawView::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
 
 
}



void CEastDrawView::OnMENUITEM32794SetRegin() 
{
	// TODO: Add your command handler code here
    CReginSet setReginDlg;
	CClientDC dc(this);
	setReginDlg.m_Length=this->GetTotalSize().cx;
   	setReginDlg.m_Width=this->GetTotalSize().cy;
    
	setReginDlg.m_X=m_CenterPoint.x;
	setReginDlg.m_Y=m_CenterPoint.y;
    setReginDlg.m_bIsViewReign=true;
   if(setReginDlg.DoModal()==IDOK)
	{
	CSize sizeTotal;
    sizeTotal.cx=setReginDlg.m_Length;
	sizeTotal.cy=setReginDlg.m_Width;
	SetScrollSizes(MM_TEXT, sizeTotal);
	OnPrepareDC(&dc);
	m_CenterPoint.x=setReginDlg.m_X;
	m_CenterPoint.y=setReginDlg.m_Y;
	dc.SetWindowOrg(m_CenterPoint);
	OnBUTTON32850ReDraw();
	
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_CenterPoint=m_CenterPoint;
	pDoc->m_TotalSize=sizeTotal;
   }
   
   

}

void CEastDrawView::OnBUTTONLiEllipse() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
 if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();

 
CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
	m_CurrentDrawTool=LiEllipse_Tool;
    m_CurrentDrawStatus=Draw_Status;
	this->m_LBDownTimes=0;
}

void CEastDrawView::ResetSelectStatus()
{
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CClientDC dc(this);
	CClientDC*pDC=&dc;
    OnPrepareDC(pDC);
	dc.SetWindowOrg(m_CenterPoint);

	for(int i=0;i<pDoc->m_PolygonArray.GetSize();i++)
	{
	  CPolygon*pPolygon=(CPolygon*)pDoc->m_PolygonArray.GetAt(i);
	  
	  if(pPolygon->m_HaveFindFirst)
	  {
		  pPolygon->ShowSelectPoint(pDC);
	      pPolygon->m_HaveFindFirst=false;
	  }
	}

	for(i=0;i<pDoc->m_TextArray.GetSize();i++)
	{
	  CText*pText=(CText*)pDoc->m_TextArray.GetAt(i);
	  if(pText->m_HaveFindFirst)
	  {	
		pText->ShowSelectPoint(pDC);
		
	    pText->m_HaveFindFirst=false;
	
	  }
	
	
	}
    for(i=0;i<pDoc->m_EllipseArcArray.GetSize();i++)
	{
	  CEllipseArc*pEllipseArc=(CEllipseArc*)pDoc->m_EllipseArcArray.GetAt(i);
	  if(pEllipseArc->m_HaveFindFirst)
	  {
	   pEllipseArc->ShowSelectPoint(pDC);
	   pEllipseArc->m_HaveFindFirst=false;
	  }
	}

	for(i=0;i<pDoc->m_DLineArray.GetSize();i++)
   {
	CDLine*pDLine=(CDLine*)pDoc->m_DLineArray.GetAt(i);
	
    if(pDLine->m_HaveFindFirst)
	{	pDLine->m_HaveFindFirst=false;
		pDLine->ShowSelectPoint(pDC);
	}
	}
	for(i=0;i<pDoc->m_CurveArray.GetSize();i++)
	{
	 CCurve*pCurve=(CCurve*)pDoc->m_CurveArray.GetAt(i);
	 
	 if(pCurve->m_HaveFindFirst)
	 {   pCurve->m_HaveFindFirst=false;
		 pCurve->ShowSelectPoint(pDC);
     }
	}

	for(i=0;i<pDoc->m_RectangleArray.GetSize();i++)
	{
	CRectangle*pRectangle=(CRectangle*)pDoc->m_RectangleArray.GetAt(i);
	
	if(pRectangle->m_HaveFindFirst)
	{
		pRectangle->m_HaveFindFirst=false;
		pRectangle->ShowSelectPoint(pDC);
	}
	}
    for(i=0;i<pDoc->m_RoundRectangleArray.GetSize();i++)
	{
	CRoundRectangle*pRoundRectangle=(CRoundRectangle*)pDoc->m_RoundRectangleArray.GetAt(i);
	
	if(pRoundRectangle->m_HaveFindFirst)
	{   pRoundRectangle->m_HaveFindFirst=false;
		pRoundRectangle->ShowSelectPoint(pDC);
	}
	}
    for(i=0;i<pDoc->m_EllipseArray.GetSize();i++)
	{
	CEllipse*pEllipse=(CEllipse*)pDoc->m_EllipseArray.GetAt(i);
	
	if(pEllipse->m_HaveFindFirst)
	{
		pEllipse->m_HaveFindFirst=false;
		pEllipse->ShowSelectPoint(pDC);
	}
	}
    for(i=0;i<pDoc->m_RoundArray.GetSize();i++)
	{
	CRound*pRound=(CRound*)pDoc->m_RoundArray.GetAt(i);
	
	if(pRound->m_HaveFindFirst)
	{   pRound->m_HaveFindFirst=false;
		pRound->ShowSelectPoint(pDC);
	}
	}
  for(i=0;i<pDoc->m_LiEllipseArray.GetSize();i++)
	{
	CEllipse*pLiEllipse=(CEllipse*)pDoc->m_LiEllipseArray.GetAt(i);
	
	if(pLiEllipse->m_HaveFindFirst)
	{
		pLiEllipse->m_HaveFindFirst=false;
		pLiEllipse->ShowSelectPoint(pDC);
	}
	}
  for(i=0;i<pDoc->m_RoundArcArray.GetSize();i++)
  {
    CRoundArc*pRoundArc=(CRoundArc*)pDoc->m_RoundArcArray.GetAt(i);
	if(pRoundArc->m_HaveFindFirst)
	{
	  pRoundArc->m_HaveFindFirst=false;
	  pRoundArc->ShowSelectPoint(pDC);
	}
  }
 for(i=0;i<pDoc->m_PolyBezierArray.GetSize();i++)
 {
  CPolyBezier*pPolyBezier=(CPolyBezier*)pDoc->m_PolyBezierArray.GetAt(i);
  if(pPolyBezier->m_HaveFindFirst)
  {
    pPolyBezier->m_HaveFindFirst=false;
	pPolyBezier->ShowSelectPoint(pDC);
  }
 }


}



void CEastDrawView::ComputeMinandMaxPoint(CPoint point)
{
m_MinPoint.x=min(m_MinPoint.x,point.x);
m_MinPoint.y=min(m_MinPoint.y,point.y);
m_MaxPoint.x=max(m_MaxPoint.x,point.x);
m_MaxPoint.y=max(m_MaxPoint.y,point.y);
m_InvalidRect.SetRect(m_MinPoint,m_MaxPoint);
}

void CEastDrawView::InvalidateMovingRect(CUnit *pMovingUnit)
{
/*pMovingUnit->GetBoundingRect(); 
m_InvalidRect.left=min(m_InvalidRect.left,pMovingUnit->m_BoundingRect.left);
m_InvalidRect.top=min(m_InvalidRect.top,pMovingUnit->m_BoundingRect.top);
m_InvalidRect.right=max(m_InvalidRect.right,pMovingUnit->m_BoundingRect.right);
m_InvalidRect.bottom=max(m_InvalidRect.bottom,pMovingUnit->m_BoundingRect.bottom);
m_InvalidRect.InflateRect(pMovingUnit->m_PenWidth,pMovingUnit->m_PenWidth);
//
CClientDC dc(this);
dc.Rectangle(m_InvalidRect);*/
//this->InvalidateRect(m_InvalidRect);
}

void CEastDrawView::OnBUTTON32800RoundArc() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here

if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();
 
 CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
	m_CurrentDrawTool=RoundArc_Tool;
    m_CurrentDrawStatus=Draw_Status;
	this->m_LBDownTimes=0;
}



void CEastDrawView::OnBUTTON32805EllipseArc() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();
 
CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
 m_CurrentDrawTool=EllipseArc_Tool;
 m_CurrentDrawStatus=Draw_Status;
 this->m_LBDownTimes=0;

}

void CEastDrawView::OnBUTTON32807PolyBezier() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();
 
 CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
m_CurrentDrawTool=PolyBezier_Tool;
 m_CurrentDrawStatus=Draw_Status;
 this->m_LBDownTimes=0;
}



void CEastDrawView::OnBUTTON32809Text() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
 ResetSelectStatus();
 
 CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(true);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
  if(!pMainFrame->m_wndStyleBar.IsWindowVisible())
  {
   pMainFrame->ShowFontBar(true);
   this->m_LBDownTimes=0;
  }
  //m_pTextEdit->ShowWindow(true);
  m_CurrentDrawTool=Text_Tool;
 m_CurrentDrawStatus=Draw_Status;
 this->m_LBDownTimes=0;

}







void CEastDrawView::GetFontInfo(LPTSTR pszFaceName, int &nSize)
{
   /// CHARFORMAT cf = GetCharFormatSelection ();
    //::lstrcpy (pszFaceName, cf.dwMask & CFM_FACE ? cf.szFaceName : _T (""));
   // nSize = cf.dwMask & CFM_SIZE ? cf.yHeight : -1;
}


void CEastDrawView::OnCharBold() 
{
	// TODO: Add your command handler code here
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_wndStyleBar.OnCharBold();
	 
}

void CEastDrawView::OnCharItalic() 
{
	// TODO: Add your command handler code here
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_wndStyleBar.OnCharItalic();
}

void CEastDrawView::OnCharUnderline() 
{
	// TODO: Add your command handler code here
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_wndStyleBar.OnCharUnderline();
}







void CEastDrawView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
      CClientDC dc(this);
	  this->OnPrepareDC(&dc);
	  dc.SetWindowOrg(m_CenterPoint);
	  //dc.DPtoLP(&point);
	// TODO: Add your message handler code here
      if(m_CurrentDrawStatus!=Select_Status)
	  m_pCurrentUnit->OnContextMenu(pWnd,point);
	  
     if(m_CurrentDrawStatus==Select_Status)
	 {
	  CMenu menuTextEdit;
      menuTextEdit.LoadMenu(IDR_MENU_Delete);
      
      menuTextEdit.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,pWnd);
	 
	 }

}

void CEastDrawView::OnMenuitemOk() 
{
	// TODO: Add your command handler code here

	  CClientDC dc(this);
	  OnPrepareDC(&dc);
	  dc.SetWindowOrg(m_CenterPoint);
	  m_pCurrentUnit->OnMenuitemOk(&dc,this);
}





BOOL CEastDrawView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_CurrentDrawStatus==Draw_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Color_Status)
	 SetCursor(AfxGetApp()->LoadCursor(ID_Draw));
    
	if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Select_Status)
     SetCursor(AfxGetApp()->LoadCursor(ID_Change));
	return true;
	//return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CEastDrawView::ShowFontBar(BOOL isShow)
{
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->ShowFontBar(isShow);
}

void CEastDrawView::OnCirCu() 
{
	// TODO: Add your command handler code here
      CClientDC dc(this);
	  this->OnPrepareDC(&dc);
	  dc.SetWindowOrg(m_CenterPoint);
	  this->m_pCurrentUnit->OnMenuitemCirCu(&dc,this);
}

void CEastDrawView::OnUpdateCirCu(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

  pCmdUI->SetCheck();
}

void CEastDrawView::SetMousePosText(CPoint Logpoint,CPoint Devpoint)
{
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->SetMousePosText(Logpoint,Devpoint);
}

void CEastDrawView::DrawAxis(CDC*pDC)
{
  
	CPen pen;
    pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	CPen*OldPen=pDC->SelectObject(&pen);
	int oldRop=pDC->SetROP2(R2_NOTXORPEN);
	pDC->MoveTo(m_CenterPoint.x+2,0);
	pDC->LineTo(GetTotalSize().cx+m_CenterPoint.x-2,0);
	
	pDC->MoveTo(0,m_CenterPoint.y+2);
	pDC->LineTo(0,GetTotalSize().cy+m_CenterPoint.y-2);
	
	pDC->SelectObject(OldPen);
	pDC->SetROP2(oldRop);

}






BOOL CEastDrawView::SaveDIB(CBitmap*pbitmap, CFile&file)
{
    CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	CDC*pViewDC=&dc;
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CBitmap*Oldmap;
    CBitmap Bitmap;
	CDC memDC;
    Bitmap.CreateCompatibleBitmap(pViewDC,this->GetTotalSize().cx,this->GetTotalSize().cy);
    memDC.CreateCompatibleDC(pViewDC);
    memDC.SetMapMode(pViewDC->GetMapMode());
	Oldmap=memDC.SelectObject(&Bitmap);
	
	memDC.SetWindowOrg(m_CenterPoint);
	memDC.PatBlt(m_CenterPoint.x,m_CenterPoint.y,GetTotalSize().cx,GetTotalSize().cy,WHITENESS);
	
	this->DrawAll(&memDC);
    if(this->m_HaveAxis)
		this->DrawAxis(&memDC);
	::GdiFlush();
	memDC.SelectObject(Oldmap);
	memDC.DeleteDC();
	
	BITMAPFILEHEADER bmfHdr; 
	
    bmfHdr.bfType=0x4d42;
	int sizeHdr=sizeof(BITMAPINFOHEADER);
   
    LPBITMAPINFOHEADER lpBI=( LPBITMAPINFOHEADER)::GlobalAlloc(LMEM_FIXED,sizeHdr);
 	if(!lpBI)
	{
	  AfxMessageBox("ƒ⁄¥Ê…Í«Î ß∞‹£¨«Î…‘∫Û‘Ÿ ‘");
	
	}
	BYTE *pDib;
	BYTE *pDib2;
    pDib=(BYTE*)::GlobalAlloc(LMEM_FIXED,GetTotalSize().cx*GetTotalSize().cy*4);
    if(!pDib)
	{
	  AfxMessageBox("ƒ⁄¥Ê…Í«Î ß∞‹£¨«Î…‘∫Û‘Ÿ ‘");
	
	}
	lpBI->biSize=(LONG)sizeof(BITMAPINFOHEADER);
	lpBI->biWidth=(LONG)this->GetTotalSize().cx;
	lpBI->biHeight=(LONG)this->GetTotalSize().cy;
	lpBI->biPlanes=1;
	lpBI->biBitCount=32;
	lpBI->biCompression=BI_RGB;
	lpBI->biSizeImage=0;
	lpBI->biXPelsPerMeter=0;
	lpBI->biYPelsPerMeter=0;
	lpBI->biClrUsed=0;
	lpBI->biClrImportant=0;

   
    Bitmap.GetBitmapBits(GetTotalSize().cx*GetTotalSize().cy*4,pDib);

	pDib2=(BYTE*)::GlobalAlloc(LMEM_FIXED,GetTotalSize().cx*GetTotalSize().cy*4);
	  if(!pDib2)
	{
	  AfxMessageBox("ƒ⁄¥Ê…Í«Î ß∞‹£¨«Î…‘∫Û‘Ÿ ‘");
	
	}
	
	for(int j=0;j<GetTotalSize().cy;j++)
		 for(int i=0;i<GetTotalSize().cx*4;i++)
	 {
	  pDib2[i+j*GetTotalSize().cx*4]=pDib[GetTotalSize().cx*(GetTotalSize().cy-1-j)*4+i];	
	}
		 ::GlobalFree(pDib);
	

    bmfHdr.bfSize=(DWORD) (sizeof(BITMAPFILEHEADER) + 
                 sizeHdr +GetTotalSize().cx*GetTotalSize().cy*4); 
	bmfHdr.bfReserved1=bmfHdr.bfReserved2=0;
	bmfHdr.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeHdr;
	
  try
  {
	 
     file.Write((LPVOID)&bmfHdr,sizeof(BITMAPFILEHEADER));
	 file.Write((LPVOID)lpBI,sizeHdr);
	 file.WriteHuge((LPVOID)pDib2,GetTotalSize().cx*GetTotalSize().cy*4);
  }

  catch(CException*Fe)
  {
    Fe->ReportError();
	Fe->Delete();
   return false;
  }
  
 
  ::GlobalFree(lpBI);
  ::GlobalFree(pDib2);
   return true;  
  

}





void CEastDrawView::DeleteSelect()
{   
	this->ExchangeSave();
	CClientDC dc(this);
	this->OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	CDC*pDC=&dc;
	CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int oldrop=pDC->SetROP2(R2_NOTXORPEN);   
	for(int i=0;i<pDoc->m_PolygonArray.GetSize();i++)
	{
	  CPolygon*pPolygon=(CPolygon*)pDoc->m_PolygonArray.GetAt(i);
	  if(pPolygon->m_HaveFindFirst)
	  {
		pPolygon->ShowSelectPoint(pDC);
	    pPolygon->DrawStatic(pDC);
		pDoc->m_PolygonArray.RemoveAt(i);
		delete 	pPolygon;
		i--;

	  }
                
	}
    for(i=0;i<pDoc->m_TextArray.GetSize();i++)
	{
	  CText*pText=(CText*)pDoc->m_TextArray.GetAt(i);
	  
	    if(pText->m_HaveFindFirst)
		{	
			 pText->ShowSelectPoint(pDC);
	         //pText->DrawStatic(pDC);
			 CPoint point=pText->m_PositionPoint;
			 pDC->LPtoDP(&point);

			 this->InvalidateRect(CRect(point,CPoint(point.x+pText->m_Width,point.y+pText->m_Hight)));
			 pDoc->m_TextArray.RemoveAt(i);
			 delete pText;
             i--;
		}
	}

	for(i=0;i<pDoc->m_PolyBezierArray.GetSize();i++)
	{
	 CPolyBezier*pPolyBezier=(CPolyBezier*)pDoc->m_PolyBezierArray.GetAt(i);
	 
	  if(pPolyBezier->m_HaveFindFirst)
	  {
		  pPolyBezier->ShowSelectPoint(pDC);
	      pPolyBezier->DrawStatic(pDC);
		  pDoc->m_PolyBezierArray.RemoveAt(i);
		  delete pPolyBezier;
		  i--;
	  }
	}
	for(i=0;i<pDoc->m_DLineArray.GetSize();i++)
   {
	CDLine*pDLine=(CDLine*)pDoc->m_DLineArray.GetAt(i);

    if(pDLine->m_HaveFindFirst)
	{   pDLine->ShowSelectPoint(pDC);
	    pDLine->DrawStatic(pDC);
	     pDoc->m_DLineArray.RemoveAt(i);
		 delete pDLine;
	     i--;
	}
	
	}
	for(i=0;i<pDoc->m_CurveArray.GetSize();i++)
	{
	 CCurve*pCurve=(CCurve*)pDoc->m_CurveArray.GetAt(i);
	 
	 if(pCurve->m_HaveFindFirst)
	 {
		 pCurve->ShowSelectPoint(pDC);
	     pCurve->DrawStatic(pDC);
		 pDoc->m_CurveArray.RemoveAt(i);
		 delete pCurve;
	     i--;
	 }
	 }

	for(i=0;i<pDoc->m_RectangleArray.GetSize();i++)
	{
	CRectangle*pRectangle=(CRectangle*)pDoc->m_RectangleArray.GetAt(i);
	
	if(pRectangle->m_HaveFindFirst)
	{
		pRectangle->ShowSelectPoint(pDC);
        pRectangle->DrawStatic(pDC);
		pDoc->m_RectangleArray.RemoveAt(i);
		delete pRectangle;
		i--;
	}
	}
    for(i=0;i<pDoc->m_RoundRectangleArray.GetSize();i++)
	{
	CRoundRectangle*pRoundRectangle=(CRoundRectangle*)pDoc->m_RoundRectangleArray.GetAt(i);
	
	if(pRoundRectangle->m_HaveFindFirst)
	{	
		pRoundRectangle->ShowSelectPoint(pDC);
	   pRoundRectangle->DrawStatic(pDC);
	    pDoc->m_RoundRectangleArray.RemoveAt(i);
		delete pRoundRectangle;
		i--;
	}
	}
    for(i=0;i<pDoc->m_EllipseArray.GetSize();i++)
	{
	CEllipse*pEllipse=(CEllipse*)pDoc->m_EllipseArray.GetAt(i);
	
	if(pEllipse->m_HaveFindFirst)
	{   pEllipse->ShowSelectPoint(pDC);
	    pEllipse->DrawStatic(pDC);
	    pDoc->m_EllipseArray.RemoveAt(i);
		delete pEllipse;
		i--;
	
	}
	}
    for(i=0;i<pDoc->m_RoundArray.GetSize();i++)
	{
	CRound*pRound=(CRound*)pDoc->m_RoundArray.GetAt(i);
	
	if(pRound->m_HaveFindFirst)
	{
		pRound->ShowSelectPoint(pDC);
	    pRound->DrawStatic(pDC);
	    pDoc->m_RoundArray.RemoveAt(i);
		delete pRound;
		i--;
	
	}
	}
  for(i=0;i<pDoc->m_LiEllipseArray.GetSize();i++)
	{
	CEllipse*pLiEllipse=(CEllipse*)pDoc->m_LiEllipseArray.GetAt(i);

	if(pLiEllipse->m_HaveFindFirst)
	{
		pLiEllipse->ShowSelectPoint(pDC);
	    pLiEllipse->DrawStatic(pDC);
	    pDoc->m_LiEllipseArray.RemoveAt(i);
		delete pLiEllipse;
		i--;
	
	}
	}
  for(i=0;i<pDoc->m_RoundArcArray.GetSize();i++)
  {
    CRoundArc*pRoundArc=(CRoundArc*)pDoc->m_RoundArcArray.GetAt(i);
	
	if(pRoundArc->m_HaveFindFirst)
	{
		pRoundArc->ShowSelectPoint(pDC);
	    pRoundArc->DrawStatic(pDC);
	    pDoc->m_RoundArcArray.RemoveAt(i);
		delete pRoundArc;
		i--;
	}
	}
    
  for(i=0;i<pDoc->m_EllipseArcArray.GetSize();i++)
  {
   CEllipseArc*pEllipseArc=(CEllipseArc*)pDoc->m_EllipseArcArray.GetAt(i);
   
   if(pEllipseArc->m_HaveFindFirst)
   {   pEllipseArc->ShowSelectPoint(pDC);
       pEllipseArc->DrawStatic(pDC);
       pDoc->m_EllipseArcArray.RemoveAt(i);
	   delete pEllipseArc;
	   i--;
   }
   }
  pDC->SetROP2(oldrop);
}

void CEastDrawView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_DELETE)
		this->DeleteSelect();
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEastDrawView::OnDelete() 
{
	// TODO: Add your command handler code here
	this->DeleteSelect();
}

void CEastDrawView::OnBUTTON32830Lock() 
{
	// TODO: Add your command handler code here
	m_IsLocked=!m_IsLocked;
}

void CEastDrawView::OnUpdateBUTTON32830Lock(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_IsLocked?1:0);
}

void CEastDrawView::OnUpdateBUTTONDLine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentDrawTool==DLine_Tool?1:0);

}

void CEastDrawView::OnUpdateBUTTON32776Rectangle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
  pCmdUI->SetCheck(m_CurrentDrawTool==Rectangle_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32778RoundRectangle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 pCmdUI->SetCheck(m_CurrentDrawTool==RoundRectangle_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32791Polygon(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 pCmdUI->SetCheck(m_CurrentDrawTool==Polygon_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32779Ellipse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 pCmdUI->SetCheck(m_CurrentDrawTool==Ellipse_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32780Round(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
pCmdUI->SetCheck(m_CurrentDrawTool==Round_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32777Curve(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 pCmdUI->SetCheck(m_CurrentDrawTool==Curve_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTONLiEllipse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 pCmdUI->SetCheck(m_CurrentDrawTool==LiEllipse_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32800RoundArc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
pCmdUI->SetCheck(m_CurrentDrawTool==RoundArc_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32805EllipseArc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
  pCmdUI->SetCheck(m_CurrentDrawTool==EllipseArc_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32807PolyBezier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
  pCmdUI->SetCheck(m_CurrentDrawTool==PolyBezier_Tool?1:0);	
}

void CEastDrawView::OnUpdateBUTTON32809Text(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
 pCmdUI->SetCheck(m_CurrentDrawTool==Text_Tool?1:0);
}

void CEastDrawView::OnUpdateBUTTON32774Move(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
pCmdUI->SetCheck(m_CurrentDrawStatus!=Draw_Status&&m_CurrentDrawStatus!=0?1:0);	
}

void CEastDrawView::OnBUTTON32845Axis() 
{
	// TODO: Add your command handler code here
     CClientDC dc(this);
	 OnPrepareDC(&dc);
	 dc.SetWindowOrg(m_CenterPoint);
	 DrawAxis(&dc);
	 m_HaveAxis=!m_HaveAxis;
}

void CEastDrawView::OnUpdateBUTTON32845Axis(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_HaveAxis?1:0);
	
}

void CEastDrawView::OnBUTTON32850ReDraw() 
{
	// TODO: Add your command handler code here

    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CClientDC dc(this);
	this->OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
    
	CBitmap*Oldmap;
    CBitmap Bitmap;
	CDC memDC;
    Bitmap.CreateCompatibleBitmap(&dc,this->GetTotalSize().cx,this->GetTotalSize().cy);
    memDC.CreateCompatibleDC(&dc);
    Oldmap=memDC.SelectObject(&Bitmap);
	memDC.SetWindowOrg(m_CenterPoint);
	CRect client;
    CBrush br(RGB(255,0,255));
	GetClientRect(client);
	dc.DPtoLP(&client);
    memDC.SetMapMode(dc.GetMapMode());
	memDC.PatBlt(client.left,client.top,client.Width(),client.Height(),WHITENESS);
	this->DrawAll(&memDC);

	if(this->m_HaveAxis)
		this->DrawAxis(&memDC);
	dc.BitBlt(client.left,client.top,client.Width(),client.Height(),&memDC,client.left,client.top,SRCCOPY);
    memDC.DeleteDC();
   }

void CEastDrawView::DrawAll(CDC *pDrawDC)
{
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    CDC*pDC=pDrawDC;
    
	
	


	for(int i=0;i<pDoc->m_PolygonArray.GetSize();i++)
	{
	  CPolygon*pPolygon=(CPolygon*)pDoc->m_PolygonArray.GetAt(i);
	  pPolygon->DrawStatic(pDC);
	  if(pPolygon->m_HaveFindFirst)
		 pPolygon->ShowSelectPoint(pDC);
	}
    

	for(i=0;i<pDoc->m_PolyBezierArray.GetSize();i++)
	{
	 CPolyBezier*pPolyBezier=(CPolyBezier*)pDoc->m_PolyBezierArray.GetAt(i);
	 pPolyBezier->DrawStatic(pDC);
	  if(pPolyBezier->m_HaveFindFirst)
		  pPolyBezier->ShowSelectPoint(pDC);
	}
	for(i=0;i<pDoc->m_DLineArray.GetSize();i++)
   {
	CDLine*pDLine=(CDLine*)pDoc->m_DLineArray.GetAt(i);
	pDLine->DrawStatic(pDC);
    if(pDLine->m_HaveFindFirst)
		pDLine->ShowSelectPoint(pDC);
   }
	for(i=0;i<pDoc->m_CurveArray.GetSize();i++)
	{
	 CCurve*pCurve=(CCurve*)pDoc->m_CurveArray.GetAt(i);
	 pCurve->DrawStatic(pDC);
	 if(pCurve->m_HaveFindFirst)
		 pCurve->ShowSelectPoint(pDC);
	}

	for(i=0;i<pDoc->m_RectangleArray.GetSize();i++)
	{
	CRectangle*pRectangle=(CRectangle*)pDoc->m_RectangleArray.GetAt(i);
	pRectangle->DrawStatic(pDC);
	if(pRectangle->m_HaveFindFirst)
		pRectangle->ShowSelectPoint(pDC);
	}
    for(i=0;i<pDoc->m_RoundRectangleArray.GetSize();i++)
	{
	CRoundRectangle*pRoundRectangle=(CRoundRectangle*)pDoc->m_RoundRectangleArray.GetAt(i);
	pRoundRectangle->DrawStatic(pDC);
	if(pRoundRectangle->m_HaveFindFirst)
		pRoundRectangle->ShowSelectPoint(pDC);
	}
    for(i=0;i<pDoc->m_EllipseArray.GetSize();i++)
	{
	CEllipse*pEllipse=(CEllipse*)pDoc->m_EllipseArray.GetAt(i);
	pEllipse->DrawStatic(pDC);
	if(pEllipse->m_HaveFindFirst)
		pEllipse->ShowSelectPoint(pDC);
	}
    for(i=0;i<pDoc->m_RoundArray.GetSize();i++)
	{
	CRound*pRound=(CRound*)pDoc->m_RoundArray.GetAt(i);
	pRound->DrawStatic(pDC);
	if(pRound->m_HaveFindFirst)
		pRound->ShowSelectPoint(pDC);
	}
  for(i=0;i<pDoc->m_LiEllipseArray.GetSize();i++)
	{
	CEllipse*pLiEllipse=(CEllipse*)pDoc->m_LiEllipseArray.GetAt(i);
	pLiEllipse->DrawStatic(pDC);
	if(pLiEllipse->m_HaveFindFirst)
		pLiEllipse->ShowSelectPoint(pDC);
  }
  for(i=0;i<pDoc->m_RoundArcArray.GetSize();i++)
  {
    CRoundArc*pRoundArc=(CRoundArc*)pDoc->m_RoundArcArray.GetAt(i);
	pRoundArc->DrawStatic(pDC);
	if(pRoundArc->m_HaveFindFirst)
	pRoundArc->ShowSelectPoint(pDC);
  }
    
  for(i=0;i<pDoc->m_EllipseArcArray.GetSize();i++)
  {
   CEllipseArc*pEllipseArc=(CEllipseArc*)pDoc->m_EllipseArcArray.GetAt(i);
   pEllipseArc->DrawStatic(pDC);
   if(pEllipseArc->m_HaveFindFirst)
	   pEllipseArc->ShowSelectPoint(pDC);
  }
  for(i=0;i<pDoc->m_TextArray.GetSize();i++)
  {
   CText*pText=(CText*)pDoc->m_TextArray.GetAt(i);
   pText->DrawStatic(pDC);
    if(pText->m_HaveFindFirst)
	   pText->ShowSelectPoint(pDC);

  }
  
 // TODO: add draw code for native data here
}



void CEastDrawView::OnBUTTON32856DONothing() 
{
	// TODO: Add your command handler code here
    CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(m_CenterPoint);
	// TODO: Add your command handler code here
   
	if(m_CurrentDrawStatus==Drag_Status||m_CurrentDrawStatus==Change_Status||m_CurrentDrawStatus==Select_Status)
    ResetSelectStatus();
    
    
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndStyleBar.IsWindowVisible())
    { 
	 pMainFrame->ShowFontBar(false);
     if(this->m_LBDownTimes==1)
	 {
	  this->m_LBDownTimes=2;
	  this->m_pCurrentUnit->OnLButtonDown(&dc,this,0);
	  
	 }
	}
	m_CurrentDrawTool=0;
	m_CurrentDrawStatus=0;
	m_LBDownTimes=0;	
}







void CEastDrawView::AddTempFile()
{ 

	CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->AddTempFile();
}

void CEastDrawView::AutoSave()
{
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->AutoSave();
}

void CEastDrawView::ExchangeSave()
{
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->SetModifiedFlag();
   if(m_IsLastMove)
	{   
	    pDoc->Requeue(m_Index,m_Rtimes);
		m_IsLastMove=false;
	    m_Index=0;	
	
   }
   else
   {
	  m_Index=0;
      if(m_Rtimes<6)
        m_Rtimes++;
   }
   pDoc->ExchangeSave(m_Index,m_Rtimes);
   
  
   
}

void CEastDrawView::OnBUTTON32858OnDo() 
{
// TODO: Add your command handler code here
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
   if(m_Index<5)
   {
	 	 m_Index++;
		 
        this->m_IsLastMove=true;
		pDoc->OnBUTTON32858OnDo(m_Index);
        OnBUTTON32850ReDraw();
   }     
}

void CEastDrawView::OnBUTTON32859ReDo() 
{
	// TODO: Add your command handler code here
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
         if(m_Index>0)
		 {  
	     m_Index--;
	     pDoc->OnBUTTON32858OnDo(m_Index);
         m_IsLastMove=true;
	     OnBUTTON32850ReDraw();
		 }
}

void CEastDrawView::OnUpdateBUTTON32858OnDo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_Index<5&&m_Index<m_Rtimes&&m_Rtimes!=0?true:false);	
}

void CEastDrawView::OnUpdateBUTTON32859ReDo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_Index>0&&m_Rtimes!=0?true:false);
}

void CEastDrawView::ReMoveTemp()
{
    CEastDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->ReMoveTemp();
}



void CEastDrawView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CClientDC dc(this);
    int nHorzSC = dc.GetDeviceCaps(LOGPIXELSX);
    int nHorzPR = pDC->GetDeviceCaps(LOGPIXELSX);
	
	int PhyToDis=nHorzPR/nHorzSC;
	
	pDC->SetMapMode(MM_ANISOTROPIC);
    
	pDC->SetWindowExt(GetTotalSize());
	pDC->SetViewportExt(GetTotalSize().cx*PhyToDis,GetTotalSize().cy*PhyToDis);
    pDC->SetWindowOrg(m_CenterPoint.x,m_CenterPoint.y);
	this->DrawAll(pDC);
	if(this->m_HaveAxis)
		this->DrawAxis(pDC);

}

BOOL CEastDrawView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box

	return DoPreparePrinting(pInfo);
}

void CEastDrawView::SetBoxFont()
{
	CMainFrame*pMainFrame=STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->m_wndStyleBar.IniFont();
}

void CEastDrawView::OnMouseCapOn() 
{
	// TODO: Add your command handler code here
	
	m_IsMouseCap=!m_IsMouseCap;
}

void CEastDrawView::OnUpdateMouseCapOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_IsMouseCap?1:0);
}



void CEastDrawView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CSize size=this->GetTotalSize();
	CRect rect;
	this->GetClientRect(&rect);
	switch(nChar)
	{
	case VK_UP:
		if(rect.bottom<size.cy)
		ScrollToPosition(CPoint(GetDeviceScrollPosition().x,GetDeviceScrollPosition().y-rect.Height()/2));	
	    break;
	case VK_DOWN:
		if(rect.bottom<size.cy)
		ScrollToPosition(CPoint(GetDeviceScrollPosition().x,GetDeviceScrollPosition().y+rect.Height()/2));	
	    break;

	case VK_LEFT:
      ScrollToPosition(CPoint(GetDeviceScrollPosition().x-rect.Width()/2,GetDeviceScrollPosition().y));	
	    break;
    case VK_RIGHT:
      ScrollToPosition(CPoint(GetDeviceScrollPosition().x+rect.Width()/2,GetDeviceScrollPosition().y));	
	    break;
    case VK_HOME:
		ScrollToPosition(CPoint(0,0));	
	    break;
		
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}
