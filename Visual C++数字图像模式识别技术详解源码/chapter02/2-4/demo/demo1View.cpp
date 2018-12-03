// demo1View.cpp : implementation of the CDemo1View class
//

#include "stdafx.h"
#include "demo1.h"

#include "demo1Doc.h"
#include "demo1View.h"
#include "MainFrm.h"

#include "GeometryTrans.h"//几何变换类
#include "GeometryMoveDlg.h"//平移参数设置
#include "ZoomDlg.h"   //缩放参数设置
#include "RotateDlg.h"//旋转角度设置
#include "GrayTrans.h" //灰度变换类
#include "HistogramDrawDlg.h"//直方图显示
#include "ThreshStrechDlg.h"//阈值设置
#include "StrechWindowDlg.h"//窗口变换参数设置
#include "LinearStepParamDlg.h"//分段线性拉伸参数设置


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "ImageDib.h"
/////////////////////////////////////////////////////////////////////////////
// CDemo1View

IMPLEMENT_DYNCREATE(CDemo1View, CView)

BEGIN_MESSAGE_MAP(CDemo1View, CView)
	//{{AFX_MSG_MAP(CDemo1View)
	ON_COMMAND(id_Move, OnMove)
	ON_COMMAND(id_HorizontalMirror, OnHorizontalMirror)
	ON_COMMAND(id_VerticalMirror, OnVerticalMirror)
	ON_COMMAND(id_Zoom, OnZoom)
	ON_COMMAND(id_Clockwise90, OnClockwise90)
	ON_COMMAND(id_Anticlockwise90, OnAnticlockwise90)
	ON_COMMAND(id_Rotate180, OnRotate180)
	ON_COMMAND(id_FreeRotate, OnFreeRotate)
	ON_COMMAND(id_Binary, OnBinary)
	ON_COMMAND(id_HistogramDraw, OnHistogramDraw)
	ON_COMMAND(id_HistgramAver, OnHistgramAver)
	ON_COMMAND(id_ImageReverse, OnImageReverse)
	ON_COMMAND(id_ImgThresh, OnImgThresh)
	ON_COMMAND(id_ThresholdWindow, OnThresholdWindow)
	ON_COMMAND(id_LinearStrecth, OnLinearStrecth)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo1View construction/destruction

CDemo1View::CDemo1View()
{
	// TODO: add construction code here

}

CDemo1View::~CDemo1View()
{
}

BOOL CDemo1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDemo1View drawing
/**/
void CDemo1View::OnDraw(CDC* pDC)
{
	CDemo1Doc* pDoc = GetDocument();			//获取文档类指针
	ASSERT(pDoc != NULL);
	ImageDib* pDib = pDoc->m_dib;					//返回m_dib的指针
	
	pDib->Draw(pDC, CPoint(0, 0), pDib->GetDimensions()); 	//显示DIB
}


/////////////////////////////////////////////////////////////////////////////
// CDemo1View printing

BOOL CDemo1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDemo1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDemo1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDemo1View diagnostics

#ifdef _DEBUG
void CDemo1View::AssertValid() const
{
	CView::AssertValid();
}

void CDemo1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo1Doc* CDemo1View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo1Doc)));
	return (CDemo1Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemo1View message handlers

void CDemo1View::OnMove() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc = GetDocument();
	ImageDib *pDib = pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8 && pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//平移参数设置对话框
	GeometryMoveDlg dlgPara;
	if(dlgPara.DoModal()==IDOK)
	{
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//将dlgPara.m_Xmove, dlgPara.m_Ymove作为平移量，对图像平移
		geoTrans.Move(dlgPara.m_Xmove, dlgPara.m_Ymove);

		
/*	*/	//新建视图窗口，多文档显示变换结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();		
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);

//      单文档显示变换结果
/*
		CClientDC dc(this);
		ImageDib *dibNew = new ImageDib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		dibNew->Draw(&dc,CPoint(0,0),dibNew->GetDimensions());
		pDib->Draw(&dc,CPoint(dibNew->GetDimensions().cx,0),pDib->GetDimensions());
 */
/*
		pDib->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		Invalidate();
*/
	}	
}

void CDemo1View::OnHorizontalMirror() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//水平镜像
	geoTrans.MirrorHorTrans();
   //显示变换结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();		
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);

}

void CDemo1View::OnVerticalMirror() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//垂直镜像
	geoTrans.MirrorVerTrans();
   //显示变换结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();		
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
}

void CDemo1View::OnZoom() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//原图像尺寸
	CSize oldImgSize=pDib->GetDimensions();
	
	//缩放参数设置对话框
	ZoomDlg dlgPara;
	
	if(dlgPara.DoModal()==IDOK){
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//根据指定的缩放比例对对原图像进行缩放缩放
		geoTrans.Zoom(dlgPara.m_XstrechRatio,dlgPara.m_YstrechRatio);	
		
		//新建视图窗口，显示处理结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
			geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
		Invalidate();
	}
}

void CDemo1View::OnClockwise90() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//顺时针90度
	geoTrans.Clockwise90();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnAnticlockwise90() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//逆时针90度
	geoTrans.Anticlockwise90();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnRotate180() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//旋转180度
	geoTrans.Rotate180();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
		geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnFreeRotate() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	//旋转参数设置对话框
	RotateDlg dlgPara;
	if(dlgPara.DoModal()==IDOK){
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//调用Rotate函数进行图像旋转，将角度和插值方式参数传进函数
		geoTrans.Rotate(dlgPara.m_RotateAngle);
		
		//新建视图窗口，显示处理结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
			geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
		Invalidate();
	}
}


void CDemo1View::OnBinary() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.BinaryImage();
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnHistogramDraw() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//直方图绘制对话框
	HistogramDrawDlg dlgHist;
	dlgHist.m_himageDib.ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//显示直方图
   dlgHist.DoModal();
}


void CDemo1View::OnHistgramAver() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	HistogramDrawDlg histodrawdlg1; 
	histodrawdlg1.m_himageDib.ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//直方图均衡
	histodrawdlg1.histogramAve();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(histodrawdlg1.GetDimensions(),histodrawdlg1.m_nBitCountOut,histodrawdlg1.m_lpColorTableOut, histodrawdlg1.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnImageReverse() 
{
	// TODO: Add your command handler code here
	//获取文档类指针
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//DIB类型判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(),
		pDib->m_nBitCount,pDib->m_lpColorTable,pDib->m_pImgData);
	
	//调用ReverseImg()对图像求反
	graytrans.RevImage();
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnImgThresh() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//阈值分割对话框类对象threshsetdlg
	ThreshStrechDlg threshsetdlg;
	if(threshsetdlg.DoModal()==IDOK)
	{
		//定义灰度变换类的对象graytrans，并将当前DIB数据作为输入数据传入graytrans中
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//根据用户选择的阈值，调用二值化函数分割图像
		if ((threshsetdlg.m_Thresh>=0)&&(threshsetdlg.m_Thresh<=255)) 	
			graytrans.BinaryImage(threshsetdlg.m_Thresh);
		
        else
		{
			AfxMessageBox("请输入一个0-255之间的阈值！");
		    return;
		}
			//新建视图，显示分割结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
		Invalidate();
	}
}

void CDemo1View::OnThresholdWindow() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//阈值分割对话框类对象threshsetdlg
	StrechWindowDlg  strechwinsetdlg;
	if(strechwinsetdlg.DoModal()==IDOK)
	{
		//定义灰度变换类的对象graytrans，并将当前DIB数据作为输入数据传入graytrans中
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		graytrans.ThresholdWindow(strechwinsetdlg.m_nBottom,strechwinsetdlg.m_nTop);
	  
	   	//新建视图，显示分割结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
		Invalidate();
	}
}

void CDemo1View::OnLinearStrecth() 
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=24&&pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理彩色和灰度图像",MB_OK,0);
		return ;
	}
	
	//线性拉伸参数设置对话框
	LinearStepParamDlg linesdlgPara;
	if(linesdlgPara.DoModal()==IDOK){
		//获取用户设置的两个拉伸转折点
		CPoint point1,point2;
		point1.x=linesdlgPara.m_nX1;
		point1.y=linesdlgPara.m_nY1;
		point2.x =linesdlgPara.m_nX2;
		point2.y=linesdlgPara.m_nY2;
		
		//判断参数的合理性
		if(point1.x<0||point1.x>255||point2.x<0||point2.x>255
			||point1.y<0||point1.y>255||point2.y<0||point2.y>255
			||point1.x>point2.x){
			::MessageBox(0,"参数不合理",MB_OK,0);
			return;
		}
		
		//定义灰度变换的类对象
		GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		
		//线性拉伸
		graytrans.LinearStrech(point1,point2);
		
		//新建窗口显示
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);		
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->m_dib;	
		dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
		pView->OnInitialUpdate();	
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
		Invalidate();
	}
}
