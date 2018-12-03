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
#include "FeatureDetect.h"
#include "TextureResult.h"
#include "ModelBuildResult.h"

#define   HN       9             //隐含层神经元个数, 请多次修改其值来进行比较
#define   InputN   4
#define   OutN     1
#define   datanum  12              //样本数
#define   g(x)     1/(1+exp(-x))  //sigmod函数
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
	ON_COMMAND(id_FeatureDetect, OnFeatureDetect)
	ON_COMMAND(id_Modelbuild, OnModelbuild)
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

void CDemo1View::OnFeatureDetect() 
{
	CDemo1Doc *pDoc=GetDocument();
	ImageDib *pDib=pDoc->GetPDib();
	
	CSize size=pDib->GetDimensions();
	int lineByte=(size.cx*3+3)/4*4;
	

	double dEnergy			  = 0.0;
	double dEntropy			  = 0.0;
	double dInertiaQuadrature = 0.0;
	double dLocalCalm		  = 0.0;
	double dCorrelation		  = 0.0;
	double dEnergy1			  = 0.0;
	double dEntropy1		  = 0.0;
	double dInertiaQuadrature1= 0.0;
	double dLocalCalm1		  = 0.0;
	double dCorrelation1	  = 0.0;
	unsigned char** arLocalImage;
	
	FeatureDetect features(pDib->GetDimensions(),pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	
	int rolltimeH = features.m_imgHeight/features.FilterWindowWidth;
	int rolltimeW = features.m_imgWidth/features.FilterWindowWidth;
	int i,j;
	int p,q;

	arLocalImage=(unsigned char **)calloc((unsigned)features.FilterWindowWidth,sizeof(unsigned char*));
	for(i=0;i<features.FilterWindowWidth;i++) {
		arLocalImage[i]=(unsigned char *)calloc((unsigned)features.FilterWindowWidth,sizeof(unsigned char));
	}
	
	//将图像分成若干个窗口，计算其纹理均值
	for(i=0;i<rolltimeH;i++){
		for(j=0;j<rolltimeW;j++){
			//首先赋值给子窗口
			for(p=0;p<features.FilterWindowWidth;p++){
				for(q=0;q<features.FilterWindowWidth;q++){
					arLocalImage[p][q] = *(features.m_pImgData+(i*features.FilterWindowWidth+p)*lineByte+(j*features.FilterWindowWidth+q)*3);  
				}
			}
			features.ComputeMatrix(arLocalImage,features.FilterWindowWidth);
			features.ComputeFeature(dEnergy1, dEntropy1, dInertiaQuadrature1, dCorrelation1, dLocalCalm1, features.PMatrixH, features.GrayLayerNum);
			dEnergy              += dEnergy1;
			dEntropy             += dEntropy1;
			dInertiaQuadrature   += dInertiaQuadrature1;
			dCorrelation         += dCorrelation1;
			dLocalCalm           += dLocalCalm1;
		}
	}
	dEnergy              /= (rolltimeH*rolltimeW);
	dEntropy             /= (rolltimeH*rolltimeW);
	dInertiaQuadrature   /= (rolltimeH*rolltimeW);
	dCorrelation         /= (rolltimeH*rolltimeW);
	dLocalCalm           /= (rolltimeH*rolltimeW);

	TextureResult TextureDlg;

	TextureDlg.m_dEnergy			=  dEnergy;
	TextureDlg.m_dEntropy			=  dEntropy;
	TextureDlg.m_dInertiaQuadrature =  dInertiaQuadrature;
	TextureDlg.m_dLocalCalm			=  dLocalCalm;

	TextureDlg.DoModal(); 
}

void CDemo1View::OnModelbuild() 
{
	ModelBuildResult * ModelDlg;
	ModelDlg=new ModelBuildResult;
	ModelDlg->Create();

	CString m_result,m_result1;
	int  decimal, sign;
	char  buffer[200];
	
	double x_out[InputN];            //输入层的输出值
	double hn_out[HN];               //隐含层的输出值
	double y_out[OutN];              //输出层的输出值
	double y[OutN];                  //输出层的期望输出值
	
	double w[InputN][HN];            //输入层到隐含层的权值
	double v[HN][OutN];              //隐含层到输出层的权值

	double deltaw[InputN][HN];
	double deltav[HN][OutN];
	
	double hn_delta[HN];             //隐含层的delta
	double y_delta[OutN];            //输出层的delta
	
	double error;                    //全局误差
	double errlimit=0.001;            //预设的全局误差极限
	double alpha=0.1, beta=0.1;      //预设的学习速率
	
	int loop=0;                      //训练次数
	int times=50000;                    //预设的训练次数上限
	int i, j, m;
	double max, min;
	double sumtemp;
	double errtemp;
	
	struct{                           //训练样本对. 对XOR问题, 共有4对
		double input[InputN];
		double teach[OutN];
	}data[datanum] ={{0.7286989, 0.6227754, 2.0578557, 0.8839495,0},
		        {0.7832924, 0.5477852, 0.9352306, 0.9116902,0}, 
	            {0.6844877, 0.7475081, 2.1303267, 0.8635362,0},
				{0.7343029, 0.6601697, 2.0677268, 0.8800931, 0},
				{0.7590993, 0.5681108, 0.8638250, 0.9032526, 0},
				{0.7295788, 0.6334724, 2.1639459, 0.8811238, 0},
				{0.6639910, 0.8174171, 1.6788274, 0.8584345,1},
				{0.6122161, 1.0077206, 2.6496249, 0.8228670,1},
				{0.6917030, 0.7713253, 1.8655783, 0.8644863,1},
				{0.7190323, 0.7008590, 1.7684992, 0.8772450,1},
				{0.7047494, 0.7661451, 1.2735768, 0.8711355,1},
				{0.7473130, 0.6724366, 1.0652668, 0.8888800,1}};
	
	//-----------------------------------------------------------
	//初始化. 将连接权值、偏置系数置为(-1,1)之间的随机数
	ModelDlg->GetDlgItem(IDC_ModelResult)->GetWindowText(m_result1);
	m_result="Initializing..................\r\n";
	m_result+=m_result1;
	ModelDlg->GetDlgItem(IDC_ModelResult)->SetWindowText(m_result);
	for(i=0; i<InputN; i++)        //w的随机输入
		for(j=0; j<HN; j++){
			w[i][j] = ((double)rand()/32767.0)*2-1;
			deltaw[i][j] = 0;
		}

	for(i=0; i<HN; i++)   //v的随机输入
		for(j=0;j<OutN;j++){		
			v[i][j] = ((double)rand()/32767.0)*2-1;
			deltav[i][j] = 0;
		}
	//训练.
	ModelDlg->GetDlgItem(IDC_ModelResult)->GetWindowText(m_result1);
	m_result="Train beginning..................\r\n";
	m_result+=m_result1;
	ModelDlg->GetDlgItem(IDC_ModelResult)->SetWindowText(m_result);
	while(loop < times){
		loop++;
		error = 0.0;
			
		for(m=0; m<datanum; m++){                     //对datanum个样本进行增量训练,m为样本个数
			//输入正向传播
			max=0.0;
			min=0.0;
			for(i=0; i<InputN; i++){ //设置输入层的输出
				x_out[i] = data[m].input[i];
				if (max < x_out[i])   //比较得出输入的最大最小值
			    	max = x_out[i];
				
				if (min > x_out[i]) 
					min = x_out[i];
			}

			for(i=0; i<InputN; i++){   //归一化后的输入值
				x_out[i] = (x_out[i] - min) / (max-min);
			}

			for(i=0; i<OutN; i++){     //输出层的期望输出
				y[i] = data[m].teach[i];
			}			
				
			for(i=0; i<HN; i++){       //计算隐含层的输出
				sumtemp = 0.0;
				for(j=0; j<InputN; j++)
					sumtemp += w[j][i] * x_out[j];
				hn_out[i] = tanh(sumtemp);     //隐含层作用函数为y=tanh(x)
			}
				
			for(i=0; i<OutN; i++){             //计算输出层的输出
				sumtemp = 0.0;
				for(j=0; j<HN; j++) sumtemp += v[j][i] * hn_out[j];
				y_out[i] = g(sumtemp);            //输出层作用函数为sigmod函数
			}

				
			//误差反向传播
			for(i=0; i<OutN; i++){
				errtemp = y[i] - y_out[i]; 
				y_delta[i] = errtemp * g(y_out[i]) * (1.0 - g(y_out[i]));
				error += (errtemp * errtemp);  //所有样本的全局误差
			}
				
			for(i=0; i<HN; i++){               //计算隐含层的delta
				errtemp = 0.0;
				for(j=0; j<OutN; j++)	errtemp += y_delta[j] * v[i][j];
				hn_delta[i] = errtemp * (1.0 + hn_out[i]) * (1.0 - hn_out[i]);
			}
				
			for(i=0; i<OutN; i++){       //调整输出层的权值和偏置系数
				for(j=0; j<HN; j++){
					deltav[j][i] = alpha * deltav[j][i] + beta * y_delta[i] * hn_out[j];
					v[j][i] += deltav[j][i];
				}
			}
				
				
			for(i=0; i<HN; i++){        //调整隐含层的权值和偏置系数
				for(j=0; j<InputN; j++){
					deltaw[j][i] = alpha * deltaw[j][i] + beta * hn_delta[i] * x_out[j];
					w[j][i] += deltaw[j][i];
				}
			}
		}
				
		//全局误差判断

		error = error/2;

		if(loop%1000==0){
			ModelDlg->GetDlgItem(IDC_ModelResult)->GetWindowText(m_result1);
			m_result="全局误差Error=";
			sprintf( buffer, "%f", error);
			m_result+=buffer;
			m_result+="\r\n";
			m_result+=m_result1;
			ModelDlg->GetDlgItem(IDC_ModelResult)->SetWindowText(m_result);
		}
		if(error < errlimit)	break;
	}
		
    //----------------------------------------------------------------
	//输出训练结果
	ModelDlg->GetDlgItem(IDC_ModelResult)->GetWindowText(m_result1);
	m_result="HN=";
	sprintf( buffer, "%d", HN);
	m_result+=buffer;
	m_result+="\r\n";
	m_result+="Error=";
	sprintf( buffer, "%f", error);
	m_result+=buffer;
	m_result+="\r\n";
	m_result+="Loop=";
	sprintf( buffer, "%d", loop);
	m_result+=buffer;
	m_result+="\r\n";
	for(i=0; i<InputN; i++)
		for(j=0; j<HN; j++){
			sprintf(buffer, "w[%d][%d]=%f", i+1,j+1,w[i][j]);
		    m_result+=buffer;
			m_result+="\r\n";
	}
	for(i=0; i<HN; i++)
		for(j=0; j<OutN; j++){
			sprintf(buffer, "v[%d][%d]=%f", i+1,j+1,v[i][j]);
		    m_result+=buffer;
			m_result+="\r\n";
	}
	m_result+="training completion..................\r\n";
	m_result+=m_result1;
	ModelDlg->GetDlgItem(IDC_ModelResult)->SetWindowText(m_result);	

	//------------------------------------------------------------------
	//利用上面训练得到的权值和偏置系数进行检验
	ModelDlg->GetDlgItem(IDC_ModelResult)->GetWindowText(m_result1);
	m_result="testing样本............\r\n";
	for(m=0; m<datanum; m++){                   //对datanum个样本分别检验其实际输出
		for(i=0; i<InputN; i++){                //设置输入层的输出
			x_out[i] = data[m].input[i];
			sprintf(buffer, "data[%d].x[%d]=%f", m+1,i+1,data[m].input[i]);
		    m_result+=buffer;
			m_result+="\r\n";
		}
		for(i=0; i<HN; i++){                //计算隐含层的输出
			sumtemp = 0.0;
			for(j=0; j<InputN; j++)		sumtemp += w[j][i] * x_out[j];
			hn_out[i] = tanh(sumtemp);     //隐含层作用函数为y=tanh(x)
		}

		for(i=0; i<OutN; i++){     //计算输出层的输出
			sumtemp = 0.0;
			for(j=0; j<HN; j++)		sumtemp += v[j][i] * hn_out[j];
			y_out[i] = g(sumtemp);
			sprintf(buffer, "data[%d].y[%d]=%f", m+1,i+1,y_out[i]);
		    m_result+=buffer;
			m_result+="\r\n";
		}
	}
	m_result+=m_result1;
	ModelDlg->GetDlgItem(IDC_ModelResult)->SetWindowText(m_result);
	

	//------------------------------------------------------------------
	//利用上面训练得到的权值和偏置系数对新的数据进行检验
	ModelDlg->GetDlgItem(IDC_ModelResult)->GetWindowText(m_result1);
	m_result="testing输入............\r\n";

	struct{                           //检验数据
		double input[InputN];
	}newdata[4] ={{0.7720868, 0.5687513, 1.1274239, 0.9061907},
				{0.8410155, 0.3963381, 0.7977771, 0.9350835},
				{0.6617912, 0.8482799, 1.9548561, 0.8531878},
				{0.7257483, 0.6342603, 1.1469111, 0.8931182}};
	
	for(m=0; m<4; m++){               //对datanum个样本分别检验其实际输出
		for(i=0; i<InputN; i++){                 //设置输入层的输出
			x_out[i] = newdata[m].input[i];
			sprintf(buffer, "newdata[%d].x[%d]=%f", m+1,i+1,newdata[m].input[i]);
		    m_result+=buffer;
			m_result+="\r\n";
		}
		for(i=0; i<HN; i++){                //计算隐含层的输出
			sumtemp = 0.0;
			for(j=0; j<InputN; j++)		sumtemp += w[j][i] * x_out[j];
			hn_out[i] = tanh(sumtemp);     //隐含层作用函数为y=tanh(x)
		}
		
		for(i=0; i<OutN; i++){     //计算输出层的输出
			sumtemp = 0.0;
			for(j=0; j<HN; j++)		sumtemp += v[j][i] * hn_out[j];
			y_out[i] = g(sumtemp);
			sprintf(buffer, "newdata[%d].y[%d]=%f", m+1,i+1,y_out[i]);
		    m_result+=buffer;
			m_result+="\r\n";
		}
	}
	m_result+=m_result1;
	ModelDlg->GetDlgItem(IDC_ModelResult)->SetWindowText(m_result);
}
