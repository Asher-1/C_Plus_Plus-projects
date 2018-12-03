// demo1View.cpp : implementation of the CDemo1View class
//

#include "stdafx.h"
#include "demo1.h"
#include "math.h"

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
#include "ImageEnhance.h"//图像空域增强类
#include "NeiAverSmoothDlg.h"//图像邻域平均模版设置
#include "MedianSmoothDlg.h"//图像中值平均模板设置
#include "FourierTrans.h"//傅立叶变换类
#include "ImageFreqEnhance.h"//图像频域增强类
#include "WaveletTrans.h"//小波变换类
#include "Morphology.h"//图像形态学类
#include "ImgSegment.h"//图像分割类
#include "HistSegmentDlg.h"//图像直方图分割对话框
#include "PlateTrans.h"

//全局变量
FourierTrans fftTrans;
WaveletTrans wavltTrans;

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
	ON_COMMAND(id_PepperSaltNoise, OnPepperSaltNoise)
	ON_COMMAND(id_GaussNoise, OnGaussNoise)
	ON_COMMAND(id_ChooseMaskSmooth, OnChooseMaskSmooth)
	ON_COMMAND(id_GradeSharp, OnGradeSharp)
	ON_COMMAND(id_NeiAveSmooth, OnNeiAveSmooth)
	ON_COMMAND(id_MedianSmooth, OnMedianSmooth)
	ON_COMMAND(id_LaplaceSharp, OnLaplaceSharp)
	ON_COMMAND(id_QuickFFt, OnQuickFFt)
	ON_COMMAND(id_QuickFFt_Reverse, OnQuickFFtReverse)
	ON_COMMAND(id_IdealLowPass, OnIdealLowPass)
	ON_COMMAND(id_ButterLowPass, OnButterLowPass)
	ON_COMMAND(id_IdealHighPass, OnIdealHighPass)
	ON_COMMAND(id_ButterHighPass, OnButterHighPass)
	ON_COMMAND(id_HarrWaveletTrans, OnHarrWaveletTrans)
	ON_COMMAND(id_HarrWavRevTrans, OnHarrWavRevTrans)
	ON_COMMAND(id_Erosion, OnErosion)
	ON_COMMAND(id_Dilation, OnDilation)
	ON_COMMAND(id_Open, OnOpen)
	ON_COMMAND(id_Close, OnClose)
	ON_COMMAND(id_Thinning, OnThinning)
	ON_COMMAND(id_Robert, OnRobert)
	ON_COMMAND(id_Sobel, OnSobel)
	ON_COMMAND(id_Prewitt, OnPrewitt)
	ON_COMMAND(id_Laplacian, OnLaplacian)
	ON_COMMAND(id_RegionGrow, OnRegionGrow)
	ON_COMMAND(id_EdgeTrace, OnEdgeTrace)
	ON_COMMAND(id_AdaptiveThreshold, OnAdaptThresholdSeg)
	ON_COMMAND(id_HistThreshSeg, OnHistThreshSeg)
	ON_COMMAND(id_ColorPair, OnColorPair)
	ON_COMMAND(id_ColorToGray, OnColorToGray)
	ON_COMMAND(id_platelocate, Onplatelocate)
	ON_COMMAND(id_platerelocate, Onplaterelocate)
	ON_COMMAND(id_charseparate, Oncharseparate)
	ON_COMMAND(id_charrecognition, Oncharrecognition)
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
		geoTrans.Rotate(dlgPara.m_rotateAngle, dlgPara.m_interpolationSelect);
		
		//新建视图窗口，显示处理结果
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
		CDemo1Doc* pDocNew=pView->GetDocument();
		ImageDib *dibNew=pDocNew->GetPDib();
		dibNew->ReplaceDib(geoTrans.GetDimensions(),geoTrans.m_nBitCountOut,
			geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		pView->OnInitialUpdate();
		pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);
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
	graytrans.BinaryImage(120);
	
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
	}
}

void CDemo1View::OnPepperSaltNoise() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);
	imgnoise.AddPepperSaltNoise();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}


void CDemo1View::OnGaussNoise() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgnoise(pDib->GetDimensions(),pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);
	imgnoise.AddGaussNoise();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	
	dibNew->ReplaceDib(imgnoise.GetDimensions(),imgnoise.m_nBitCountOut,imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnChooseMaskSmooth() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgSmooth(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	imgSmooth.ChooseMaskSmooth();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgSmooth.GetDimensions(),imgSmooth.m_nBitCountOut,imgSmooth.m_lpColorTable, imgSmooth.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);

}


void CDemo1View::OnGradeSharp() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgsharp(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	int nThresh=30;
	imgsharp.GradeSharp(nThresh);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	
	dibNew->ReplaceDib(imgsharp.GetDimensions(),imgsharp.m_nBitCountOut,imgsharp.m_lpColorTable, imgsharp.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
}

void CDemo1View::OnNeiAveSmooth() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	ImageEnhance imgsmooth(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	NeiAverSmoothDlg dlgparam;
	
	if (dlgparam.DoModal()==IDOK) {
        
		if (dlgparam.m_radiovalue==0||dlgparam.m_radiovalue==1||dlgparam.m_radiovalue==2) {
	
        float Temp1[81];
		int i;
		for (i = 0; i < 81; i++)
		{
			Temp1[i]=1.0;
		}

           imgsmooth.NeiAveTemplate(dlgparam.TempH,dlgparam.TempW,dlgparam.TempCX,
	       dlgparam.TempCY,Temp1,dlgparam.fCoef);
		}
		else if (dlgparam.m_radiovalue=3) {
		   	float Temp[25];
			//设模板为平均模板
			Temp[0]=0.0;
			Temp[1]=1.0;
			Temp[2]=2.0;
			Temp[3]=1.0;
			Temp[4]=0.0;
			
			Temp[5]=1.0;
			Temp[6]=2.0;
			Temp[7]=4.0;
			Temp[8]=2.0;
			Temp[9]=1.0;
			
			Temp[10]=2.0;
			Temp[11]=4.0;
			Temp[12]=8.0;
			Temp[13]=4.0;
			Temp[14]=2.0;
			
			Temp[15]=1.0;
			Temp[16]=2.0;
			Temp[17]=4.0;
			Temp[18]=2.0;
			Temp[19]=1.0;
			
			Temp[20]=0.0;
			Temp[21]=1.0;
			Temp[22]=2.0;
			Temp[23]=1.0;
	        Temp[24]=0.0;
			imgsmooth.NeiAveTemplate(dlgparam.TempH,dlgparam.TempW,dlgparam.TempCX,
	       dlgparam.TempCY,Temp,dlgparam.fCoef); 

		} 
		else
			return;
		
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgsmooth.GetDimensions(),imgsmooth.m_nBitCountOut,imgsmooth.m_lpColorTable, imgsmooth.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
	}
}


void CDemo1View::OnMedianSmooth() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	MedianSmoothDlg dlgparam;
    if (dlgparam.DoModal()==TRUE) {
	
	ImageEnhance imgsmooth(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	imgsmooth.MedianSmooth(dlgparam.TempH,dlgparam.TempW,dlgparam.TempCX,dlgparam.TempCY);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgsmooth.GetDimensions(),imgsmooth.m_nBitCountOut,imgsmooth.m_lpColorTable, imgsmooth.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
	Invalidate();
	}
}

void CDemo1View::OnLaplaceSharp() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	ImageEnhance imgsharp(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	int TempH, TempW, TempCX, TempCY;
	float fCoef;
	float Temp[9];
	//设模板为平均模板
	Temp[0]=-1.0;
	Temp[1]=-1.0;
	Temp[2]=-1.0;
	Temp[3]=-1.0;
	Temp[4]=9.0;
	Temp[5]=-1.0;
	Temp[6]=-1.0;
	Temp[7]=-1.0;
	Temp[8]=-1.0;
    //初始化对话框变量
	TempH=3;
	TempW=3;
	TempCX=1;
	TempCY=1;
	fCoef=1.0;
    imgsharp.LapTemplate(TempH,TempW,TempCX,TempCY,Temp,fCoef);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgsharp.GetDimensions(),imgsharp.m_nBitCountOut,imgsharp.m_lpColorTable, imgsharp.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnQuickFFt() 
{
	// TODO: Add your command handler code here
		// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给transform_FFT类对象FFTTrans
	fftTrans.InputImgData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//快速傅立叶变换
	fftTrans.Fourier();
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(fftTrans.GetDimensions(),fftTrans.m_nBitCountOut,
		fftTrans.m_lpColorTable, fftTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}


void CDemo1View::OnQuickFFtReverse() 
{
	// TODO: Add your command handler code here
	fftTrans.RevFourier();
    //新建视图窗口，显示图像恢复的结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(fftTrans.GetDimensions(),fftTrans.m_nBitCountOut,
		fftTrans.m_lpColorTable, fftTrans.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}



void CDemo1View::OnIdealLowPass() 
{
	// TODO: Add your command handler code here
    CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//图像理想低通滤波
	imgfreq.IdealLowPassFilter(pDib->m_imgWidth,pDib->m_imgHeight,pDib->m_imgHeight/16);
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgfreq.GetDimensions(),imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
	
}

void CDemo1View::OnButterLowPass() 
{
	// TODO: Add your command handler code here
    CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//图像理想低通滤波
	imgfreq.ButterLowPassFilter(pDib->m_imgWidth,pDib->m_imgHeight,pDib->m_imgHeight/16);
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgfreq.GetDimensions(),imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnIdealHighPass() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//图像理想低通滤波
	imgfreq.IdealHighPassFilter(pDib->m_imgWidth,pDib->m_imgHeight,pDib->m_imgHeight/16);
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgfreq.GetDimensions(),imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnButterHighPass() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag=1;
	int width=pDib->GetDimensions().cx;
	int height=pDib->GetDimensions().cy;
	int i=1;
	while(i<width)
		i=i*2;
	if(i!=width)//图像宽不是2的幂次方
		isValidateFlag=0;
	
	i=1;
	while(i<height)
		i=i*2;
	if(i!=height)//图像高不是2的幂次方
		isValidateFlag=0;
	
	if(isValidateFlag==0){
		::MessageBox(0,"输入图像的宽和高应为2的幂次方",MB_OK,0);
		return ;
	}
	
	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//图像理想低通滤波
	imgfreq.ButterHighPassFilter(pDib->m_imgWidth,pDib->m_imgHeight,pDib->m_imgHeight/16);
	
	//新建视图窗口，显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(imgfreq.GetDimensions(),imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnHarrWaveletTrans() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	wavltTrans.InputImageData(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    wavltTrans.BasicWaveletTrans(2);
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(wavltTrans.GetDimensions(),wavltTrans.m_nBitCountOut,wavltTrans.m_pColorTableOut,wavltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnHarrWavRevTrans() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
    wavltTrans.BasicWaveletReverse();
	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(wavltTrans.GetDimensions(),wavltTrans.m_nBitCountOut,wavltTrans.m_pColorTableOut,wavltTrans.m_pImgDataOut);
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnErosion() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;

	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
   
	//定义结构元素模板，默认为3*3模板
/*
    int i;
	for (i=0;i<TempH*TempW;i++) {
		m_InitTemp[i]=0;
	}	
*/
	//模板1
	morTrans.m_InitTemp[1]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[5]=1;
	morTrans.m_InitTemp[7]=1;
	
/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if(morTrans.m_TempBuf){
		delete []morTrans.m_TempBuf;
		morTrans.m_TempBuf=NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf=new int[morTrans.TempW*morTrans.TempH];
	int i0, j0;
	for(i0=0;i0<morTrans.TempH;i0++){
		for(j0=0;j0<morTrans.TempW;j0++){
			morTrans.m_TempBuf[i0*morTrans.TempW+j0]=morTrans.m_InitTemp[i0*morTrans.TempW+j0];
		}
	}
	
	morTrans.ImgErosion(morTrans.m_pImgData,morTrans.m_pImgDataOut,morTrans.m_imgWidth,morTrans.m_imgHeight,
		morTrans.m_TempBuf, morTrans.TempW, morTrans.TempH);
/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnDilation() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;

	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
   
	//定义结构元素模板，默认为3*3模板
/*
    int i;
	for (i=0;i<TempH*TempW;i++) {
		m_InitTemp[i]=0;
	}	
*/
	//模板1
	morTrans.m_InitTemp[1]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[5]=1;
	morTrans.m_InitTemp[7]=1;
	
/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if(morTrans.m_TempBuf){
		delete []morTrans.m_TempBuf;
		morTrans.m_TempBuf=NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf=new int[morTrans.TempW*morTrans.TempH];
	int i0, j0;
	for(i0=0;i0<morTrans.TempH;i0++){
		for(j0=0;j0<morTrans.TempW;j0++){
			morTrans.m_TempBuf[i0*morTrans.TempW+j0]=morTrans.m_InitTemp[i0*morTrans.TempW+j0];
		}
	}
	
	morTrans.ImgDilation(morTrans.m_pImgData,morTrans.m_pImgDataOut,morTrans.m_imgWidth,morTrans.m_imgHeight,
		morTrans.m_TempBuf, morTrans.TempW, morTrans.TempH);
/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemo1View::OnOpen() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;

	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
   
	//定义结构元素模板，默认为3*3模板
/*
    int i;
	for (i=0;i<TempH*TempW;i++) {
		m_InitTemp[i]=0;
	}	
*/
	//模板1
	morTrans.m_InitTemp[1]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[5]=1;
	morTrans.m_InitTemp[7]=1;
	
/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if(morTrans.m_TempBuf){
		delete []morTrans.m_TempBuf;
		morTrans.m_TempBuf=NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf=new int[morTrans.TempW*morTrans.TempH];
	int i0, j0;
	for(i0=0;i0<morTrans.TempH;i0++){
		for(j0=0;j0<morTrans.TempW;j0++){
			morTrans.m_TempBuf[i0*morTrans.TempW+j0]=morTrans.m_InitTemp[i0*morTrans.TempW+j0];
		}
	}
	
	morTrans.Open();
	
	/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnClose() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;

	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
   
	//定义结构元素模板，默认为3*3模板
/*
    int i;
	for (i=0;i<TempH*TempW;i++) {
		m_InitTemp[i]=0;
	}	
*/
	//模板1
	morTrans.m_InitTemp[1]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[5]=1;
	morTrans.m_InitTemp[7]=1;
	
/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if(morTrans.m_TempBuf){
		delete []morTrans.m_TempBuf;
		morTrans.m_TempBuf=NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf=new int[morTrans.TempW*morTrans.TempH];
	int i0, j0;
	for(i0=0;i0<morTrans.TempH;i0++){
		for(j0=0;j0<morTrans.TempW;j0++){
			morTrans.m_TempBuf[i0*morTrans.TempW+j0]=morTrans.m_InitTemp[i0*morTrans.TempW+j0];
		}
	}
	
	morTrans.Close();
	
	/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}




void CDemo1View::OnThinning() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}
	
	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
    morTrans.ImgThinning();

	/*	*/
	//显示变换结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(morTrans.GetDimensions(),morTrans.m_nBitCountOut,morTrans.m_lpColorTable, morTrans.m_pImgDataOut);
	pView->OnInitialUpdate();		
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnRobert() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Roberts算子
	segment.Roberts();
	
	//新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();
}

void CDemo1View::OnSobel() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Sobel算子
	segment.Sobel();
	
	//新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();	
}

void CDemo1View::OnPrewitt() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Prewitt算子
	segment.Prewitt();
	
	//新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();	
}

void CDemo1View::OnLaplacian() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度和彩色图像
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//Laplacian算子
	segment.Laplacian();
	
	//新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();	
}

void CDemo1View::OnRegionGrow() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
    
	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//设定种子点位置
	CPoint Pos(segment.m_imgWidth/2,segment.m_imgHeight/2);
	
	//阈值
	int nThresh=3;

	segment.RegionGrow(Pos,nThresh);
	
    //新建视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemo1View::OnEdgeTrace() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}
	//定义图像分割类对象segment，用当前DIB对其初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//边界跟踪
	segment.EdgeTrace();
	
	//新建视图，显示处理结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();
}

void CDemo1View::OnAdaptThresholdSeg() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度",MB_OK,0);
		return ;
	}
	//定义图像分割类对象segment，用当前DIB对其初始化
	ImgSegment segment(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//自适应阈值分割
	segment.AdaptThreshSeg(pDib->m_pImgData);
	
	//新建视图，显示处理结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->m_dib;
	dibNew->ReplaceDib(segment.GetDimensions(),segment.m_nBitCountOut,segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);	
	Invalidate();

}

void CDemo1View::OnHistThreshSeg() 
{
	// TODO: Add your command handler code here
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->m_dib;
	
	//异常判断
	if(pDib->m_nBitCount!=8&&pDib->m_nBitCount!=24){
		::MessageBox(0,"只处理灰度和彩色图像",MB_OK,0);
		return ;
	}
	
	//直方图绘制对话框
	HistSegmentDlg dlgHist;
	dlgHist.m_himageDib.ReplaceDib(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//显示直方图
  if (dlgHist.DoModal()==IDOK) {
	  GrayTrans gray(dlgHist.m_himageDib.GetDimensions(),dlgHist.m_himageDib.m_nBitCount,
		  dlgHist.m_himageDib.m_lpColorTable, dlgHist.m_himageDib.m_pImgData);
      gray.BinaryImage(dlgHist.m_threshchange);
	  //新建视图，显示处理结果
	  CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	  pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	  CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	  CDemo1Doc* pDocNew=pView->GetDocument();
	  ImageDib *dibNew=pDocNew->m_dib;
	  dibNew->ReplaceDib(gray.GetDimensions(),gray.m_nBitCountOut,gray.m_lpColorTableOut, gray.m_pImgDataOut);
	  pView->OnInitialUpdate();
	  pDocNew->SetModifiedFlag(TRUE);
	  pDocNew->UpdateAllViews(pView);	
      Invalidate();

  } 
  else
	  return;

}

void CDemo1View::OnColorPair() 
{
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	

	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1=pDoc1->GetPDib();
	ImageDib *pdib2=pDoc2->GetPDib();
	
	ImageDib *pdibtemp;
	
	//处理一幅彩色和一幅二值图像
	if(pdib1->m_nBitCount==24){
		if(pdib2->m_nBitCount==24){
			::MessageBox(0,"需要一幅二值图像",MB_OK,0);
			return ;
		}
	}
	else{
		if(pdib2->m_nBitCount==24){
			pdibtemp=pdib1;
			pdib1=pdib2;
			pdib2=pdibtemp;
		}
		else{
			::MessageBox(0,"需要一幅彩色图像",MB_OK,0);
			return ;
		}
	}

	
	//只处理彩色图像
	if(pdib1->m_nBitCount!=24){
		::MessageBox(0,"只处理彩色图像",MB_OK,0);
		return ;
	}
	
	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	PlateTrans colorpair(pdib1->GetDimensions(),pdib1->m_nBitCount,
		pdib1->m_lpColorTable, pdib1->m_pImgData,pdib2->m_pImgData);
	
	//有色点对搜索
	colorpair.ColorPairSearch();
	
	//发送新建文件的消息，创建一个新的文档-视图
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	//获取新建视图指针
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	
	//获取相关联的新的文档类指针
	CDemo1Doc* pDocNew=pView->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *dibNew=pDocNew->GetPDib();

	//将变换后的输出图像作为新建文档的DIB进行显示
	dibNew->ReplaceDib(colorpair.GetDimensions(),colorpair.m_nBitCountOut,colorpair.m_lpColorTableOut, colorpair.m_pImgDataOut);
	
	//设置滚动窗口
	pView->OnInitialUpdate();


	//文档数据置脏，提示存盘信息
	pDocNew->SetModifiedFlag(TRUE);

	//各视图刷新显示
	pDocNew->UpdateAllViews(pView);

}

void CDemo1View::OnColorToGray() 
{
	//获取文档类指针
	CDemo1Doc *pDoc=GetDocument();
	//获取ImgCenterDib类对象m_dib的指针，访问当前DIB数据
	ImageDib *pDib=pDoc->GetPDib();
	
	//只处理彩色图像

	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	//彩色变灰度格式
	graytrans.ColorToGray();
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//发送新建文件的消息，创建一个新的文档-视图
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	
	//获取新建视图指针
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	
	//获取相关联的新的文档类指针
	CDemo1Doc* pDocNew=pView->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *dibNew=pDocNew->GetPDib();

	//将变换后的输出图像作为新建文档的DIB进行显示
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	
	//设置滚动窗口
	pView->OnInitialUpdate();


	//文档数据置脏，提示存盘信息
	pDocNew->SetModifiedFlag(TRUE);

	//各视图刷新显示
	pDocNew->UpdateAllViews(pView);
}

void CDemo1View::Onplatelocate() 
{

	//获取文档类中m_dib的指针，访问当前DIB数据
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->GetPDib();
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	//判断是否二值图像
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	int m_defineMask[25];
	for(i=0;i<25;i++) m_defineMask[i]=0;
	m_defineMask[7]=1;
	m_defineMask[11]=1;
	m_defineMask[12]=1;
	m_defineMask[13]=1;
	m_defineMask[17]=1;
	morph.InputMask(m_defineMask,5,5);
	
	morph.Open();
	
	//新建视图，显示处理结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut,morph.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);		
	
	int times;

	for(times=0;times<10;times++){
		morph.ImgErosion(morph.m_pImgData,morph.m_pImgDataOut,morph.m_imgWidth,morph.m_imgHeight,
		morph.m_TempBuf, morph.TempW, morph.TempH);
		dibNew->ReplaceDib(size,morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData=dibNew->m_pImgData;
	}
	
	for(times=0;times<12;times++){
		morph.ImgDilation(morph.m_pImgData,morph.m_pImgDataOut,morph.m_imgWidth,morph.m_imgHeight,
		morph.m_TempBuf, morph.TempW, morph.TempH);
		dibNew->ReplaceDib(size,morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData=dibNew->m_pImgData;
	}

	for(times=0;times<2;times++){
		morph.ImgErosion(morph.m_pImgData,morph.m_pImgDataOut,morph.m_imgWidth,morph.m_imgHeight,
		morph.m_TempBuf, morph.TempW, morph.TempH);
		dibNew->ReplaceDib(size,morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData=dibNew->m_pImgData;
	}

	///粗寻找车牌纵向坐标
	int x1,x2,xx1,xx2,y1,y2,yy1,yy2,pcount;
	float factor;
	flag=0;
	x1=x2=y1=y2=1;
	xx1=xx2=yy1=yy2=1;
	for(i=20;i<size.cy-20;i++){
		pcount=0;
		for(j=20;j<size.cx-20;j++){
			if(*(dibNew->m_pImgData+i*lineByte+j)==0)	pcount++;
		}
		if(pcount>20)
		{
			if(flag==0){
				y1=i;
				y2=i;
				flag=1;
			}
			else{
				y2=i;
			}
		}
		else{
			if(flag==1){
				if((y2-y1)<5){
					flag=0;
					y1=1;
					y2=1;
				}
				else{
					yy1=y1;
					yy2=y2;
					flag=0;
					break;
				}
			}
		}
	}

///粗寻找车牌横向坐标
	flag=0;
	for(i=20;i<size.cx-20;i++){
		pcount=0;
		for(j=yy1;j<=yy2;j++){
			if(*(dibNew->m_pImgData+j*lineByte+i)==0)	pcount++;
		}
		if(pcount>10){
			if(flag==0){
				x1=i;
				x2=i;
				flag=1;
			}
			else{
				x2=i;
			}
		}
		else{
			if(flag==1){
				if((x2-x1)<5){
					flag=0;
					x1=1;
					x2=1;
				}
				else{
					xx1=x1;
					xx2=x2;
					flag=0;
				}
			}
		}
	}

///细寻找车牌纵向坐标
	flag=0;
	for(i=yy1;i<=yy2;i++){
		pcount=0;
		for(j=xx1;j<=xx2;j++){
			if(*(dibNew->m_pImgData+i*lineByte+j)==0)	pcount++;
	    }
		if(pcount>10){
			if(flag==0){
				y1=i;
				y2=i;
				flag=1;
			}
			else{
				y2=i;
			}
		}
		else{
			if(flag==1){
				if((y2-y1)<5){
					flag=0;
					y1=1;
					y2=1;
				}
			}
		}
	}
	yy1=y1;
	yy2=y2;
	factor=(float)(xx2-xx1)/(yy2-yy1);
	
	
	int Refpcount,frq,dirc;
	Refpcount=0;
	frq=0;
	dirc=1;

	for(j=yy1;j<yy2;j++){
		if(*(pDib->m_pImgData+j*lineByte+xx1)==0)	Refpcount++;
	}
	
	for(i=xx1;i<xx2;i++){
		for(j=yy1;j<yy2;j++){
			if(*(pDib->m_pImgData+j*lineByte+i)==0)	pcount++;
		}
		if(dirc==1){
			if(pcount>Refpcount) Refpcount=pcount;
			if(Refpcount>2*pcount){
				frq++;
				dirc=0;
				Refpcount=pcount;
			}
			pcount=0;
		}
		else{
			if(pcount<Refpcount) Refpcount=pcount;
			if(pcount>2*Refpcount){
				frq++;
				dirc=1;
				Refpcount=pcount;
			}
			pcount=0;
		}
	}
	if(factor>2&&factor<4&&frq>5){
		for(i=yy1-20;i<yy2+20;i++){
			*(pDib->m_pImgData+i*lineByte+xx1)=0;
			*(pDib->m_pImgData+i*lineByte+xx2)=0;
		}
		for(i=xx1-20;i<xx2+20;i++){
			*(pDib->m_pImgData+yy1*lineByte+i)=0;
			*(pDib->m_pImgData+yy2*lineByte+i)=0;
		}

		int pangle[100],pavg[500],pnum,psum,pdiff;

		for(i=0;i<100;i++) pangle[i]=0;

		for(i=xx1;i<xx2;i++){
			pnum=0;
			psum=0;
			for(j=yy1;j<yy2;j++){
				if(*(dibNew->m_pImgData+j*lineByte+i)==0){
					psum+=j;
					pnum++;
				}
			}
			pavg[i-xx1]=psum/pnum;
		}
		
		for(i=xx1+10;i<xx2-50;i++){
			pnum=0;
			psum=0;
			pdiff=pavg[i-xx1+40]-pavg[i-xx1];
			if(pdiff>49) pdiff=49;
			if(pdiff<-50) pdiff=-50;
			pangle[50+pdiff]++;
		}
		
		int pglb,lglb;
		float angle;

		pglb=pangle[0];
		lglb=0;
		for(i=1;i<100;i++){
			if(pangle[i]>pglb){
				pglb=pangle[i];
				lglb=i;
			}
		}



		angle=atan((float)(lglb-50)/40.0)*180/3.14;
		CString angle_str;
		angle_str.Format("%4.2f",angle);
		MessageBox("角度:"+angle_str+"度","旋转校正角度",MB_ICONINFORMATION);
	}
}

void CDemo1View::Onplaterelocate() 
{
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	
	//获取新建视图指针
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	pFrame->MDINext();
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	

	//获取相关联的新的文档类指针
	CDemo1Doc* pDoc1=pView1->GetDocument();
	CDemo1Doc* pDoc2=pView2->GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pDib1=pDoc1->GetPDib();
	ImageDib *pDib=pDoc2->GetPDib();
	
	//处理一幅彩色和一幅二值图像
	if(pDib1->m_nBitCount==8){
		if(pDib->m_nBitCount==24){
			::MessageBox(0,"不需要彩色图像",MB_OK,0);
			return ;
		}
	}
	else{
			::MessageBox(0,"不需要彩色图像",MB_OK,0);
			return ;
	}
	
	int i, j;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	int flag=1;
	for(i=0;i<size.cy;i++){
		for(j=0; j<size.cx;j++){
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if(*(pDib->m_pImgData+i*lineByte+j)!=0
				&&*(pDib->m_pImgData+i*lineByte+j)!=255){
				flag=0;
				break;
			}
		}
	}
	//非二值图像
	if(!flag){
		::MessageBox(0,"需要输入二值图像",MB_OK,0);
		return ;
	}

	Morphology morph(size,pDib->m_nBitCount,pDib->m_lpColorTable, pDib->m_pImgData);

	int m_defineMask[25];
	for(i=0;i<25;i++) m_defineMask[i]=0;
	m_defineMask[7]=1;
	m_defineMask[11]=1;
	m_defineMask[12]=1;
	m_defineMask[13]=1;
	m_defineMask[17]=1;
	morph.InputMask(m_defineMask,5,5);
	
	//新建视图，显示处理结果
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(morph.GetDimensions(),morph.m_nBitCountOut,morph.m_lpColorTableOut,morph.m_pImgDataOut);
	pView->OnInitialUpdate();
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);		
	
	int times;

	for(times=0;times<10;times++){
		morph.ImgErosion(morph.m_pImgData,morph.m_pImgDataOut,morph.m_imgWidth,morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		dibNew->ReplaceDib(size,morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData=dibNew->m_pImgData;
	}
	
	for(times=0;times<12;times++){
		morph.ImgDilation(morph.m_pImgData,morph.m_pImgDataOut,morph.m_imgWidth,morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		dibNew->ReplaceDib(size,morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData=dibNew->m_pImgData;
	}

	for(times=0;times<2;times++){
		morph.ImgErosion(morph.m_pImgData,morph.m_pImgDataOut,morph.m_imgWidth,morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		dibNew->ReplaceDib(size,morph.m_nBitCountOut,morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData=dibNew->m_pImgData;
	}

	///粗寻找车牌纵向坐标
	int x1,x2,xx1,xx2,y1,y2,yy1,yy2,pcount;
	float factor;
	flag=0;
	x1=x2=y1=y2=1;
	xx1=xx2=yy1=yy2=1;
	for(i=20;i<size.cy-20;i++){
		pcount=0;
		for(j=20;j<size.cx-20;j++){
			if(*(dibNew->m_pImgData+i*lineByte+j)==0)	pcount++;
		}
		if(pcount>20)
		{
			if(flag==0){
				y1=i;
				y2=i;
				flag=1;
			}
			else{
				y2=i;
			}
		}
		else{
			if(flag==1){
				if((y2-y1)<5){
					flag=0;
					y1=1;
					y2=1;
				}
				else{
					yy1=y1;
					yy2=y2;
					flag=0;
					break;
				}
			}
		}
	}

///粗寻找车牌横向坐标
	flag=0;
	for(i=20;i<size.cx-20;i++){
		pcount=0;
		for(j=yy1;j<=yy2;j++){
			if(*(dibNew->m_pImgData+j*lineByte+i)==0)	pcount++;
		}
		if(pcount>10){
			if(flag==0){
				x1=i;
				x2=i;
				flag=1;
			}
			else{
				x2=i;
			}
		}
		else{
			if(flag==1){
				if((x2-x1)<5){
					flag=0;
					x1=1;
					x2=1;
				}
				else{
					xx1=x1;
					xx2=x2;
					flag=0;
				}
			}
		}
	}

///细寻找车牌纵向坐标
	flag=0;
	for(i=yy1;i<=yy2;i++){
		pcount=0;
		for(j=xx1;j<=xx2;j++){
			if(*(dibNew->m_pImgData+i*lineByte+j)==0)	pcount++;
	    }
		if(pcount>10){
			if(flag==0){
				y1=i;
				y2=i;
				flag=1;
			}
			else{
				y2=i;
			}
		}
		else{
			if(flag==1){
				if((y2-y1)<5){
					flag=0;
					y1=1;
					y2=1;
				}
			}
		}
	}
	yy1=y1;
	yy2=y2;
	factor=(float)(xx2-xx1)/(yy2-yy1);
	
	
	int Refpcount,frq,dirc;
	Refpcount=0;
	frq=0;
	dirc=1;

	for(j=yy1;j<yy2;j++){
		if(*(pDib->m_pImgData+j*lineByte+xx1)==0)	Refpcount++;
	}
	
	for(i=xx1;i<xx2;i++){
		for(j=yy1;j<yy2;j++){
			if(*(pDib->m_pImgData+j*lineByte+i)==0)	pcount++;
		}
		if(dirc==1){
			if(pcount>Refpcount) Refpcount=pcount;
			if(Refpcount>2*pcount){
				frq++;
				dirc=0;
				Refpcount=pcount;
			}
			pcount=0;
		}
		else{
			if(pcount<Refpcount) Refpcount=pcount;
			if(pcount>2*Refpcount){
				frq++;
				dirc=1;
				Refpcount=pcount;
			}
			pcount=0;
		}
	}
	if(factor>2&&factor<4&&frq>5){
		for(i=yy1-20;i<yy2+20;i++){
			*(pDib->m_pImgData+i*lineByte+xx1)=0;
			*(pDib->m_pImgData+i*lineByte+xx2)=0;
		}
		for(i=xx1-20;i<xx2+20;i++){
			*(pDib->m_pImgData+yy1*lineByte+i)=0;
			*(pDib->m_pImgData+yy2*lineByte+i)=0;
		}
	}

	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pViewNew1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew1=pViewNew1->GetDocument();
	ImageDib *dibNew1=pDocNew1->GetPDib();
	pViewNew1->OnInitialUpdate();
	pDocNew1->SetModifiedFlag(TRUE);
	pDocNew1->UpdateAllViews(pView);	
	
	size.cx=xx2-xx1+10;
	size.cy=yy2-yy1+10;

	int lineByte1=(size.cx+3)/4*4;
	
	unsigned char *pDib_imgdata=new BYTE [size.cy*((size.cx+3)/4*4)];

	for(i=0;i<size.cx;i++){
		for(j=0;j<size.cy;j++){
			*(pDib_imgdata+j*lineByte1+i)=*(pDib1->m_pImgData+(yy1+j-5)*lineByte+i+xx1-5);
		}
	}
	dibNew1->ReplaceDib(size,pDib->m_nBitCount,pDib->m_lpColorTable,pDib_imgdata);


}

void CDemo1View::Oncharseparate() 
{
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->GetPDib();
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}

	int i,j,thd;
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx+3)/4*4;
	long pixel_scales=0;

	for(i=0;i<size.cx;i++){
		for(j=0;j<size.cy;j++){
			pixel_scales+=*(pDib->m_pImgData+j*lineByte+i);		
		}
	}
	
	thd=pixel_scales/(size.cx*size.cy);

	GrayTrans graytrans(size,pDib->m_nBitCount, pDib->m_lpColorTable, pDib->m_pImgData);
	
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.BinaryImage(thd);
	
	//建立一个新视图，显示分割结果
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew=pView->GetDocument();
	ImageDib *dibNew=pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(),graytrans.m_nBitCountOut,graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate();	
	pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);

	//去除上下左边框
	int pixels=0,curpixels=0,yy1,yy2,xx1,xx2,flag=0;
	
	yy1=0;
	yy2=size.cy-1;
	
	for(j=0;j<size.cx;j++){
		if(*(dibNew->m_pImgData+j)==255) curpixels++;
	}
	pixels=curpixels;
	
	for(i=1;i<size.cy/2;i++){
		curpixels=0;
		for(j=0;j<size.cx;j++){
			if(*(dibNew->m_pImgData+i*lineByte+j)==255) curpixels++;
		}
		if(flag==0){
			if(curpixels>pixels)	pixels=curpixels;
			if(pixels-curpixels>0.5*pixels){
				flag=1;
				pixels=curpixels;
			}
		}
		else{
			if(curpixels<pixels)	pixels=curpixels;
			if(curpixels-pixels>0.5*curpixels){
				yy1=i;
				flag=0;
			}
		}
	}

	flag=0;
	for(j=0;j<size.cx;j++){
		if(*(dibNew->m_pImgData+(size.cy-1)*lineByte+j)==255) curpixels++;
	}
	pixels=curpixels;
	
	for(i=size.cy-2;i>size.cy/2;i--){
		curpixels=0;
		for(j=0;j<size.cx;j++){
			if(*(dibNew->m_pImgData+i*lineByte+j)==255) curpixels++;
		}
		if(flag==0){
			if(curpixels>pixels)	pixels=curpixels;
			if(pixels-curpixels>0.5*pixels){
				flag=1;
				pixels=curpixels;
			}
		}
		else{
			if(curpixels<pixels)	pixels=curpixels;
			if(curpixels-pixels>0.5*curpixels){
				yy2=i;
				flag=0;
			}
		}
	}

	flag=0;
	for(i=yy1;i<yy2;i++){
		if(*(dibNew->m_pImgData+i*lineByte)==255) curpixels++;
	}
	pixels=curpixels;
	
	for(j=1;j<size.cx/10;j++){
		curpixels=0;
		for(i=yy1;i<yy2;i++){
			if(*(dibNew->m_pImgData+i*lineByte+j)==255) curpixels++;
		}
		if(flag==0){
			if(curpixels>pixels)	pixels=curpixels;
			if(pixels-curpixels>0.5*pixels){
				flag=1;
				pixels=curpixels;
			}
		}
		else{
			if(curpixels<pixels)	pixels=curpixels;
			if(curpixels-pixels>0.5*curpixels){
				xx1=j;
				flag=0;
			}
		}
	}

	xx2=size.cx-1;

	//建立一个新视图，显示去除边框结果
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView1=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew1=pView1->GetDocument();
	ImageDib *dibNew1=pDocNew1->GetPDib();
	pView1->OnInitialUpdate();	
	pDocNew1->SetModifiedFlag(TRUE);
	pDocNew1->UpdateAllViews(pView1);

	size.cx=xx2-xx1+1;
	size.cy=yy2-yy1+1;

	int lineByte1=(size.cx+3)/4*4;
	
	unsigned char *pDib_imgdata1=new BYTE [size.cy*lineByte1];

	for(i=0;i<size.cx;i++){
		for(j=0;j<size.cy;j++){
			*(pDib_imgdata1+j*lineByte1+i)=*(dibNew->m_pImgData+(yy1+j)*lineByte+i+xx1);
		}
	}
	dibNew1->ReplaceDib(size,pDib->m_nBitCount,pDib->m_lpColorTable,pDib_imgdata1);

	//字符分割

	int char_x1[7],char_x2[7],char_xx1[7],char_xx2[7],k=0,char_width,char_gap;
	yy1=0;
	yy2=size.cy;
	xx1=0;
	xx2=size.cx;
	flag=0;
	lineByte=(size.cx+3)/4*4;
	curpixels=0;

	char_x1[k]=0;
	
	for(i=0;i<size.cy;i++){
		if(*(dibNew1->m_pImgData+i*lineByte)==255) curpixels++;
	}
	pixels=curpixels;
	
	for(j=1;j<size.cx;j++){
		curpixels=0;
		for(i=0;i<size.cy;i++){
			if(*(dibNew1->m_pImgData+i*lineByte+j)==255) curpixels++;
		}
		if(flag==0){
			if(curpixels>pixels)	pixels=curpixels;
			if(curpixels<3){
				char_x2[k]=j;
				flag=1;
				pixels=curpixels;
				k++;
				if(k==7) break;
			}
		}
		else{
			if(curpixels<pixels)	pixels=curpixels;
			if(curpixels>3){
				char_x1[k]=j;
				flag=0;
			}
		}
	}

	char_gap=char_x1[1]-char_x2[0];
	for(i=2;i<7;i++){
		if(char_x1[i]-char_x2[i-1]<char_gap) char_gap=char_x1[i]-char_x2[i-1];
	}
	
	char_width=char_gap+char_x2[0]-char_x1[0]+1;

	char_xx1[0]=char_x1[0]-char_gap/2;
	char_xx2[0]=min(char_x2[0]+max(char_gap,char_width-char_x2[0]+char_x1[0])/2-1,char_x1[1]);

	for(i=1;i<6;i++)
	{
		char_xx1[i]=max(char_x1[i]-max(char_gap,char_width-char_x2[i]+char_x1[i])/2-1,char_xx2[i-1]);
		char_xx2[i]=min(char_x2[i]+max(char_gap,char_width-char_x2[i]+char_x1[i])/2-1,char_x1[i+1]);

	}

	char_xx1[6]=max(char_x1[6]-max(char_gap,char_width-char_x2[6]+char_x1[6])/2-1,char_xx2[5]);
	char_xx2[6]=char_x2[6]+max(char_gap,char_width-char_x2[6]+char_x1[6])/2-1;

	//建立一个新视图，显示字符分割结果
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView2=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew2=pView2->GetDocument();
	ImageDib *dibNew2=pDocNew2->GetPDib();
	pView2->OnInitialUpdate();	
	pDocNew2->SetModifiedFlag(TRUE);
	pDocNew2->UpdateAllViews(pView2);

	CSize size1;
	size1.cx=char_xx2[6]-char_xx1[0]+1;
	size1.cy=size.cy;
	int lineByte2=(size1.cx+3)/4*4;
	
	unsigned char *pDib_imgdata2=new BYTE [size1.cy*lineByte2];

	for(i=0;i<size1.cx;i++){
		for(j=0;j<size1.cy;j++){
			if(i+char_xx1[0]>=0&&i+char_xx1[0]<size.cx){
				*(pDib_imgdata2+j*lineByte2+i)=*(dibNew1->m_pImgData+j*lineByte1+i+char_xx1[0]);
			}
			else{
				*(pDib_imgdata2+j*lineByte2+i)=0;
			}
		}
	}

	dibNew2->ReplaceDib(size1,pDib->m_nBitCount,pDib->m_lpColorTable,pDib_imgdata2);
	
	for(j=0;j<size1.cy;j++){
		*(dibNew2->m_pImgData+j*lineByte2+char_xx2[0]-char_xx1[0])=255;
	}
	for(i=1;i<6;i++){
		for(j=0;j<size1.cy;j++){
			*(dibNew2->m_pImgData+j*lineByte2+char_xx1[i]-char_xx1[0])=255;
			*(dibNew2->m_pImgData+j*lineByte2+char_xx2[i]-char_xx1[0])=255;
		}
	}
	for(j=0;j<size1.cy;j++){
		*(dibNew2->m_pImgData+j*lineByte2+char_xx1[6]-char_xx1[0])=255;
	}

	

	//字符归一化
	
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemo1View* pView3=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDocNew3=pView3->GetDocument();
	ImageDib *dibNew3=pDocNew3->GetPDib();
	pView3->OnInitialUpdate();	
	pDocNew3->SetModifiedFlag(TRUE);
	pDocNew3->UpdateAllViews(pView3);

	CSize size2;
	size2.cx=160;
	size2.cy=60;
	int lineByte3=(size2.cx+3)/4*4;
	
	unsigned char *pDib_imgdata3=new BYTE [size2.cy*lineByte3];

	float x_ratio,y_ratio;
    int pnums,x1,y1;
    
	for(i=0;i<size2.cx;i++)
		for(j=0;j<size2.cy;j++)
			*(pDib_imgdata3+j*lineByte3+i)=255;

	for(i=0;i<7;i++){
        x_ratio=(char_xx2[i]-char_xx1[i]+1)/20.0;
        y_ratio=size1.cy/40.0;
        for(j=0;j<20;j++){
			for(k=0;k<40;k++){
				pnums=0;
				for(x1=j*x_ratio;x1<(j+1)*x_ratio;x1++){
					for(y1=k*y_ratio;y1<(k+1)*y_ratio;y1++){
                            if(*(pDib_imgdata2+y1*lineByte2+char_xx1[i]+x1)==255)	pnums++;
                    }
				}
                if((pnums/(x_ratio*y_ratio))>0.5){
                    *(pDib_imgdata3+(k+10)*lineByte3+i*20+j+9)=0;
                }
                else{
                    *(pDib_imgdata3+(k+10)*lineByte3+i*20+j+9)=255;
                }
            }
        }
	}
	dibNew3->ReplaceDib(size2,pDib->m_nBitCount,pDib->m_lpColorTable,pDib_imgdata3);

}

void CDemo1View::Oncharrecognition() 
{
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CDemo1View* pView=(CDemo1View*)pFrame->MDIGetActive()->GetActiveView();
	CDemo1Doc* pDoc=pView->GetDocument();
	ImageDib *pDib=pDoc->GetPDib();
	
	//只处理灰度图像
	if(pDib->m_nBitCount!=8){
		::MessageBox(0,"只处理灰度图像",MB_OK,0);
		return ;
	}
	
	int i,j,k,lineByte;
	lineByte=(20*7+20+3)/4*4;
	///去噪声
    for(i=1;i<159;i++)
		for(j=1;j<59;j++)
			if(*(pDib->m_pImgData+j*lineByte+i)==0){
				if(*(pDib->m_pImgData+j*lineByte+i-1)==255){
					if(*(pDib->m_pImgData+(j-1)*lineByte+i)==255){
						if(*(pDib->m_pImgData+(j+1)*lineByte+i)==255){
							if(*(pDib->m_pImgData+j*lineByte+i+1)==255){
								*(pDib->m_pImgData+j*lineByte+i)=255;
							}
						}
					}
				}
			}
	PlateTrans charrecog(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData,NULL);

	charrecog.charrecognize();

	
}
