// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// MSMoneyDemoView.cpp : implementation of the CMSMoneyDemoView class

#include "stdafx.h"
#include "MSMoneyDemo.h"
#include "MSMoneyDemoView.h"
#include "MSMVisualManager.h"
#include "MainFrm.h"

#include "imageTools\GrayTrans.h"// 灰度变换类
#include "imageTools\GeometryTrans.h"//几何变换类
#include "imageTools\ImageEnhance.h"//图像空域增强类
#include "imageTools\ImageFreqEnhance.h"//图像频域增强类
#include "imageTools\ImgSegment.h"//图像分割类
#include "imageTools\Morphology.h"//图像形态学类
#include "imageTools\WaveletTrans.h"//小波变换类
#include "imageTools\GeometryMoveDlg.h"//平移参数设置
#include "imageTools\HistogramDrawDlg.h"//直方图显示
#include "imageTools\HistSegmentDlg.h"//图像直方图分割对话框
#include "imageTools\LinearStepParamDlg.h"//分段线性拉伸参数设置
#include "imageTools\MedianSmoothDlg.h"//图像中值平均模板设置
#include "imageTools\NeiAverSmoothDlg.h"//图像邻域平均模版设置
#include "imageTools\StrechWindowDlg.h"//窗口变换参数设置	
#include "imageTools\ThreshStrechDlg.h"//阈值设置
#include "imageTools\Zoomdlg.h"//缩放参数设置
#include "imageTools\RotateDlg.h"//旋转角度设置

#include "imageTools\FaceDetect.h"
#include "imageTools\Register.h"
#include "imageTools\PlateTrans.h"
#include "imageTools\charsRecognition.h"
#include "imageTools\Structure.h"
#include "imageTools\Result.h"

#include "VideoTools\chafenMul.h"
#include "VideoTools\AVIHandle.h"	// Added by ClassView
#include "VideoTools\MeanShiftSegger.h"	// Added by ClassView

//全局变量
FourierTrans fftTrans;
FourierTrans fftTrans_Output;
WaveletTrans wavltTrans;
WaveletTrans wavltTrans_Output;

int left;
int right;
int top;
int bottom;



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoView

IMPLEMENT_DYNCREATE(CMSMoneyDemoView, CView)

BEGIN_MESSAGE_MAP(CMSMoneyDemoView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)


	// 按钮消息效应绑定
	// 几何变换
	ON_COMMAND(ID_LINKS_Move, OnMove)
	ON_COMMAND(ID_LINKS_HorizontalMirror, OnHorizontalMirror)
	ON_COMMAND(ID_LINKS_verticalMirror, OnVerticalMirror)
	ON_COMMAND(ID_LINKS_Zoom, OnZoom)
	ON_COMMAND(ID_LINKS_Clockwise90, OnClockwise90)
	ON_COMMAND(ID_LINKS_Anticlockwise90, OnAnticlockwise90)
	ON_COMMAND(ID_LINKS_Rotate180, OnRotate180)
	ON_COMMAND(ID_LINKS_FreeRotate, OnFreeRotate)
	// 灰度变换
	ON_COMMAND(ID_Binary, OnBinary)
	ON_COMMAND(ID_HistogramDraw, OnHistogramDraw)
	ON_COMMAND(ID_HistgramAver, OnHistgramAver)
	ON_COMMAND(ID_ImageReverse, OnImageReverse)
	ON_COMMAND(ID_ImgThresh, OnImgThresh)
	ON_COMMAND(ID_ThresholdWindow, OnThresholdWindow)
	ON_COMMAND(ID_LinearStrecth, OnLinearStrecth)
	// 图像空域增强
	ON_COMMAND(ID_LINKS_GaussNoise, OnGaussNoise)
	ON_COMMAND(ID_LINKS_PeppersaltNoise, OnPepperSaltNoise)
	ON_COMMAND(ID_LINKS_NeiAveSmooth, OnNeiAveSmooth)
	ON_COMMAND(ID_LINKS_MedianSmooth, OnMedianSmooth)
	ON_COMMAND(ID_LINKS_ChooseMaskSmooth, OnChooseMaskSmooth)
	ON_COMMAND(ID_LINKS_GradeSharp, OnGradeSharp)
	ON_COMMAND(ID_LINKS_LaplaceSharp, OnLaplaceSharp)
	// 图像频域增强
	ON_COMMAND(ID_LINKS_QuickFFT, OnQuickFFt)
	ON_COMMAND(ID_LINKS_QuickFFT_Reverse, OnQuickFFtReverse)
	ON_COMMAND(ID_LINKS_IdealLowPass, OnIdealLowPass)
	ON_COMMAND(ID_LINKS_IdealHighPass, OnIdealHighPass)
	ON_COMMAND(ID_LINKS_ButterLowPass, OnButterLowPass)
	ON_COMMAND(ID_LINKS_ButterHighPass, OnButterHighPass)
	ON_COMMAND(ID_LINKS_HarrWaveletTrans, OnHarrWaveletTrans)
	ON_COMMAND(ID_LINKS_HarrWaveletTrans_Reverse, OnHarrWavRevTrans)
	// 图像形态学
	ON_COMMAND(ID_LINKS_Erosion, OnErosion)
	ON_COMMAND(ID_LINKS_Dilation, OnDilation)
	ON_COMMAND(ID_LINKS_Open, OnOpen)
	ON_COMMAND(ID_LINKS_Close, OnClose)
	ON_COMMAND(ID_LINKS_Thinning, OnThinning)
	// 图像分割
	ON_COMMAND(ID_LINKS_HistThreshSeg, OnHistThreshSeg)
	ON_COMMAND(ID_LINKS_AdaptiveThreshold, OnAdaptThresholdSeg)
	ON_COMMAND(ID_LINKS_Robert, OnRobert)
	ON_COMMAND(ID_LINKS_Sobel, OnSobel)
	ON_COMMAND(ID_LINKS_Prewitt, OnPrewitt)
	ON_COMMAND(ID_LINKS_Laplacian, OnLaplacian)
	ON_COMMAND(ID_LINKS_EdgeTrace, OnEdgeTrace)
	ON_COMMAND(ID_LINKS_RegionGrow, OnRegionGrow)
	// 人脸检测
	ON_COMMAND(id_similardegree, OnSimilardegree)
	ON_COMMAND(id_boundary, Onboundary)
	ON_COMMAND(id_facelocate, Onfacelocate)
	ON_COMMAND(id_innerboundary, Oninnerboundary)
	ON_COMMAND(id_eyelocate, Oneyelocate)
	ON_COMMAND(id_noselocate, Onnoselocate)
	ON_COMMAND(id_mouthlocate, Onmouthlocate)

	// 车号识别
	ON_COMMAND(id_ColorPair, OnColorPair)
	ON_COMMAND(id_ColorToGray, OnColorToGray)
	ON_COMMAND(ID_Binary2, OnBinary2)
	ON_COMMAND(id_platelocate, Onplatelocate)
	ON_COMMAND(id_platerelocate, Onplaterelocate)
	ON_COMMAND(id_charseparate, Oncharseparate)
	ON_COMMAND(id_charrecognition, Oncharrecognition)


	// 印刷体汉字识别
	ON_COMMAND(id_PaperBinary, OnPaperBinary)
	ON_COMMAND(id_location, Onlocation)
	ON_COMMAND(id_recognition, Onrecognition)

	// 运动图像分析
	ON_COMMAND(id_ObjectDetect, OnObjectDetect)
	ON_COMMAND(id_ObjectTrace, OnObjectTrace)

	ON_WM_PAINT()

	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoView construction/destruction

CMSMoneyDemoView::CMSMoneyDemoView()
{
	// TODO: add construction code here
	state2 = 0;
}

CMSMoneyDemoView::~CMSMoneyDemoView()
{
	m_sizeImage = CSize (0, 0);
}

BOOL CMSMoneyDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~WS_BORDER;
	cs.lpszName = "我的标题";
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoView drawing
int CMSMoneyDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_Image.LoadBitmap(IDB_MSM_VIEW_BKGND);

	BITMAP bitmap;
	m_Image.GetBitmap(&bitmap);
	m_sizeImage = CSize(bitmap.bmWidth, bitmap.bmHeight);
	return 0;
}

void CMSMoneyDemoView::OnDraw(CDC* pDCView)
{

	CMSMoneyDemoDoc* pDoc = GetDocument();								// 获取文档类指针
	ASSERT(pDoc != NULL);
	if (state2 == 1)
	{
		ImageDib* pDib = pDoc->m_dib_SoleProc;											// 返回m_dib的指针
		CSize sizeFileDib = pDib->GetDimensions();										// 获取DIB的尺寸
		pDib->Draw(pDCView,CPoint(0, 0), sizeFileDib);						    //显示DIB
	}

	if (state2 == 2) // 启动视频帧数绘制
	{
		if (pDoc->isAvi) {
			int frameWidth = pDoc->m_avi->GetFrameWidth();
			int frameHeight = pDoc->m_avi->GetFrameHeight();
			BITMAPINFO bitmapInfo;
			bitmapInfo.bmiHeader.biHeight = (LONG)frameHeight;
			bitmapInfo.bmiHeader.biWidth = (LONG)frameWidth;
			bitmapInfo.bmiHeader.biSizeImage = (LONG)frameWidth * (LONG)frameHeight * 3;
			bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bitmapInfo.bmiHeader.biBitCount = 24;
			bitmapInfo.bmiHeader.biCompression = 0;
			bitmapInfo.bmiHeader.biPlanes = 1;

			pDCView->SetStretchBltMode(STRETCH_DELETESCANS);
			StretchDIBits(pDCView->m_hDC, 0, 0, frameWidth, frameHeight, 0, 0, frameWidth, frameHeight,
				pDoc->Avidata, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}

	}

	//CMemDC memDC (*pDCView, this);
	//CDC* pDC = &memDC.GetDC ();
	//CRect rectClient;
	//GetClientRect (rectClient);
	//pDC->FillSolidRect (rectClient, RGB (255, 255, 255));
	//pDC->DrawState (CPoint (20, 20), m_sizeImage, &m_Image, DSS_NORMAL);
}

void CMSMoneyDemoView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类

}

void CMSMoneyDemoView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CView::OnPaint()
	OnPrepareDC(&dc);
	OnDraw(&dc); //调用了OnDraw
}

BOOL CMSMoneyDemoView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CMSMoneyDemoView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类

		CRect rectClient;
		GetClientRect(rectClient);
		pDC->FillSolidRect(rectClient, RGB(255, 255, 255));
	
	CView::OnPrepareDC(pDC, pInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoView message handlers

// 几何变换
void CMSMoneyDemoView::OnMove()
{
	// TODO: 在此处添加消息处理程序代码
	CMSMoneyDemoDoc *pDoc = GetDocument();
	  ImageDib *pDib = pDoc->m_dib;
	  ImageDib *pDib_Res = pDoc->m_dib_Result;
	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}
	
	//平移参数设置对话框
	GeometryMoveDlg dlgPara;
	if (dlgPara.DoModal() == IDOK)
	{
		
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(), pDib->m_nBitCount, 
			pDib->m_lpColorTable, pDib->m_pImgData);							
		//将dlgPara.m_Xmove, dlgPara.m_Ymove作为平移量，对图像平移
		geoTrans.Move(dlgPara.m_Xmove, dlgPara.m_Ymove);

		// 以上一次处理结果初始化几何变换对象
		GeometryTrans geoTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
			pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
		geoTrans_Output.Move(dlgPara.m_Xmove, dlgPara.m_Ymove);


		/*	*/	//新建视图窗口，多文档显示变换结果
		//CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		//pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		//CMSMoneyDemoView* pView = (CMSMoneyDemoView*)pFrame->GetActiveView();
		//CMSMoneyDemoDoc* pDocNew = pView->GetDocument();
		//ImageDib *dibNew = pDocNew->m_dib;
		//dibNew->ReplaceDib(geoTrans.GetDimensions(), geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
		//pView->OnInitialUpdate();
		//pDocNew->SetModifiedFlag(TRUE);
		//pDocNew->UpdateAllViews(pView);

		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);
		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		// 更新单次处理输出
		dibNew->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(geoTrans_Output.GetDimensions(),
			geoTrans_Output.m_nBitCountOut, geoTrans_Output.m_lpColorTable, geoTrans_Output.m_pImgDataOut);

		// 强制刷新窗口(所有视图)
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->InvalidateRect(NULL, FALSE);
		pFrame->UpdateWindow();
	}

}

void CMSMoneyDemoView::OnHorizontalMirror()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独水平镜像处理
	geoTrans.MirrorHorTrans();

	// 以上一次处理结果初始化几何变换对象
	GeometryTrans geoTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加水平镜像处理
	geoTrans_Output.MirrorHorTrans();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),
		geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);
	
	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(geoTrans_Output.GetDimensions(),
		geoTrans_Output.m_nBitCountOut, geoTrans_Output.m_lpColorTable, geoTrans_Output.m_pImgDataOut);

	// 强制刷新所有视图
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnVerticalMirror()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独垂直镜像处理
	geoTrans.MirrorVerTrans();

	// 以上一次处理结果初始化几何变换对象
	GeometryTrans geoTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加垂直镜像处理
	geoTrans_Output.MirrorVerTrans();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),
		geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(geoTrans_Output.GetDimensions(),
		geoTrans_Output.m_nBitCountOut, geoTrans_Output.m_lpColorTable, geoTrans_Output.m_pImgDataOut);

	// 强制刷新所有视图
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); 
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnZoom()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//原图像尺寸
	CSize oldImgSize = pDib->GetDimensions();

	//缩放参数设置对话框
	ZoomDlg dlgPara;

	if (dlgPara.DoModal() == IDOK) {
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(), pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		//根据指定的缩放比例对对原图像进行缩放缩放
		geoTrans.Zoom(dlgPara.m_XstrechRatio, dlgPara.m_YstrechRatio);

		// 以上一次处理结果初始化几何变换对象
		GeometryTrans geoTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
			pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
		//叠加缩放比例处理
		geoTrans_Output.Zoom(dlgPara.m_XstrechRatio, dlgPara.m_YstrechRatio);

		//新建视图窗口，显示处理结果
		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);
		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(geoTrans_Output.GetDimensions(),
			geoTrans_Output.m_nBitCountOut, geoTrans_Output.m_lpColorTable, geoTrans_Output.m_pImgDataOut);

		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); 
		pFrame->InvalidateRect(NULL, FALSE); 
		pFrame->UpdateWindow();
	}
}

void CMSMoneyDemoView::OnClockwise90()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独顺时针90度处理
	geoTrans.Clockwise90();

	// 以上一次处理结果初始化几何变换对象
	GeometryTrans geoTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加顺时针90度处理
	geoTrans_Output.Clockwise90();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),
		geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(geoTrans_Output.GetDimensions(),
		geoTrans_Output.m_nBitCountOut, geoTrans_Output.m_lpColorTable, geoTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnAnticlockwise90()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独逆时针90度处理
	geoTrans.Anticlockwise90();

	// 以上一次处理结果初始化几何变换对象
	GeometryTrans geoTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加逆时针90度处理
	geoTrans_Output.Anticlockwise90();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),
		geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(geoTrans_Output.GetDimensions(),
		geoTrans_Output.m_nBitCountOut, geoTrans_Output.m_lpColorTable, geoTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnRotate180()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
	GeometryTrans geoTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独旋转180度处理
	geoTrans.Rotate180();

	// 以上一次处理结果初始化几何变换对象
	GeometryTrans geoTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加旋转180度处理
	geoTrans_Output.Rotate180();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(geoTrans.GetDimensions(),
		geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(geoTrans_Output.GetDimensions(),
		geoTrans_Output.m_nBitCountOut, geoTrans_Output.m_lpColorTable, geoTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnFreeRotate()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}
	//旋转参数设置对话框
	RotateDlg dlgPara;
	if (dlgPara.DoModal() == IDOK) {
		//定义GeometryTrans类的对象geoTrans，用当前DIB对其初始化
		GeometryTrans geoTrans(pDib->GetDimensions(), pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		//调用Rotate函数进行图像旋转，将角度和插值方式参数传进函数
		geoTrans.Rotate(dlgPara.m_RotateAngle);

		// 以上一次处理结果初始化几何变换对象
		GeometryTrans geoTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
			pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
		//叠加Rotate函数处理
		geoTrans_Output.Rotate(dlgPara.m_RotateAngle);

		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);

		pDib->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

		pDoc->m_dib_Temp->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		dibNew->ReplaceDib(geoTrans.GetDimensions(),
			geoTrans.m_nBitCountOut, geoTrans.m_lpColorTable, geoTrans.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(geoTrans_Output.GetDimensions(),
			geoTrans_Output.m_nBitCountOut, geoTrans_Output.m_lpColorTable, geoTrans_Output.m_pImgDataOut);

		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->InvalidateRect(NULL, FALSE);
		pFrame->UpdateWindow();
	}
}

// 灰度变换
void CMSMoneyDemoView::OnBinary()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//异常判断
	if (pDib->m_nBitCount != 24 && pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理彩色和灰度图像", MB_OK, 0);
		return;
	}

	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.BinaryImage();

	// 以上一次处理结果初始化几何变换对象
	GrayTrans graytrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加二值化处理
	graytrans_Output.BinaryImage();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(graytrans.GetDimensions(),
		graytrans.m_nBitCountOut, graytrans.m_lpColorTable, graytrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(graytrans_Output.GetDimensions(),
		graytrans_Output.m_nBitCountOut, graytrans_Output.m_lpColorTable, graytrans_Output.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnHistogramDraw()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//异常判断
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//直方图绘制对话框
	HistogramDrawDlg dlgHist1;
	dlgHist1.m_himageDib.ReplaceDib(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	HistogramDrawDlg dlgHist2;
	dlgHist2.m_himageDib.ReplaceDib(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//显示直方图
	dlgHist1.DoModal();
	dlgHist2.DoModal();
}

void CMSMoneyDemoView::OnHistgramAver()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	HistogramDrawDlg histodrawdlg1;
	histodrawdlg1.m_himageDib.ReplaceDib(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独直方图均衡处理
	histodrawdlg1.histogramAve();

	// 以上一次处理结果初始化几何变换对象
	HistogramDrawDlg histodrawdlg2;
	histodrawdlg2.m_himageDib.ReplaceDib(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加直方图均衡处理
	histodrawdlg2.histogramAve();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(histodrawdlg1.GetDimensions(), histodrawdlg1.m_nBitCountOut, 
		histodrawdlg1.m_lpColorTableOut, histodrawdlg1.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(histodrawdlg2.GetDimensions(),
		histodrawdlg2.m_nBitCountOut, histodrawdlg2.m_lpColorTableOut, histodrawdlg2.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnImageReverse()
{
	// TODO: Add your command handler code here
	//获取文档类指针
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//DIB类型判断
	if (pDib->m_nBitCount != 24 && pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理彩色和灰度图像", MB_OK, 0);
		return;
	}

	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(),
		pDib->m_nBitCount, pDib->m_lpColorTable, pDib->m_pImgData);
	//单独调用ReverseImg()对图像求反处理
	graytrans.RevImage();

	// 以上一次处理结果初始化几何变换对象
	GrayTrans graytrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加调用ReverseImg()对图像求反处理
	graytrans_Output.RevImage();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(graytrans.GetDimensions(),
		graytrans.m_nBitCountOut, graytrans.m_lpColorTable, graytrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(graytrans_Output.GetDimensions(),
		graytrans_Output.m_nBitCountOut, graytrans_Output.m_lpColorTable, graytrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnImgThresh()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//阈值分割对话框类对象threshsetdlg
	ThreshStrechDlg threshsetdlg;
	if (threshsetdlg.DoModal() == IDOK)
	{
		//定义灰度变换类的对象graytrans，并将当前DIB数据作为输入数据传入graytrans中
		GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);

		// 以上一次处理结果初始化几何变换对象
		GrayTrans graytrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
			pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);


		//根据用户选择的阈值，调用二值化函数分割图像
		if ((threshsetdlg.m_Thresh >= 0) && (threshsetdlg.m_Thresh <= 255)) {
			graytrans.BinaryImage(threshsetdlg.m_Thresh);
			//叠加调用二值化函数分割图像处理
			graytrans_Output.BinaryImage(threshsetdlg.m_Thresh);
		}
		else
		{
			AfxMessageBox("请输入一个0-255之间的阈值！");
			return;
		}

		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);
		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		dibNew->ReplaceDib(graytrans.GetDimensions(),
			graytrans.m_nBitCountOut, graytrans.m_lpColorTable, graytrans.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(graytrans_Output.GetDimensions(),
			graytrans_Output.m_nBitCountOut, graytrans_Output.m_lpColorTable, graytrans_Output.m_pImgDataOut);

		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
	}
}

void CMSMoneyDemoView::OnThresholdWindow()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//阈值分割对话框类对象threshsetdlg
	StrechWindowDlg  strechwinsetdlg;
	if (strechwinsetdlg.DoModal() == IDOK)
	{
		//定义灰度变换类的对象graytrans，并将当前DIB数据作为输入数据传入graytrans中
		GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		graytrans.ThresholdWindow(strechwinsetdlg.m_nBottom, strechwinsetdlg.m_nTop);

		// 以上一次处理结果初始化几何变换对象
		GrayTrans graytrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
			pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
		//叠加阈值分割处理
		graytrans_Output.ThresholdWindow(strechwinsetdlg.m_nBottom, strechwinsetdlg.m_nTop);

		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);
		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		dibNew->ReplaceDib(graytrans.GetDimensions(),
			graytrans.m_nBitCountOut, graytrans.m_lpColorTable, graytrans.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(graytrans_Output.GetDimensions(),
			graytrans_Output.m_nBitCountOut, graytrans_Output.m_lpColorTable, graytrans_Output.m_pImgDataOut);

		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
	}
}

void CMSMoneyDemoView::OnLinearStrecth()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//异常判断
	if (pDib->m_nBitCount != 24 && pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理彩色和灰度图像", MB_OK, 0);
		return;
	}

	//线性拉伸参数设置对话框
	LinearStepParamDlg linesdlgPara;
	if (linesdlgPara.DoModal() == IDOK) {
		//获取用户设置的两个拉伸转折点
		CPoint point1, point2;
		point1.x = linesdlgPara.m_nX1;
		point1.y = linesdlgPara.m_nY1;
		point2.x = linesdlgPara.m_nX2;
		point2.y = linesdlgPara.m_nY2;

		//判断参数的合理性
		if (point1.x<0 || point1.x>255 || point2.x<0 || point2.x>255
			|| point1.y<0 || point1.y>255 || point2.y<0 || point2.y>255
			|| point1.x>point2.x) {
			::MessageBox(0, "参数不合理", MB_OK, 0);
			return;
		}

		//定义灰度变换的类对象
		GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
			pDib->m_lpColorTable, pDib->m_pImgData);
		//单独线性拉伸处理
		graytrans.LinearStrech(point1, point2);

		// 以上一次处理结果初始化几何变换对象
		GrayTrans graytrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
			pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
		//叠加线性拉伸处理
		graytrans_Output.LinearStrech(point1, point2);

		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);
		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		dibNew->ReplaceDib(graytrans.GetDimensions(),
			graytrans.m_nBitCountOut, graytrans.m_lpColorTable, graytrans.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(graytrans_Output.GetDimensions(),
			graytrans_Output.m_nBitCountOut, graytrans_Output.m_lpColorTable, graytrans_Output.m_pImgDataOut);

		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
	}
}

void CMSMoneyDemoView::OnPepperSaltNoise()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}
	// 单独处理
	ImageEnhance imgnoise(pDib->GetDimensions(), pDib->m_nBitCount, pDib->m_lpColorTable, pDib->m_pImgData);
	// 单独椒盐噪声处理
	imgnoise.AddPepperSaltNoise();

	// 以上一次处理结果初始化几何变换对象
	ImageEnhance imgnoise_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加椒盐噪声处理
	imgnoise_Output.AddPepperSaltNoise();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgnoise.GetDimensions(), imgnoise.m_nBitCountOut, imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgnoise_Output.GetDimensions(),
		imgnoise_Output.m_nBitCountOut, imgnoise_Output.m_lpColorTable, imgnoise_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
	

}

// 图像空域增强
void CMSMoneyDemoView::OnGaussNoise()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	ImageEnhance imgnoise(pDib->GetDimensions(), pDib->m_nBitCount, pDib->m_lpColorTable, pDib->m_pImgData);
	// 单独高斯噪声处理
	imgnoise.AddGaussNoise();

	// 以上一次处理结果初始化几何变换对象
	ImageEnhance imgnoise_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加高斯噪声处理
	imgnoise_Output.AddGaussNoise();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgnoise.GetDimensions(), imgnoise.m_nBitCountOut, imgnoise.m_lpColorTable, imgnoise.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgnoise_Output.GetDimensions(),
		imgnoise_Output.m_nBitCountOut, imgnoise_Output.m_lpColorTable, imgnoise_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnChooseMaskSmooth()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	ImageEnhance imgSmooth(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	// 单独掩模平滑处理
	imgSmooth.ChooseMaskSmooth();

	// 以上一次处理结果初始化几何变换对象
	ImageEnhance imgSmooth_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加掩模平滑处理
	imgSmooth_Output.ChooseMaskSmooth();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgSmooth.GetDimensions(), imgSmooth.m_nBitCountOut,
		imgSmooth.m_lpColorTable, imgSmooth.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgSmooth_Output.GetDimensions(),
		imgSmooth_Output.m_nBitCountOut, imgSmooth_Output.m_lpColorTable, imgSmooth_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnGradeSharp()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	int nThresh = 30;
	ImageEnhance imgsharp(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	// 单独梯度锐化处理
	imgsharp.GradeSharp(nThresh);

	// 以上一次处理结果初始化几何变换对象
	ImageEnhance imgsharp_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加梯度锐化处理
	imgsharp_Output.GradeSharp(nThresh);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgsharp.GetDimensions(), imgsharp.m_nBitCountOut, imgsharp.m_lpColorTable, imgsharp.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgsharp_Output.GetDimensions(),
		imgsharp_Output.m_nBitCountOut, imgsharp_Output.m_lpColorTable, imgsharp_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnNeiAveSmooth()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}
	ImageEnhance imgsmooth(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	// 以上一次处理结果初始化几何变换对象
	ImageEnhance imgSmooth_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	NeiAverSmoothDlg dlgparam;
	if (dlgparam.DoModal() == IDOK) {

		if (dlgparam.m_radiovalue == 0 || dlgparam.m_radiovalue == 1 || dlgparam.m_radiovalue == 2) {

			float Temp1[81];
			int i;
			for (i = 0; i < 81; i++)
			{
				Temp1[i] = 1.0;
			}

			imgsmooth.NeiAveTemplate(dlgparam.TempH, dlgparam.TempW, dlgparam.TempCX,
				dlgparam.TempCY, Temp1, dlgparam.fCoef);
			//叠加处理
			imgSmooth_Output.NeiAveTemplate(dlgparam.TempH, dlgparam.TempW, dlgparam.TempCX,
				dlgparam.TempCY, Temp1, dlgparam.fCoef);
		}
		else if (dlgparam.m_radiovalue = 3) {
			float Temp[25];
			//设模板为平均模板
			Temp[0] = 0.0;
			Temp[1] = 1.0;
			Temp[2] = 2.0;
			Temp[3] = 1.0;
			Temp[4] = 0.0;

			Temp[5] = 1.0;
			Temp[6] = 2.0;
			Temp[7] = 4.0;
			Temp[8] = 2.0;
			Temp[9] = 1.0;

			Temp[10] = 2.0;
			Temp[11] = 4.0;
			Temp[12] = 8.0;
			Temp[13] = 4.0;
			Temp[14] = 2.0;

			Temp[15] = 1.0;
			Temp[16] = 2.0;
			Temp[17] = 4.0;
			Temp[18] = 2.0;
			Temp[19] = 1.0;

			Temp[20] = 0.0;
			Temp[21] = 1.0;
			Temp[22] = 2.0;
			Temp[23] = 1.0;
			Temp[24] = 0.0;
			imgsmooth.NeiAveTemplate(dlgparam.TempH, dlgparam.TempW, dlgparam.TempCX,
				dlgparam.TempCY, Temp, dlgparam.fCoef);

			//叠加处理
			imgSmooth_Output.NeiAveTemplate(dlgparam.TempH, dlgparam.TempW, dlgparam.TempCX,
				dlgparam.TempCY, Temp, dlgparam.fCoef);
		}
		else
			return;

		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);
		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		dibNew->ReplaceDib(imgsmooth.GetDimensions(), imgsmooth.m_nBitCountOut, 
			imgsmooth.m_lpColorTable, imgsmooth.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(imgSmooth_Output.GetDimensions(),
			imgSmooth_Output.m_nBitCountOut, imgSmooth_Output.m_lpColorTable, imgSmooth_Output.m_pImgDataOut);

		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
	}
}

void CMSMoneyDemoView::OnMedianSmooth()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	ImageEnhance imgsmooth(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	// 以上一次处理结果初始化几何变换对象
	ImageEnhance imgSmooth_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	MedianSmoothDlg dlgparam;
	if (dlgparam.DoModal() == IDOK) {
		// 单独均值平滑处理
		imgsmooth.MedianSmooth(dlgparam.TempH, dlgparam.TempW, dlgparam.TempCX, dlgparam.TempCY);
		// 叠加均值平滑处理
		imgSmooth_Output.MedianSmooth(dlgparam.TempH, dlgparam.TempW, dlgparam.TempCX, dlgparam.TempCY);

		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);
		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		dibNew->ReplaceDib(imgsmooth.GetDimensions(), imgsmooth.m_nBitCountOut,
			imgsmooth.m_lpColorTable, imgsmooth.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(imgSmooth_Output.GetDimensions(),
			imgSmooth_Output.m_nBitCountOut, imgSmooth_Output.m_lpColorTable, imgSmooth_Output.m_pImgDataOut);

		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
	}
}

void CMSMoneyDemoView::OnLaplaceSharp()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	ImageEnhance imgsharp(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	// 以上一次处理结果初始化几何变换对象
	ImageEnhance imgsharp_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	int TempH, TempW, TempCX, TempCY;
	float fCoef;
	float Temp[9];
	//设模板为平均模板
	Temp[0] = -1.0;
	Temp[1] = -1.0;
	Temp[2] = -1.0;
	Temp[3] = -1.0;
	Temp[4] = 9.0;
	Temp[5] = -1.0;
	Temp[6] = -1.0;
	Temp[7] = -1.0;
	Temp[8] = -1.0;
	//初始化对话框变量
	TempH = 3;
	TempW = 3;
	TempCX = 1;
	TempCY = 1;
	fCoef = 1.0;

	// 单独拉普拉斯锐处理
	imgsharp.LapTemplate(TempH, TempW, TempCX, TempCY, Temp, fCoef);
	// 叠加拉普拉斯锐处理
	imgsharp_Output.LapTemplate(TempH, TempW, TempCX, TempCY, Temp, fCoef);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgsharp.GetDimensions(), imgsharp.m_nBitCountOut, imgsharp.m_lpColorTable, imgsharp.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgsharp_Output.GetDimensions(),
		imgsharp_Output.m_nBitCountOut, imgsharp_Output.m_lpColorTable, imgsharp_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

// 图像频域增强
void CMSMoneyDemoView::OnQuickFFt()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag = 1;
	int width = pDib->GetDimensions().cx;
	int height = pDib->GetDimensions().cy;
	int i = 1;
	while (i<width)
		i = i * 2;
	if (i != width)//图像宽不是2的幂次方
		isValidateFlag = 0;

	i = 1;
	while (i<height)
		i = i * 2;
	if (i != height)//图像高不是2的幂次方
		isValidateFlag = 0;

	if (isValidateFlag == 0) {
		::MessageBox(0, "输入图像的宽和高应为2的幂次方", MB_OK, 0);
		return;
	}

	//输入图像数据给transform_FFT类对象FFTTrans
	fftTrans.InputImgData(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独快速傅立叶变换处理
	fftTrans.Fourier();

	//输入图像数据给transform_FFT类对象FFTTrans
	fftTrans_Output.InputImgData(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加快速傅立叶变换处理
	fftTrans_Output.Fourier();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(fftTrans.GetDimensions(), fftTrans.m_nBitCountOut,
		fftTrans.m_lpColorTable, fftTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(fftTrans_Output.GetDimensions(),
		fftTrans_Output.m_nBitCountOut, fftTrans_Output.m_lpColorTable, fftTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnQuickFFtReverse()
{
	// TODO: Add your command handler code here
	fftTrans.RevFourier();
	fftTrans_Output.RevFourier();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(fftTrans.GetDimensions(), fftTrans.m_nBitCountOut,
		fftTrans.m_lpColorTable, fftTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDoc->m_dib_Result->ReplaceDib(fftTrans_Output.GetDimensions(),
		fftTrans_Output.m_nBitCountOut, fftTrans_Output.m_lpColorTable, fftTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnIdealLowPass()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag = 1;
	int width = pDib->GetDimensions().cx;
	int height = pDib->GetDimensions().cy;
	int i = 1;
	while (i<width)
		i = i * 2;
	if (i != width)//图像宽不是2的幂次方
		isValidateFlag = 0;

	i = 1;
	while (i<height)
		i = i * 2;
	if (i != height)//图像高不是2的幂次方
		isValidateFlag = 0;

	if (isValidateFlag == 0) {
		::MessageBox(0, "输入图像的宽和高应为2的幂次方", MB_OK, 0);
		return;
	}

	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	// 单独图像理想低通滤波处理
	imgfreq.IdealLowPassFilter(pDib->m_imgWidth, pDib->m_imgHeight, pDib->m_imgHeight / 16);

	// 以上一次处理结果初始化几何变换对象
	ImageFreqEnhance imgfreq_Output;
	imgfreq_Output.InputImageData(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	// 叠加图像理想低通滤波处理
	imgfreq_Output.IdealLowPassFilter(pDib_Res->m_imgWidth, pDib_Res->m_imgHeight, pDib_Res->m_imgHeight / 16);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgfreq.GetDimensions(), imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgfreq_Output.GetDimensions(),
		imgfreq_Output.m_nBitCountOut, imgfreq_Output.m_lpColorTable, imgfreq_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnIdealHighPass()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag = 1;
	int width = pDib->GetDimensions().cx;
	int height = pDib->GetDimensions().cy;
	int i = 1;
	while (i < width)
		i = i * 2;
	if (i != width)//图像宽不是2的幂次方
		isValidateFlag = 0;

	i = 1;
	while (i < height)
		i = i * 2;
	if (i != height)//图像高不是2的幂次方
		isValidateFlag = 0;

	if (isValidateFlag == 0) {
		::MessageBox(0, "输入图像的宽和高应为2的幂次方", MB_OK, 0);
		return;
	}

	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独图像理想高通滤波处理
	imgfreq.IdealHighPassFilter(pDib->m_imgWidth, pDib->m_imgHeight, pDib->m_imgHeight / 16);

	// 以上一次处理结果初始化几何变换对象
	ImageFreqEnhance imgfreq_Output;
	imgfreq_Output.InputImageData(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	// 叠加图像理想高通滤波处理
	imgfreq_Output.IdealHighPassFilter(pDib_Res->m_imgWidth, pDib_Res->m_imgHeight, pDib_Res->m_imgHeight / 16);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgfreq.GetDimensions(), imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgfreq_Output.GetDimensions(),
		imgfreq_Output.m_nBitCountOut, imgfreq_Output.m_lpColorTable, imgfreq_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnButterLowPass()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag = 1;
	int width = pDib->GetDimensions().cx;
	int height = pDib->GetDimensions().cy;
	int i = 1;
	while (i<width)
		i = i * 2;
	if (i != width)//图像宽不是2的幂次方
		isValidateFlag = 0;

	i = 1;
	while (i<height)
		i = i * 2;
	if (i != height)//图像高不是2的幂次方
		isValidateFlag = 0;

	if (isValidateFlag == 0) {
		::MessageBox(0, "输入图像的宽和高应为2的幂次方", MB_OK, 0);
		return;
	}

	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//单独图像巴特低通滤波处理
	imgfreq.ButterLowPassFilter(pDib->m_imgWidth, pDib->m_imgHeight, pDib->m_imgHeight / 16);

	// 以上一次处理结果初始化几何变换对象
	ImageFreqEnhance imgfreq_Output;
	imgfreq_Output.InputImageData(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	// 叠加图像巴特低通滤波处理
	imgfreq_Output.ButterLowPassFilter(pDib_Res->m_imgWidth, pDib_Res->m_imgHeight, pDib_Res->m_imgHeight / 16);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgfreq.GetDimensions(), imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgfreq_Output.GetDimensions(),
		imgfreq_Output.m_nBitCountOut, imgfreq_Output.m_lpColorTable, imgfreq_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnButterHighPass()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断图像大小是否为2的幂次的方阵
	int isValidateFlag = 1;
	int width = pDib->GetDimensions().cx;
	int height = pDib->GetDimensions().cy;
	int i = 1;
	while (i<width)
		i = i * 2;
	if (i != width)//图像宽不是2的幂次方
		isValidateFlag = 0;

	i = 1;
	while (i<height)
		i = i * 2;
	if (i != height)//图像高不是2的幂次方
		isValidateFlag = 0;

	if (isValidateFlag == 0) {
		::MessageBox(0, "输入图像的宽和高应为2的幂次方", MB_OK, 0);
		return;
	}

	//输入图像数据给ImageFreqEnhance类对象imgfreq;
	ImageFreqEnhance imgfreq;
	imgfreq.InputImageData(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	// 单独图像巴特低通滤波处理
	imgfreq.ButterHighPassFilter(pDib->m_imgWidth, pDib->m_imgHeight, pDib->m_imgHeight / 16);

	// 以上一次处理结果初始化几何变换对象
	ImageFreqEnhance imgfreq_Output;
	imgfreq_Output.InputImageData(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	// 叠加图像巴特低通滤波处理
	imgfreq_Output.ButterHighPassFilter(pDib_Res->m_imgWidth, pDib_Res->m_imgHeight, pDib_Res->m_imgHeight / 16);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(imgfreq.GetDimensions(), imgfreq.m_nBitCountOut,
		imgfreq.m_lpColorTable, imgfreq.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(imgfreq_Output.GetDimensions(),
		imgfreq_Output.m_nBitCountOut, imgfreq_Output.m_lpColorTable, imgfreq_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnHarrWaveletTrans()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	wavltTrans.InputImageData(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	wavltTrans.BasicWaveletTrans(2);

	// 叠加处理
	wavltTrans_Output.InputImageData(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	wavltTrans_Output.BasicWaveletTrans(2);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(wavltTrans.GetDimensions(), wavltTrans.m_nBitCountOut, 
		wavltTrans.m_pColorTableOut, wavltTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(wavltTrans_Output.GetDimensions(),
		wavltTrans_Output.m_nBitCountOut, wavltTrans_Output.m_lpColorTable, wavltTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnHarrWavRevTrans()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	wavltTrans.BasicWaveletReverse();
	wavltTrans_Output.BasicWaveletReverse();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(wavltTrans.GetDimensions(), wavltTrans.m_nBitCountOut,
		wavltTrans.m_pColorTableOut, wavltTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(wavltTrans_Output.GetDimensions(),
		wavltTrans_Output.m_nBitCountOut, wavltTrans_Output.m_lpColorTable, wavltTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

// 图像形态学
void CMSMoneyDemoView::OnErosion()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断是否二值图像
	int i, j;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i<size.cy; i++) {
		for (j = 0; j<size.cx; j++) {
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if (*(pDib->m_pImgData + i * lineByte + j) != 0
				&& *(pDib->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//非二值图像
	if (!flag) {
		::MessageBox(0, "需要输入二值图像", MB_OK, 0);
		return;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	Morphology morTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//定义结构元素模板，默认为3*3模板
	/*
	int i;
	for (i=0;i<TempH*TempW;i++) {
	m_InitTemp[i]=0;
	}
	*/
	//模板1
	morTrans.m_InitTemp[1] = 1;
	morTrans.m_InitTemp[3] = 1;
	morTrans.m_InitTemp[4] = 1;
	morTrans.m_InitTemp[5] = 1;
	morTrans.m_InitTemp[7] = 1;

	morTrans_Output.m_InitTemp[1] = 1;
	morTrans_Output.m_InitTemp[3] = 1;
	morTrans_Output.m_InitTemp[4] = 1;
	morTrans_Output.m_InitTemp[5] = 1;
	morTrans_Output.m_InitTemp[7] = 1;

	/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if (morTrans.m_TempBuf) {
		delete[]morTrans.m_TempBuf;
		morTrans.m_TempBuf = NULL;
	}

	if (morTrans_Output.m_TempBuf) {
		delete[]morTrans_Output.m_TempBuf;
		morTrans_Output.m_TempBuf = NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans_Output.m_TempBuf = new int[morTrans_Output.TempW*morTrans_Output.TempH];
	morTrans_Output.m_TempBuf = new int[morTrans_Output.TempW*morTrans_Output.TempH];

	int i0, j0;
	for (i0 = 0; i0<morTrans.TempH; i0++) {
		for (j0 = 0; j0<morTrans.TempW; j0++) {
			morTrans.m_TempBuf[i0*morTrans.TempW + j0] = morTrans.m_InitTemp[i0*morTrans.TempW + j0];
		}
	}

	for (i0 = 0; i0 < morTrans_Output.TempH; i0++) {
		for (j0 = 0; j0 < morTrans_Output.TempW; j0++) {
			morTrans_Output.m_TempBuf[i0*morTrans_Output.TempW + j0] = morTrans_Output.m_InitTemp[i0*morTrans_Output.TempW + j0];
		}
	}

	// 单独处理
	morTrans.ImgErosion(morTrans.m_pImgData, morTrans.m_pImgDataOut, morTrans.m_imgWidth, morTrans.m_imgHeight,
		morTrans.m_TempBuf, morTrans.TempW, morTrans.TempH);
	// 叠加处理
	morTrans_Output.ImgErosion(morTrans_Output.m_pImgData, morTrans_Output.m_pImgDataOut, morTrans_Output.m_imgWidth, morTrans_Output.m_imgHeight,
		morTrans_Output.m_TempBuf, morTrans_Output.TempW, morTrans_Output.TempH);
	/*	*/

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(morTrans.GetDimensions(), morTrans.m_nBitCountOut, morTrans.m_lpColorTable, morTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(morTrans_Output.GetDimensions(),
		morTrans_Output.m_nBitCountOut, morTrans_Output.m_lpColorTable, morTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
	
}

void CMSMoneyDemoView::OnDilation()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断是否二值图像
	int i, j;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i<size.cy; i++) {
		for (j = 0; j<size.cx; j++) {
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if (*(pDib->m_pImgData + i * lineByte + j) != 0
				&& *(pDib->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//非二值图像
	if (!flag) {
		::MessageBox(0, "需要输入二值图像", MB_OK, 0);
		return;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	Morphology morTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//定义结构元素模板，默认为3*3模板
	/*
	int i;
	for (i=0;i<TempH*TempW;i++) {
	m_InitTemp[i]=0;
	}
	*/
	//模板1
	morTrans.m_InitTemp[1] = 1;
	morTrans.m_InitTemp[3] = 1;
	morTrans.m_InitTemp[4] = 1;
	morTrans.m_InitTemp[5] = 1;
	morTrans.m_InitTemp[7] = 1;

	morTrans_Output.m_InitTemp[1] = 1;
	morTrans_Output.m_InitTemp[3] = 1;
	morTrans_Output.m_InitTemp[4] = 1;
	morTrans_Output.m_InitTemp[5] = 1;
	morTrans_Output.m_InitTemp[7] = 1;

	/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if (morTrans.m_TempBuf) {
		delete[]morTrans.m_TempBuf;
		morTrans.m_TempBuf = NULL;
	}

	if (morTrans_Output.m_TempBuf) {
		delete[]morTrans_Output.m_TempBuf;
		morTrans_Output.m_TempBuf = NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf = new int[morTrans.TempW*morTrans.TempH];
	morTrans_Output.m_TempBuf = new int[morTrans_Output.TempW*morTrans_Output.TempH];

	int i0, j0;
	for (i0 = 0; i0<morTrans.TempH; i0++) {
		for (j0 = 0; j0<morTrans.TempW; j0++) {
			morTrans.m_TempBuf[i0*morTrans.TempW + j0] = morTrans.m_InitTemp[i0*morTrans.TempW + j0];
		}
	}

	for (i0 = 0; i0 < morTrans_Output.TempH; i0++) {
		for (j0 = 0; j0 < morTrans_Output.TempW; j0++) {
			morTrans_Output.m_TempBuf[i0*morTrans_Output.TempW + j0] = morTrans_Output.m_InitTemp[i0*morTrans_Output.TempW + j0];
		}
	}

	morTrans.ImgDilation(morTrans.m_pImgData, morTrans.m_pImgDataOut, morTrans.m_imgWidth, morTrans.m_imgHeight,
		morTrans.m_TempBuf, morTrans.TempW, morTrans.TempH);

	// 叠加处理
	morTrans_Output.ImgDilation(morTrans_Output.m_pImgData, morTrans_Output.m_pImgDataOut, morTrans_Output.m_imgWidth, morTrans_Output.m_imgHeight,
		morTrans_Output.m_TempBuf, morTrans_Output.TempW, morTrans_Output.TempH);
	/*	*/

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(morTrans.GetDimensions(), morTrans.m_nBitCountOut, morTrans.m_lpColorTable, morTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(morTrans_Output.GetDimensions(),
		morTrans_Output.m_nBitCountOut, morTrans_Output.m_lpColorTable, morTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();

}

void CMSMoneyDemoView::OnOpen()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断是否二值图像
	int i, j;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i<size.cy; i++) {
		for (j = 0; j<size.cx; j++) {
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if (*(pDib->m_pImgData + i * lineByte + j) != 0
				&& *(pDib->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//非二值图像
	if (!flag) {
		::MessageBox(0, "需要输入二值图像", MB_OK, 0);
		return;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	Morphology morTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//定义结构元素模板，默认为3*3模板
	/*
	int i;
	for (i=0;i<TempH*TempW;i++) {
	m_InitTemp[i]=0;
	}
	*/
	//模板1
	morTrans.m_InitTemp[1] = 1;
	morTrans.m_InitTemp[3] = 1;
	morTrans.m_InitTemp[4] = 1;
	morTrans.m_InitTemp[5] = 1;
	morTrans.m_InitTemp[7] = 1;

	morTrans_Output.m_InitTemp[1] = 1;
	morTrans_Output.m_InitTemp[3] = 1;
	morTrans_Output.m_InitTemp[4] = 1;
	morTrans_Output.m_InitTemp[5] = 1;
	morTrans_Output.m_InitTemp[7] = 1;

	/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if (morTrans.m_TempBuf) {
		delete[]morTrans.m_TempBuf;
		morTrans.m_TempBuf = NULL;
	}
	if (morTrans_Output.m_TempBuf) {
		delete[]morTrans_Output.m_TempBuf;
		morTrans_Output.m_TempBuf = NULL;
	}

	// 将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf = new int[morTrans.TempW*morTrans.TempH];
	morTrans_Output.m_TempBuf = new int[morTrans_Output.TempW*morTrans_Output.TempH];

	int i0, j0;
	for (i0 = 0; i0<morTrans.TempH; i0++) {
		for (j0 = 0; j0<morTrans.TempW; j0++) {
			morTrans.m_TempBuf[i0*morTrans.TempW + j0] = morTrans.m_InitTemp[i0*morTrans.TempW + j0];
		}
	}
	for (i0 = 0; i0 < morTrans_Output.TempH; i0++) {
		for (j0 = 0; j0 < morTrans_Output.TempW; j0++) {
			morTrans_Output.m_TempBuf[i0*morTrans_Output.TempW + j0] = morTrans_Output.m_InitTemp[i0*morTrans_Output.TempW + j0];
		}
	}

	morTrans.Open();
	morTrans_Output.Open();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(morTrans.GetDimensions(), morTrans.m_nBitCountOut, morTrans.m_lpColorTable, morTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(morTrans_Output.GetDimensions(),
		morTrans_Output.m_nBitCountOut, morTrans_Output.m_lpColorTable, morTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();

}

void CMSMoneyDemoView::OnClose()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断是否二值图像
	int i, j;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i<size.cy; i++) {
		for (j = 0; j<size.cx; j++) {
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if (*(pDib->m_pImgData + i * lineByte + j) != 0
				&& *(pDib->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//非二值图像
	if (!flag) {
		::MessageBox(0, "需要输入二值图像", MB_OK, 0);
		return;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	Morphology morTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//定义结构元素模板，默认为3*3模板
	/*
	int i;
	for (i=0;i<TempH*TempW;i++) {
	m_InitTemp[i]=0;
	}
	*/
	//模板1
	morTrans.m_InitTemp[1] = 1;
	morTrans.m_InitTemp[3] = 1;
	morTrans.m_InitTemp[4] = 1;
	morTrans.m_InitTemp[5] = 1;
	morTrans.m_InitTemp[7] = 1;

	morTrans_Output.m_InitTemp[1] = 1;
	morTrans_Output.m_InitTemp[3] = 1;
	morTrans_Output.m_InitTemp[4] = 1;
	morTrans_Output.m_InitTemp[5] = 1;
	morTrans_Output.m_InitTemp[7] = 1;

	/*	//模板2
	morTrans.m_InitTemp[0]=1;
	morTrans.m_InitTemp[3]=1;
	morTrans.m_InitTemp[4]=1;
	morTrans.m_InitTemp[6]=1;
	morTrans.m_InitTemp[7]=1;
	morTrans.m_InitTemp[8]=1;
	*/

	if (morTrans.m_TempBuf) {
		delete[]morTrans.m_TempBuf;
		morTrans.m_TempBuf = NULL;
	}
	if (morTrans_Output.m_TempBuf) {
		delete[]morTrans_Output.m_TempBuf;
		morTrans_Output.m_TempBuf = NULL;
	}

	//将输入的结构元素拷贝到m_TempBuf缓冲区中
	morTrans.m_TempBuf = new int[morTrans.TempW*morTrans.TempH];
	morTrans_Output.m_TempBuf = new int[morTrans_Output.TempW*morTrans_Output.TempH];

	int i0, j0;
	for (i0 = 0; i0<morTrans.TempH; i0++) {
		for (j0 = 0; j0<morTrans.TempW; j0++) {
			morTrans.m_TempBuf[i0*morTrans.TempW + j0] = morTrans.m_InitTemp[i0*morTrans.TempW + j0];
		}
	}
	for (i0 = 0; i0 < morTrans_Output.TempH; i0++) {
		for (j0 = 0; j0 < morTrans_Output.TempW; j0++) {
			morTrans_Output.m_TempBuf[i0*morTrans_Output.TempW + j0] = morTrans_Output.m_InitTemp[i0*morTrans_Output.TempW + j0];
		}
	}

	morTrans.Close();
	morTrans_Output.Close();

	/*	*/
	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(morTrans.GetDimensions(), morTrans.m_nBitCountOut, morTrans.m_lpColorTable, morTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(morTrans_Output.GetDimensions(),
		morTrans_Output.m_nBitCountOut, morTrans_Output.m_lpColorTable, morTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();

}

void CMSMoneyDemoView::OnThinning()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断是否二值图像
	int i, j;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i<size.cy; i++) {
		for (j = 0; j<size.cx; j++) {
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if (*(pDib->m_pImgData + i * lineByte + j) != 0
				&& *(pDib->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//非二值图像
	if (!flag) {
		::MessageBox(0, "需要输入二值图像", MB_OK, 0);
		return;
	}

	//定义Morphology类的对象mortrans，用当前DIB对其初始化
	Morphology morTrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	morTrans.ImgThinning();

	// 叠加处理
	Morphology morTrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	morTrans_Output.ImgThinning();


	/*	*/
	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(morTrans.GetDimensions(), morTrans.m_nBitCountOut, morTrans.m_lpColorTableOut, morTrans.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(morTrans_Output.GetDimensions(),
		morTrans_Output.m_nBitCountOut, morTrans_Output.m_lpColorTableOut, morTrans_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();

}

// 图像分割
void CMSMoneyDemoView::OnRobert()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	// 叠加处理
	ImgSegment segment_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	

	//Roberts算子
	segment.Roberts();
	segment_Output.Roberts();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(segment_Output.GetDimensions(),
		segment_Output.m_nBitCountOut, segment_Output.m_lpColorTable, segment_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnSobel()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	// 叠加处理
	ImgSegment segment_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//Sobel算子
	segment.Sobel();
	segment_Output.Sobel();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(segment_Output.GetDimensions(),
		segment_Output.m_nBitCountOut, segment_Output.m_lpColorTable, segment_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnPrewitt()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	// 叠加处理
	ImgSegment segment_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);


	//Prewitt算子
	segment.Prewitt();
	segment_Output.Prewitt();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);

	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(segment_Output.GetDimensions(),
		segment_Output.m_nBitCountOut, segment_Output.m_lpColorTable, segment_Output.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnLaplacian()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度和彩色图像
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	// 叠加处理
	ImgSegment segment_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//Laplacian算子
	segment.Laplacian();
	segment_Output.Laplacian();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(segment_Output.GetDimensions(),
		segment_Output.m_nBitCountOut, segment_Output.m_lpColorTable, segment_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnRegionGrow()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//定义分割类对象segment，并用当前DIB数据初始化
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	// 叠加处理
	ImgSegment segment_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);


	//阈值
	int nThresh = 3;

	//设定种子点位置
	CPoint Pos(segment.m_imgWidth / 2, segment.m_imgHeight / 2);
	CPoint Pos_Output(segment_Output.m_imgWidth / 2, segment_Output.m_imgHeight / 2);

	segment.RegionGrow(Pos, nThresh);
	segment_Output.RegionGrow(Pos_Output, nThresh);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(segment_Output.GetDimensions(),
		segment_Output.m_nBitCountOut, segment_Output.m_lpColorTable, segment_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnEdgeTrace()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度", MB_OK, 0);
		return;
	}

	//判断是否二值图像
	int i, j;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i<size.cy; i++) {
		for (j = 0; j<size.cx; j++) {
			if (*(pDib->m_pImgData + i * lineByte + j) != 0
				&& *(pDib->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//非二值图像
	if (!flag) {
		::MessageBox(0, "需要输入二值图像", MB_OK, 0);
		return;
	}
	//定义图像分割类对象segment，用当前DIB对其初始化
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	// 叠加处理
	ImgSegment segment_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//边界跟踪
	segment.EdgeTrace();
	segment_Output.EdgeTrace();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(segment_Output.GetDimensions(),
		segment_Output.m_nBitCountOut, segment_Output.m_lpColorTable, segment_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnAdaptThresholdSeg()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度", MB_OK, 0);
		return;
	}
	//定义图像分割类对象segment，用当前DIB对其初始化
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	// 叠加处理
	ImgSegment segment_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//自适应阈值分割
	segment.AdaptThreshSeg(pDib->m_pImgData);
	segment_Output.AdaptThreshSeg(pDib_Res->m_pImgData);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(segment_Output.GetDimensions(),
		segment_Output.m_nBitCountOut, segment_Output.m_lpColorTable, segment_Output.m_pImgDataOut);

	
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); pFrame->InvalidateRect(NULL, FALSE); pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnHistThreshSeg()
{
	// TODO: Add your command handler code here
	CMSMoneyDemoDoc *pDoc = GetDocument();
	 ImageDib *pDib = pDoc->m_dib;
	 ImageDib *pDib_Res = pDoc->m_dib_Result;

	//异常判断
	if (pDib->m_nBitCount != 8 && pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理灰度和彩色图像", MB_OK, 0);
		return;
	}

	//直方图绘制对话框
	HistSegmentDlg dlgHist;
	dlgHist.m_himageDib.ReplaceDib(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	HistSegmentDlg dlgHist_Output;
	dlgHist_Output.m_himageDib.ReplaceDib(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//显示直方图
	if (dlgHist.DoModal() == IDOK) {
		GrayTrans gray(dlgHist.m_himageDib.GetDimensions(), dlgHist.m_himageDib.m_nBitCount,
			dlgHist.m_himageDib.m_lpColorTable, dlgHist.m_himageDib.m_pImgData);
		gray.BinaryImage(dlgHist.m_threshchange);

		GrayTrans gray_Output(dlgHist_Output.m_himageDib.GetDimensions(), dlgHist_Output.m_himageDib.m_nBitCount,
			dlgHist_Output.m_himageDib.m_lpColorTable, dlgHist_Output.m_himageDib.m_pImgData);
		gray_Output.BinaryImage(dlgHist.m_threshchange);

		//      单文档显示变换结果
		state2 = 1; // 设置右视图可绘
		CClientDC dc(this);
		ImageDib *dibNew = pDoc->m_dib_SoleProc;
		dibNew->ReplaceDib(gray.GetDimensions(), gray.m_nBitCountOut, gray.m_lpColorTableOut, gray.m_pImgDataOut);

		// 更新叠加处理最终输出
		pDib_Res->ReplaceDib(gray_Output.GetDimensions(),
			gray_Output.m_nBitCountOut, gray_Output.m_lpColorTable, gray_Output.m_pImgDataOut);

		
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd); 
		pFrame->InvalidateRect(NULL, FALSE); 
		pFrame->UpdateWindow();
	}
	else
		return;

}

// 人脸检测
void CMSMoneyDemoView::OnSimilardegree()
{
	//获取文档类指针
	CMSMoneyDemoDoc *pDoc = GetDocument();
	//获取ImgCenterDib类对象m_dib的指针，访问当前DIB数据
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理彩色图像
	if (pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理彩色图像", MB_OK, 0);
		return;
	}

	// 计算相似度
	FaceDetect simcalculation(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData, NULL);
	simcalculation.CalSim();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);

	// 保存结果
	pDib_Res->ReplaceDib(simcalculation.GetDimensions(), simcalculation.m_nBitCountOut, simcalculation.m_lpColorTableOut, simcalculation.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Onboundary()
{
	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib_Res->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	int i, j, thd;
	CSize size_Output = pDib_Res->GetDimensions();
	int lineByte_Output = (size_Output.cx + 3) / 4 * 4;
	long pixel_scales_Output = 0;


	for (i = 0; i < size_Output.cx; i++) {
		for (j = 0; j < size_Output.cy; j++) {
			pixel_scales_Output += *(pDib_Res->m_pImgData + j * lineByte_Output + i);
		}
	}

	thd = pixel_scales_Output / (size_Output.cx*size_Output.cy);

	// 叠加处理
	GrayTrans graytrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount, 
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	graytrans_Output.BinaryImage(thd);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	ImageDib *dibNew = pDoc->m_dib_SoleProc;
	//将变换后的输出图像作为新建文档的DIB进行显示
	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(graytrans_Output.GetDimensions(),
		graytrans_Output.m_nBitCountOut, graytrans_Output.m_lpColorTable, graytrans_Output.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Onfacelocate()
{
	//获取相关联的文档类指针
	CMSMoneyDemoDoc* pDoc = GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1 = pDoc->m_dib;
	ImageDib *pdib2 = pDoc->m_dib_Result;

	ImageDib *pdibtemp;

	//处理一幅彩色和一幅二值图像
	if (pdib1->m_nBitCount == 24) {
		if (pdib2->m_nBitCount == 24) {
			::MessageBox(0, "需要一幅二值图像", MB_OK, 0);
			return;
		}
	}
	else {
		if (pdib2->m_nBitCount == 24) {
			pdibtemp = pdib1;
			pdib1 = pdib2;
			pdib2 = pdibtemp;
		}
		else {
			::MessageBox(0, "需要一幅彩色图像", MB_OK, 0);
			return;
		}
	}

	CSize size = pdib1->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int lineByte1 = (size.cx * 3 + 3) / 4 * 4;

	int i, j;
	double* temp = new double[pdib1->m_imgWidth];
	int count;
	for (i = 0; i < pdib1->m_imgWidth; i++) {
		count = 0;
		for (j = 0; j < pdib1->m_imgHeight; j++) {
			if (*(pdib2->m_pImgData + j * lineByte + i) == 255)
				count++;
		}
		temp[i] = count;
	}

	int max = 0;
	int pos = -1;
	for (i = 0; i < pdib1->m_imgWidth; i++) {
		if (temp[i] > max) {
			max = temp[i];
			pos = i;
		}
	}

	for (i = 0; i < pdib1->m_imgWidth; i++) {
		temp[i] = temp[i] / max;
	}

	
	for (i = pos; i >= 0; i--) {
		if (temp[i] < 0.2 || i == 0) {
			left = i;
			break;
		}
	}
	for (i = pos; i < pdib1->m_imgWidth; i++) {
		if (temp[i] < 0.2 || i == pdib1->m_imgWidth - 1) {
			right = i;
			break;
		}
	}

	if (temp != NULL)
	{
		delete[]temp;
		temp = NULL;
	}

	double* temp1 = new double[pdib1->m_imgHeight];
	for (j = 0; j < pdib1->m_imgHeight; j++) {
		count = 0;
		for (i = 0; i < pdib1->m_imgWidth; i++) {
			if (*(pdib2->m_pImgData + j * lineByte + i) == 255)
				count++;
		}
		temp1[j] = count;
	}

	max = 0;
	pos = -1;
	for (i = 0; i < pdib1->m_imgHeight; i++) {
		if (temp1[i] > max) {
			max = temp1[i];
			pos = i;
		}
	}

	for (i = 0; i < pdib1->m_imgHeight; i++) {
		temp1[i] = temp1[i] / max;
	}

	for (i = pos; i < pdib1->m_imgHeight; i++) {
		if (temp1[i] < 0.2 || i == 0) {
			top = i;
			break;
		}
	}

	if (temp1 != NULL)
	{
		delete[]temp1;
		temp1 = NULL;
	}

	bottom = top - (right - left)*1.2;

	i = 1;

	for (i = bottom; i <= top; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + left * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + left * 3 + 1) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + left * 3 + 2) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + right * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + right * 3 + 1) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + right * 3 + 2) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (left + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (left + 1) * 3 + 1) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (left + 1) * 3 + 2) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (right + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (right + 1) * 3 + 1) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (right + 1) * 3 + 2) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (left - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (left - 1) * 3 + 1) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (left - 1) * 3 + 2) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (right - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (right - 1) * 3 + 1) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (right - 1) * 3 + 2) = 0;
	}

	for (j = left; j <= right; j++) {
		*(pdib1->m_pImgData + top * lineByte1 + j * 3) = 0;
		*(pdib1->m_pImgData + top * lineByte1 + j * 3 + 1) = 255;
		*(pdib1->m_pImgData + top * lineByte1 + j * 3 + 2) = 0;
		*(pdib1->m_pImgData + bottom * lineByte1 + j * 3) = 0;
		*(pdib1->m_pImgData + bottom * lineByte1 + j * 3 + 1) = 255;
		*(pdib1->m_pImgData + bottom * lineByte1 + j * 3 + 2) = 0;
		*(pdib1->m_pImgData + (top + 1)*lineByte1 + j * 3) = 0;
		*(pdib1->m_pImgData + (top + 1)*lineByte1 + j * 3 + 1) = 255;
		*(pdib1->m_pImgData + (top + 1)*lineByte1 + j * 3 + 2) = 0;
		*(pdib1->m_pImgData + (bottom + 1)*lineByte1 + j * 3) = 0;
		*(pdib1->m_pImgData + (bottom + 1)*lineByte1 + j * 3 + 1) = 255;
		*(pdib1->m_pImgData + (bottom + 1)*lineByte1 + j * 3 + 2) = 0;
		*(pdib1->m_pImgData + (top - 1)*lineByte1 + j * 3) = 0;
		*(pdib1->m_pImgData + (top - 1)*lineByte1 + j * 3 + 1) = 255;
		*(pdib1->m_pImgData + (top - 1)*lineByte1 + j * 3 + 2) = 0;
		*(pdib1->m_pImgData + (bottom - 1)*lineByte1 + j * 3) = 0;
		*(pdib1->m_pImgData + (bottom - 1)*lineByte1 + j * 3 + 1) = 255;
		*(pdib1->m_pImgData + (bottom - 1)*lineByte1 + j * 3 + 2) = 0;
	}

	// 保存结果
	pdib2->ReplaceDib(pdib1->GetDimensions(), pdib1->m_nBitCount, pdib1->m_lpColorTable, pdib1->m_pImgData);

	ImageDib *pdib3 = pDoc->m_dib_SoleProc; 
	// 保存原始图像
	pdib1->ReplaceDib(pdib3->GetDimensions(), pdib3->m_nBitCount, pdib3->m_lpColorTable, pdib3->m_pImgData);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Oninnerboundary()
{
	// 人脸定位结果
	int temp = top;
	top = bottom - 5;
	bottom = temp + 5;
	left = left - 5;
	right = right + 5;

	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理彩色图像
	if (pDib_Res->m_nBitCount != 24) {
		::MessageBox(0, "只处理彩色图像", MB_OK, 0);
		return;
	}

	// 提取脸内轮廓
	FaceDetect BoundaryDetect(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData, NULL);

	BoundaryDetect.CalSBound(top, bottom, left, right);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(BoundaryDetect.GetDimensions(),
		BoundaryDetect.m_nBitCountOut, BoundaryDetect.m_lpColorTableOut, BoundaryDetect.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Oneyelocate()
{

	//获取相关联的文档类指针
	CMSMoneyDemoDoc* pDoc = GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1 = pDoc->m_dib;
	ImageDib *pdib2 = pDoc->m_dib_Result;

	ImageDib *pdibtemp;

	//处理一幅彩色和一幅二值图像
	if (pdib1->m_nBitCount == 24) {
		if (pdib2->m_nBitCount == 24) {
			::MessageBox(0, "需要一幅二值图像", MB_OK, 0);
			return;
		}
	}
	else {
		if (pdib2->m_nBitCount == 24) {
			pdibtemp = pdib1;
			pdib1 = pdib2;
			pdib2 = pdibtemp;
		}
		else {
			::MessageBox(0, "需要一幅彩色图像", MB_OK, 0);
			return;
		}
	}

	CSize size = pdib1->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int lineByte1 = (size.cx * 3 + 3) / 4 * 4;

	unsigned char *m_pImgDatabak;
	m_pImgDatabak = new unsigned char[pdib1->m_imgHeight*lineByte1];
	memcpy(m_pImgDatabak, pdib1->m_pImgData, pdib1->m_imgHeight*lineByte1);

	int i, j, l, m;

	//人脸中心
	int nCenterH = (right + left) / 2;
	int nCenterV = (bottom + top) / 2;
	//人脸面积
	int nArea = (right - left)*(bottom - top);
	//设定子窗口大小
	int nSubWinWidth = (right - left) / 12;
	int nSubWinHeight = (bottom - top) / 30;
	int nSubWinArea = nSubWinHeight * nSubWinWidth;

	//水平投影
	int *temp = new int[pdib1->m_imgWidth];
	for (i = 0; i < pdib1->m_imgWidth; i++)
		temp[i] = 0;

	for (i = nCenterV + nSubWinHeight; i < bottom - 6 * nSubWinHeight; i++)
		for (j = left + nSubWinWidth; j < right - nSubWinWidth; j++) {
			int count = 0;
			for (int p = -nSubWinHeight; p < nSubWinHeight; p++)
				for (int q = -nSubWinWidth; q < nSubWinWidth; q++) {
					if (*(pdib2->m_pImgData + (i + p)*lineByte + j + q) == 0)	count++;
				}
			if (count >= nSubWinArea / 3) {
				*(pdib1->m_pImgData + i * lineByte1 + j * 3) = 0;
				*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 1) = 0;
				*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) = 255;
				temp[j] ++;
			}
		}

	CList<CPoint, CPoint&> myList1(sizeof(CPoint));
	CList<CPoint, CPoint&> myList2(sizeof(CPoint));

	bool flag = 0;
	CPoint tPoint(-1, -1);
	for (i = 0; i < pdib1->m_imgWidth; i++) {
		if (temp[i] == 0 && flag == 1) {
			tPoint.y = i;
			myList1.AddTail(tPoint);
			flag = 0;
		}
		if (temp[i] > 0 && flag == 0) {
			tPoint.x = i;
			flag = 1;
		}
	}
	delete temp;
	//链表修正，去除长度过小点
	for (i = 0; i < myList1.GetCount(); i++) {
		CPoint temp(-1, -1);
		temp = myList1.GetAt(myList1.FindIndex(i));
		int minVal = (right - left) / 20;
		if ((temp.y - temp.x) >= minVal)
			myList2.AddTail(temp);
	}
	myList1.RemoveAll();
	//合并相邻很紧的区域
	bool quit = 1;
	while (quit) {
		bool doJoin = false;
		for (int i = 0; i < myList2.GetCount() - 1; i++) {
			CPoint temp1(-1, -1), temp2(-1, -1);
			temp1 = myList2.GetAt(myList2.FindIndex(i));
			temp2 = myList2.GetAt(myList2.FindIndex(i + 1));
			if ((temp2.x - temp1.y) <= (right - left) / 40) {
				temp1.y = temp2.y;
				myList2.RemoveAt(myList2.FindIndex(i));
				myList2.RemoveAt(myList2.FindIndex(i));
				if (i == 0)			myList2.AddHead(temp1);
				else			    myList2.InsertAfter(myList2.FindIndex(i - 1), temp1);
				doJoin = true;
				break;
			}
		}
		if (!doJoin)	quit = 0;
	}
	//左右眼的水平区域
	CPoint LEyeAreaH(-1, -1), REyeAreaH(-1, -1), LEyeAreaV(-1, -1), REyeAreaV(-1, -1);
	//没有找到眼睛区域
	if (myList2.GetCount() < 2) {
		CPoint t = myList2.GetHead();
		if ((t.y - t.x) > (right - left) / 2) {
			LEyeAreaH.x = t.x;
			LEyeAreaH.y = t.x + (t.y - t.x) / 3;
			REyeAreaH.x = t.y - (t.y - t.x) / 3;
			REyeAreaH.y = t.y;
		}
		else {
			AfxMessageBox("确认眼睛位置失败，请手动标定");
			return;
		}
	}
	//仅有两个区域
	else if (myList2.GetCount() == 2) {
		LEyeAreaH = myList2.GetHead();
		REyeAreaH = myList2.GetTail();
	}
	else {  //多于两个区域
		int ldis = -100000;
		int rdis = 100000;
		for (i = 0; i < myList2.GetCount(); i++) {
			CPoint temp(-1, -1);
			temp = myList2.GetAt(myList2.FindIndex(i));
			//右眼
			if ((temp.x + temp.y) / 2 > nCenterH) {
				if (((temp.x + temp.y) / 2 - nCenterH) < rdis) {
					rdis = (temp.x + temp.y) / 2 - nCenterH;
					REyeAreaH = temp;
				}
			}
			//左眼
			else {
				if (((temp.x + temp.y) / 2 - nCenterH) > ldis) {
					ldis = (temp.x + temp.y) / 2 - nCenterH;
					LEyeAreaH = temp;
				}
			}
		}
	}
	myList2.RemoveAll();
	///////////////////////////////////
	//确认两个眼睛的垂直区域
	//////////////////////////////////
	//左眼
	if (LEyeAreaH != CPoint(-1, -1)) {
		int *tArray = new int[pdib1->m_imgHeight];
		int i, j;
		for (i = 0; i < pdib1->m_imgHeight; i++) tArray[i] = 0;

		for (i = nCenterV + nSubWinHeight; i < bottom - 6 * nSubWinHeight; i++)
			for (j = LEyeAreaH.x; j <= LEyeAreaH.y; j++)
				if (*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) == 255 && *(pdib1->m_pImgData + i * lineByte1 + j * 3 + 1) == 0)
					tArray[i] ++;

		CList<CPoint, CPoint&> myListA(sizeof(CPoint));
		CList<CPoint, CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1, -1);
		for (i = nCenterV + nSubWinHeight; i < bottom - 6 * nSubWinHeight; i++) {
			if (tArray[i] > 0 && flag == 0) {
				tPoint.x = i;
				flag = 1;
			}
			if (tArray[i] == 0 && flag == 1) {
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for (i = 0; i < myListA.GetCount(); i++) {
			CPoint temp(-1, -1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top) / 100;
			if ((temp.y - temp.x) >= minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while (quit) {
			bool doJoin = false;
			for (int i = 0; i < myListB.GetCount() - 1; i++) {
				CPoint temp1(-1, -1), temp2(-1, -1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i + 1));
				if ((temp2.x - temp1.y) <= (bottom - top) / 100) {
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if (i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i - 1), temp1);
					doJoin = true;
					break;
				}
			}
			if (!doJoin)	quit = 0;
		}
		if (myListB.GetCount() == 0) {
			AfxMessageBox("无法确定左眼的位置");
		}
		else {
			LEyeAreaV = myListB.GetHead();

			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_LeftEyeLeftCorner.x = 100000;
			m_LeftEyeRightCorner.x = -1;

			for (i = LEyeAreaV.x; i <= LEyeAreaV.y; i++)
				for (j = LEyeAreaH.x; j <= LEyeAreaH.y; j++)
					if (*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) == 255 && *(pdib1->m_pImgData + i * lineByte1 + j * 3 + 1) == 0) {
						if (j < m_LeftEyeLeftCorner.x) {
							m_LeftEyeLeftCorner.x = j;
							m_LeftEyeLeftCorner.y = i;
						}
						if (j > m_LeftEyeRightCorner.x) {
							m_LeftEyeRightCorner.x = j;
							m_LeftEyeRightCorner.y = i;
						}
						sumX += j;
						sumY += i;
						sum++;
					}
			m_LeftEye.x = (int)(sumX / sum);
			m_LeftEye.y = (int)(sumY / sum);
		}
		myListB.RemoveAll();
	}
	//右眼
	if (REyeAreaH != CPoint(-1, -1)) {
		int *tArray = new int[pdib1->m_imgHeight];
		int i, j;
		for (i = 0; i < pdib1->m_imgHeight; i++) tArray[i] = 0;

		for (i = nCenterV + nSubWinHeight; i < bottom - 6 * nSubWinHeight; i++)
			for (j = REyeAreaH.x; j <= REyeAreaH.y; j++)
				if (*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) == 255 && *(pdib1->m_pImgData + i * lineByte1 + j * 3 + 1) == 0)
					tArray[i] ++;

		CList<CPoint, CPoint&> myListA(sizeof(CPoint));
		CList<CPoint, CPoint&> myListB(sizeof(CPoint));
		int flag = 0;
		CPoint tPoint(-1, -1);
		for (i = nCenterV + nSubWinHeight; i < bottom - 6 * nSubWinHeight; i++) {
			if (tArray[i] > 0 && flag == 0) {
				tPoint.x = i;
				flag = 1;
			}
			if (tArray[i] == 0 && flag == 1) {
				tPoint.y = i;
				myListA.AddTail(tPoint);
				flag = 0;
			}
		}
		delete tArray;
		//去掉长度太小的候选者	
		for (i = 0; i < myListA.GetCount(); i++) {
			CPoint temp(-1, -1);
			temp = myListA.GetAt(myListA.FindIndex(i));
			int minVal = (bottom - top) / 100;
			if ((temp.y - temp.x) >= minVal)
				myListB.AddTail(temp);
		}
		myListA.RemoveAll();
		//合并相邻很紧的区域
		bool quit = 1;
		while (quit) {
			bool doJoin = false;
			for (int i = 0; i < myListB.GetCount() - 1; i++) {
				CPoint temp1(-1, -1), temp2(-1, -1);
				temp1 = myListB.GetAt(myListB.FindIndex(i));
				temp2 = myListB.GetAt(myListB.FindIndex(i + 1));
				if ((temp2.x - temp1.y) <= (bottom - top) / 50) {
					temp1.y = temp2.y;
					myListB.RemoveAt(myListB.FindIndex(i));
					myListB.RemoveAt(myListB.FindIndex(i));
					if (i == 0)			myListB.AddHead(temp1);
					else			    myListB.InsertAfter(myListB.FindIndex(i - 1), temp1);
					doJoin = true;
					break;
				}
			}
			if (!doJoin)	quit = 0;
		}
		if (myListB.GetCount() == 0) {
			AfxMessageBox("无法确定右眼的位置");
		}
		else {
			if (myListB.GetCount() == 1)
				REyeAreaV = myListB.GetHead();
			else {
				CPoint tt = myListB.GetHead();
				int index = myListB.GetCount();
				while (tt.y > LEyeAreaV.x && index > 0) {
					index--;
					tt = myListB.GetAt(myListB.FindIndex(myListB.GetCount() - index));
				}
				REyeAreaV = tt;
			}

			double sumX = 0.0;
			double sumY = 0.0;
			int sum = 0;
			m_RightEyeLeftCorner.x = 100000;
			m_RightEyeRightCorner.x = -1;

			for (i = REyeAreaV.x; i <= REyeAreaV.y; i++)
				for (j = REyeAreaH.x; j <= REyeAreaH.y; j++)
					if (*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) == 255 && *(pdib1->m_pImgData + i * lineByte1 + j * 3 + 1) == 0) {
						if (j < m_RightEyeLeftCorner.x) {
							m_RightEyeLeftCorner.x = j;
							m_RightEyeLeftCorner.y = i;
						}
						if (j > m_RightEyeRightCorner.x) {
							m_RightEyeRightCorner.x = j;
							m_RightEyeRightCorner.y = i;
						}
						sumX += j;
						sumY += i;
						sum++;
					}
			m_RightEye.x = (int)(sumX / sum);
			m_RightEye.y = (int)(sumY / sum);
		}
		myListB.RemoveAll();
	}

	memcpy(pdib1->m_pImgData, m_pImgDatabak, pdib1->m_imgHeight*lineByte1);
	for (i = m_LeftEyeLeftCorner.x - 5; i < m_LeftEyeLeftCorner.x + 5; i++) {
		*(pdib1->m_pImgData + m_LeftEyeLeftCorner.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_LeftEyeLeftCorner.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_LeftEyeLeftCorner.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftEyeLeftCorner.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftEyeLeftCorner.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftEyeLeftCorner.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftEyeLeftCorner.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftEyeLeftCorner.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftEyeLeftCorner.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_LeftEyeLeftCorner.y - 5; i < m_LeftEyeLeftCorner.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEyeLeftCorner.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEyeLeftCorner.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEyeLeftCorner.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeLeftCorner.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeLeftCorner.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeLeftCorner.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeLeftCorner.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeLeftCorner.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeLeftCorner.x - 1) * 3 + 2) = 255;
	}
	for (i = m_LeftEyeRightCorner.x - 5; i < m_LeftEyeRightCorner.x + 5; i++) {
		*(pdib1->m_pImgData + m_LeftEyeRightCorner.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_LeftEyeRightCorner.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_LeftEyeRightCorner.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftEyeRightCorner.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftEyeRightCorner.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftEyeRightCorner.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftEyeRightCorner.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftEyeRightCorner.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftEyeRightCorner.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_LeftEyeRightCorner.y - 5; i < m_LeftEyeRightCorner.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEyeRightCorner.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEyeRightCorner.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEyeRightCorner.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeRightCorner.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeRightCorner.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeRightCorner.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeRightCorner.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeRightCorner.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEyeRightCorner.x - 1) * 3 + 2) = 255;
	}
	for (i = m_LeftEye.x - 5; i < m_LeftEye.x + 5; i++) {
		*(pdib1->m_pImgData + m_LeftEye.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_LeftEye.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_LeftEye.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftEye.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftEye.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftEye.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftEye.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftEye.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftEye.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_LeftEye.y - 5; i < m_LeftEye.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEye.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEye.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftEye.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEye.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEye.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEye.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEye.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEye.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftEye.x - 1) * 3 + 2) = 255;
	}

	for (i = m_RightEyeLeftCorner.x - 5; i < m_RightEyeLeftCorner.x + 5; i++) {
		*(pdib1->m_pImgData + m_RightEyeLeftCorner.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_RightEyeLeftCorner.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_RightEyeLeftCorner.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightEyeLeftCorner.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightEyeLeftCorner.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightEyeLeftCorner.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightEyeLeftCorner.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightEyeLeftCorner.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightEyeLeftCorner.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_RightEyeLeftCorner.y - 5; i < m_RightEyeLeftCorner.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEyeLeftCorner.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEyeLeftCorner.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEyeLeftCorner.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeLeftCorner.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeLeftCorner.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeLeftCorner.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeLeftCorner.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeLeftCorner.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeLeftCorner.x - 1) * 3 + 2) = 255;
	}
	for (i = m_RightEyeRightCorner.x - 5; i < m_RightEyeRightCorner.x + 5; i++) {
		*(pdib1->m_pImgData + m_RightEyeRightCorner.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_RightEyeRightCorner.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_RightEyeRightCorner.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightEyeRightCorner.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightEyeRightCorner.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightEyeRightCorner.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightEyeRightCorner.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightEyeRightCorner.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightEyeRightCorner.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_RightEyeRightCorner.y - 5; i < m_RightEyeRightCorner.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEyeRightCorner.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEyeRightCorner.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEyeRightCorner.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeRightCorner.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeRightCorner.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeRightCorner.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeRightCorner.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeRightCorner.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEyeRightCorner.x - 1) * 3 + 2) = 255;
	}
	for (i = m_RightEye.x - 5; i < m_RightEye.x + 5; i++) {
		*(pdib1->m_pImgData + m_RightEye.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_RightEye.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_RightEye.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightEye.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightEye.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightEye.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightEye.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightEye.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightEye.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_RightEye.y - 5; i < m_RightEye.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEye.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEye.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightEye.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEye.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEye.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEye.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEye.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEye.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightEye.x - 1) * 3 + 2) = 255;
	}

	if (m_pImgDatabak != NULL)
	{
		delete []m_pImgDatabak;
		m_pImgDatabak = NULL;
	}

	// 保存结果
	pdib2->ReplaceDib(pdib1->GetDimensions(), pdib1->m_nBitCount, pdib1->m_lpColorTable, pdib1->m_pImgData);

	ImageDib *pdib3 = pDoc->m_dib_SoleProc;
	// 保存原始图像
	pdib1->ReplaceDib(pdib3->GetDimensions(), pdib3->m_nBitCount, pdib3->m_lpColorTable, pdib3->m_pImgData);

	// 强制刷新所有窗口
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Onnoselocate()
{


	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pdib1 = pDoc->m_dib_Result;

	//只处理彩色图像
	if (pdib1->m_nBitCount != 24) {
		::MessageBox(0, "只处理彩色图像", MB_OK, 0);
		return;
	}

	ImageDib *pdibtemp;
	CSize size = pdib1->GetDimensions();
	int lineByte1 = (size.cx * 3 + 3) / 4 * 4;

	unsigned char *m_pImgDatabak;
	m_pImgDatabak = new unsigned char[pdib1->m_imgHeight*lineByte1];
	memcpy(m_pImgDatabak, pdib1->m_pImgData, pdib1->m_imgHeight*lineByte1);

	int i, j, l, m;

	for (i = top; i <= bottom; i++)
		for (j = left; j <= right; j++) {
			double Y;
			Y = 0.30*(*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2)) + 0.59*(*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 1))
				+ 0.11*(*(pdib1->m_pImgData + i * lineByte1 + j * 3));
			if (Y < 100) {
				*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) = 255;
			}
			else *(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) = 0;
		}

	AfxMessageBox("鼻子的肤色鉴定");
	//双目斜角
	double tanThta;
	if (m_RightEye.y == m_LeftEye.y) tanThta = 0;
	else tanThta = (m_RightEye.y - m_LeftEye.y) / (m_RightEye.x - m_LeftEye.x);
	//双目距离
	int EyesDis = (m_RightEye.x - m_LeftEye.x)*(m_RightEye.x - m_LeftEye.x);
	EyesDis += (m_RightEye.y - m_LeftEye.y)*(m_RightEye.y - m_LeftEye.y);
	EyesDis = (int)sqrt(EyesDis);
	//双目平均高度
	int EyeV = (m_RightEye.y + m_LeftEye.y) / 2;
	//可能的鼻子的区域
	int NoseUp = (EyeV - 0.5*EyesDis) < top ? top : (int)(EyeV - 0.5*EyesDis);
	int NoseDown = (EyeV - 0.8*EyesDis) < top ? top : (int)(EyeV - 0.8*EyesDis);

	int* Y_Arry = new int[NoseUp - NoseDown];
	for (i = 0; i < NoseUp - NoseDown; i++) Y_Arry[i] = 0;
	int* X_Arry = new int[EyesDis];
	for (i = 0; i < EyesDis; i++) X_Arry[i] = 0;
	for (i = NoseDown; i < NoseUp; i++)
		for (j = m_LeftEye.x + EyesDis / 5; j < m_RightEye.x - EyesDis / 5; j++) {
			if (*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) == 255) {
				Y_Arry[i - NoseDown] ++;
				X_Arry[j - m_LeftEye.x] ++;
			}
		}

	int maxY = 0;
	for (i = 0; i < NoseUp - NoseDown; i++) {
		if (Y_Arry[i] > maxY) {
			maxY = Y_Arry[i];
			m_MidNose.y = i + NoseDown;
		}
	}
	m_LeftNostril.y = (int)(m_MidNose.y - tanThta * EyesDis / 2);
	m_RightNostril.y = (int)(m_MidNose.y + tanThta * EyesDis / 2);
	for (i = 0; i < EyesDis; i++) {
		if (X_Arry[i] > 0) {
			m_LeftNostril.x = i + m_LeftEye.x;
			break;
		}
	}
	for (i = EyesDis - 1; i >= 0; i--) {
		if (X_Arry[i] > 0) {
			m_RightNostril.x = i + m_LeftEye.x;
			break;
		}
	}
	//唇中点较薄
	int min = 1000000;
	for (i = (int)(EyesDis / 3 + 0.5); i <= (int)(2 * EyesDis / 3 + 0.5); i++) {
		if (X_Arry[i] < min) {
			min = X_Arry[i];
			m_MidNose.x = m_LeftEye.x + i;
		}
	}
	m_MidNose.x = (m_MidNose.x + (m_LeftEye.x + EyesDis / 2)) / 2;

	memcpy(pdib1->m_pImgData, m_pImgDatabak, pdib1->m_imgHeight*lineByte1);
	for (i = m_LeftNostril.x - 5; i < m_LeftNostril.x + 5; i++) {
		*(pdib1->m_pImgData + m_LeftNostril.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_LeftNostril.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_LeftNostril.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftNostril.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftNostril.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftNostril.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftNostril.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftNostril.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftNostril.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_LeftNostril.y - 5; i < m_LeftNostril.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftNostril.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftNostril.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftNostril.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftNostril.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftNostril.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftNostril.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftNostril.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftNostril.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftNostril.x - 1) * 3 + 2) = 255;
	}
	for (i = m_RightNostril.x - 5; i < m_RightNostril.x + 5; i++) {
		*(pdib1->m_pImgData + m_RightNostril.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_RightNostril.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_RightNostril.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightNostril.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightNostril.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightNostril.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightNostril.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightNostril.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightNostril.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_RightNostril.y - 5; i < m_RightNostril.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_RightNostril.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightNostril.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightNostril.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightNostril.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightNostril.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightNostril.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightNostril.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightNostril.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightNostril.x - 1) * 3 + 2) = 255;
	}
	for (i = m_MidNose.x - 5; i < m_MidNose.x + 5; i++) {
		*(pdib1->m_pImgData + m_MidNose.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_MidNose.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_MidNose.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_MidNose.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_MidNose.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_MidNose.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_MidNose.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_MidNose.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_MidNose.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_MidNose.y - 5; i < m_MidNose.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_MidNose.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_MidNose.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_MidNose.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidNose.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidNose.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidNose.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidNose.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidNose.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidNose.x - 1) * 3 + 2) = 255;
	}

	if (m_pImgDatabak != NULL)
	{
		delete[]m_pImgDatabak;
		m_pImgDatabak = NULL;
	}

	if (X_Arry != NULL)
	{
		delete[]X_Arry;
		X_Arry = NULL;
	}

	if (Y_Arry != NULL)
	{
		delete[]Y_Arry;
		Y_Arry = NULL;
	}

	// 强制刷新所有窗口
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Onmouthlocate()
{
	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pdib1 = pDoc->m_dib_Result;

	//只处理彩色图像
	if (pdib1->m_nBitCount != 24) {
		::MessageBox(0, "只处理彩色图像", MB_OK, 0);
		return;
	}

	ImageDib *pdibtemp;

	CSize size = pdib1->GetDimensions();
	int lineByte1 = (size.cx * 3 + 3) / 4 * 4;

	unsigned char *m_pImgDatabak;
	m_pImgDatabak = new unsigned char[pdib1->m_imgHeight*lineByte1];
	memcpy(m_pImgDatabak, pdib1->m_pImgData, pdib1->m_imgHeight*lineByte1);

	int i, j, l, m;


	for (i = top; i <= bottom; i++)
		for (j = left; j <= right; j++) {
			BYTE R, G, B;
			double temp, dlta;
			R = *(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2);
			G = *(pdib1->m_pImgData + i * lineByte1 + j * 3 + 1);
			B = *(pdib1->m_pImgData + i * lineByte1 + j * 3);
			if ((R == G) && (G == B)) temp = 0;
			else temp = 0.5*(2 * R - G - B) / sqrt((R - G)*(R - G) + (R - B)*(G - B));
			dlta = acos(temp);
			if (dlta < 0.2) {
				*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) = 255;
			}
			else *(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) = 0;

		}

	AfxMessageBox("嘴的肤色鉴定");
	//双目斜角
	double tanThta;
	if (m_RightEye.y == m_LeftEye.y) tanThta = 0;
	else tanThta = (m_RightEye.y - m_LeftEye.y) / (m_RightEye.x - m_LeftEye.x);
	//双目距离
	int EyesDis = (m_RightEye.x - m_LeftEye.x)*(m_RightEye.x - m_LeftEye.x);
	EyesDis += (m_RightEye.y - m_LeftEye.y)*(m_RightEye.y - m_LeftEye.y);
	EyesDis = (int)sqrt(EyesDis);
	//双目平均高度
	int EyeV = (m_RightEye.y + m_LeftEye.y) / 2;
	//可能的嘴的区域
	int MouthUp = (EyeV - 1.0*EyesDis) < top ? top : (int)(EyeV - 1.0*EyesDis);
	int MouthDown = (EyeV - 1.5*EyesDis) < top ? top : (int)(EyeV - 1.5*EyesDis);


	int* Y_Arry = new int[MouthUp - MouthDown];
	for (i = 0; i < MouthUp - MouthDown; i++) Y_Arry[i] = 0;
	int* X_Arry = new int[EyesDis];
	for (i = 0; i < EyesDis; i++) X_Arry[i] = 0;
	for (i = MouthDown; i < MouthUp; i++)
		for (j = m_LeftEye.x; j < m_RightEye.x; j++) {
			if (*(pdib1->m_pImgData + i * lineByte1 + j * 3 + 2) == 255) {
				Y_Arry[i - MouthDown] ++;
				X_Arry[j - m_LeftEye.x] ++;
			}
		}

	int maxY = 0;
	for (i = 0; i < MouthUp - MouthDown; i++) {
		if (Y_Arry[i] > maxY) {
			maxY = Y_Arry[i];
			m_MidMouth.y = i + MouthDown - (MouthUp - MouthDown) / 10;
		}
	}
	m_LeftMouthCorner.y = (int)(m_MidMouth.y - tanThta * EyesDis / 2);
	m_RightMouthCorner.y = (int)(m_MidMouth.y + tanThta * EyesDis / 2);
	for (i = 0; i < EyesDis; i++) {
		if (X_Arry[i] > 0) {
			m_LeftMouthCorner.x = i + m_LeftEye.x;
			break;
		}
	}
	for (i = EyesDis - 1; i >= 0; i--) {
		if (X_Arry[i] > 0) {
			m_RightMouthCorner.x = m_LeftEye.x + i;
			break;
		}
	}

	//唇中点较薄
	int min = 1000000;
	for (i = (int)(3 * EyesDis / 7 + 0.5); i <= (int)(4 * EyesDis / 7 + 0.5); i++) {
		if (X_Arry[i] < min) {
			min = X_Arry[i];
			m_MidMouth.x = m_LeftEye.x + i;
		}
	}
	m_MidMouth.x = (int)(m_MidMouth.x + (m_LeftEye.x + EyesDis / 2)) / 2;

	memcpy(pdib1->m_pImgData, m_pImgDatabak, pdib1->m_imgHeight*lineByte1);
	for (i = m_LeftMouthCorner.x - 5; i < m_LeftMouthCorner.x + 5; i++) {
		*(pdib1->m_pImgData + m_LeftMouthCorner.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_LeftMouthCorner.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_LeftMouthCorner.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftMouthCorner.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftMouthCorner.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftMouthCorner.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_LeftMouthCorner.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_LeftMouthCorner.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_LeftMouthCorner.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_LeftMouthCorner.y - 5; i < m_LeftMouthCorner.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftMouthCorner.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftMouthCorner.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_LeftMouthCorner.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftMouthCorner.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftMouthCorner.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftMouthCorner.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftMouthCorner.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftMouthCorner.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_LeftMouthCorner.x - 1) * 3 + 2) = 255;
	}
	for (i = m_RightMouthCorner.x - 5; i < m_RightMouthCorner.x + 5; i++) {
		*(pdib1->m_pImgData + m_RightMouthCorner.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_RightMouthCorner.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_RightMouthCorner.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightMouthCorner.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightMouthCorner.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightMouthCorner.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_RightMouthCorner.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_RightMouthCorner.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_RightMouthCorner.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_RightMouthCorner.y - 5; i < m_RightMouthCorner.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_RightMouthCorner.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightMouthCorner.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_RightMouthCorner.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightMouthCorner.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightMouthCorner.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightMouthCorner.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightMouthCorner.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightMouthCorner.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_RightMouthCorner.x - 1) * 3 + 2) = 255;
	}
	for (i = m_MidMouth.x - 5; i < m_MidMouth.x + 5; i++) {
		*(pdib1->m_pImgData + m_MidMouth.y*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + m_MidMouth.y*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + m_MidMouth.y*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_MidMouth.y + 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_MidMouth.y + 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_MidMouth.y + 1)*lineByte1 + i * 3 + 2) = 255;
		*(pdib1->m_pImgData + (m_MidMouth.y - 1)*lineByte1 + i * 3) = 0;
		*(pdib1->m_pImgData + (m_MidMouth.y - 1)*lineByte1 + i * 3 + 1) = 0;
		*(pdib1->m_pImgData + (m_MidMouth.y - 1)*lineByte1 + i * 3 + 2) = 255;
	}
	for (i = m_MidMouth.y - 5; i < m_MidMouth.y + 5; i++) {
		*(pdib1->m_pImgData + i * lineByte1 + m_MidMouth.x * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_MidMouth.x * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + m_MidMouth.x * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidMouth.x + 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidMouth.x + 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidMouth.x + 1) * 3 + 2) = 255;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidMouth.x - 1) * 3) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidMouth.x - 1) * 3 + 1) = 0;
		*(pdib1->m_pImgData + i * lineByte1 + (m_MidMouth.x - 1) * 3 + 2) = 255;
	}

	if (m_pImgDatabak != NULL)
	{
		delete []m_pImgDatabak;
		m_pImgDatabak = NULL;
	}

	if (X_Arry != NULL)
	{
		delete []X_Arry;
		X_Arry = NULL;
	}

	if (Y_Arry != NULL)
	{
		delete []Y_Arry;
		Y_Arry = NULL;
	}

	// 强制刷新所有窗口
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

// 车号识别
void CMSMoneyDemoView::OnColorToGray()
{
	//获取文档类指针
	CMSMoneyDemoDoc *pDoc = GetDocument();
	//获取ImgCenterDib类对象m_dib的指针，访问当前DIB数据
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理彩色图像
	if (pDib->m_nBitCount != 24) {
		::MessageBox(0, "只处理彩色图像", MB_OK, 0);
		return;
	}

	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义GrayTrans类的对象graytrans
	GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//彩色变灰度格式
	graytrans.ColorToGray();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	//将变换后的输出图像作为新建文档的DIB进行显示
	pDib_Res->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);

	pDoc->m_dib_Temp->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnBinary2()
{
	// TODO: Add your command handler code here
	//获取文档类中m_dib的指针，访问当前DIB数据
	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//异常判断
	if (pDib_Res->m_nBitCount != 24 && pDib_Res->m_nBitCount != 8) {
		::MessageBox(0, "只处理彩色和灰度图像", MB_OK, 0);
		return;
	}

	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	// 以上一次处理结果初始化几何变换对象
	GrayTrans graytrans_Output(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);
	//叠加二值化处理
	graytrans_Output.BinaryImage();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);

	// 更新叠加处理最终输出
	pDib_Res->ReplaceDib(graytrans_Output.GetDimensions(),
		graytrans_Output.m_nBitCountOut, graytrans_Output.m_lpColorTable, graytrans_Output.m_pImgDataOut);

	pDoc->m_dib_SoleProc->ReplaceDib(graytrans_Output.GetDimensions(),
		graytrans_Output.m_nBitCountOut, graytrans_Output.m_lpColorTable, graytrans_Output.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::OnColorPair()
{
	//获取相关联的文档类指针
	CMSMoneyDemoDoc* pDoc = GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pdib1 = pDoc->m_dib;
	ImageDib *pdib2 = pDoc->m_dib_Result;

	ImageDib *pdibtemp;

	//处理一幅彩色和一幅二值图像
	if (pdib1->m_nBitCount == 24) {
		if (pdib2->m_nBitCount == 24) {
			::MessageBox(0, "需要一幅二值图像", MB_OK, 0);
			return;
		}
	}
	else {
		if (pdib2->m_nBitCount == 24) {
			pdibtemp = pdib1;
			pdib1 = pdib2;
			pdib2 = pdibtemp;
		}
		else {
			::MessageBox(0, "需要一幅彩色图像", MB_OK, 0);
			return;
		}
	}


	//只处理彩色图像
	if (pdib1->m_nBitCount != 24) {
		::MessageBox(0, "只处理彩色图像", MB_OK, 0);
		return;
	}

	//将pDib中的图像数据作为输入数据，调用带参数的构造函数，
	//定义PlateTrans类的对象colorpair
	PlateTrans colorpair(pdib1->GetDimensions(), pdib1->m_nBitCount,
		pdib1->m_lpColorTable, pdib1->m_pImgData, pdib2->m_pImgData);

	//有色点对搜索
	colorpair.ColorPairSearch();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);

	// 保存结果
	pdib2->ReplaceDib(colorpair.GetDimensions(), colorpair.m_nBitCountOut, colorpair.m_lpColorTableOut, colorpair.m_pImgDataOut);
	ImageDib *pdib3 = pDoc->m_dib_SoleProc;
	// 保存原始图像
	pdib1->ReplaceDib(pdib3->GetDimensions(), pdib3->m_nBitCount, pdib3->m_lpColorTable, pdib3->m_pImgData);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Onplatelocate()
{

	//获取文档类指针
	CMSMoneyDemoDoc *pDoc = GetDocument();
	//获取ImgCenterDib类对象m_dib的指针，访问当前DIB数据
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib_Res->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	//判断是否二值图像
	int i, j;
	CSize size = pDib_Res->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i < size.cy; i++) {
		for (j = 0; j < size.cx; j++) {
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if (*(pDib_Res->m_pImgData + i * lineByte + j) != 0
				&& *(pDib_Res->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//非二值图像
	if (!flag) {
		::MessageBox(0, "需要输入二值图像", MB_OK, 0);
		return;
	}

	Morphology morph(size, pDib_Res->m_nBitCount, pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	int m_defineMask[25];
	for (i = 0; i < 25; i++) m_defineMask[i] = 0;
	m_defineMask[7] = 1;
	m_defineMask[11] = 1;
	m_defineMask[12] = 1;
	m_defineMask[13] = 1;
	m_defineMask[17] = 1;
	morph.InputMask(m_defineMask, 5, 5);

	morph.Open();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	pDib_Res->ReplaceDib(morph.GetDimensions(), morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);
	pDib->ReplaceDib(morph.GetDimensions(), morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();

	int times;

	for (times = 0; times < 10; times++) {
		morph.ImgErosion(morph.m_pImgData, morph.m_pImgDataOut, morph.m_imgWidth, morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		pDib_Res->ReplaceDib(size, morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData = pDib_Res->m_pImgData;
	}

	for (times = 0; times < 12; times++) {
		morph.ImgDilation(morph.m_pImgData, morph.m_pImgDataOut, morph.m_imgWidth, morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		pDib_Res->ReplaceDib(size, morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData = pDib_Res->m_pImgData;
	}

	for (times = 0; times < 2; times++) {
		morph.ImgErosion(morph.m_pImgData, morph.m_pImgDataOut, morph.m_imgWidth, morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		pDib_Res->ReplaceDib(size, morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData = pDib_Res->m_pImgData;
	}


	///粗寻找车牌纵向坐标
	int x1, x2, xx1, xx2, y1, y2, yy1, yy2, pcount;
	float factor;
	flag = 0;
	x1 = x2 = y1 = y2 = 1;
	xx1 = xx2 = yy1 = yy2 = 1;
	for (i = 20; i < size.cy - 20; i++) {
		pcount = 0;
		for (j = 20; j < size.cx - 20; j++) {
			if (*(pDib_Res->m_pImgData + i * lineByte + j) == 0)	pcount++;
		}
		if (pcount > 20)
		{
			if (flag == 0) {
				y1 = i;
				y2 = i;
				flag = 1;
			}
			else {
				y2 = i;
			}
		}
		else {
			if (flag == 1) {
				if ((y2 - y1) < 5) {
					flag = 0;
					y1 = 1;
					y2 = 1;
				}
				else {
					yy1 = y1;
					yy2 = y2;
					flag = 0;
					break;
				}
			}
		}
	}

	///粗寻找车牌横向坐标
	flag = 0;
	for (i = 20; i < size.cx - 20; i++) {
		pcount = 0;
		for (j = yy1; j <= yy2; j++) {
			if (*(pDib_Res->m_pImgData + j * lineByte + i) == 0)	pcount++;
		}
		if (pcount > 10) {
			if (flag == 0) {
				x1 = i;
				x2 = i;
				flag = 1;
			}
			else {
				x2 = i;
			}
		}
		else {
			if (flag == 1) {
				if ((x2 - x1) < 5) {
					flag = 0;
					x1 = 1;
					x2 = 1;
				}
				else {
					xx1 = x1;
					xx2 = x2;
					flag = 0;
				}
			}
		}
	}

	///细寻找车牌纵向坐标
	flag = 0;
	for (i = yy1; i <= yy2; i++) {
		pcount = 0;
		for (j = xx1; j <= xx2; j++) {
			if (*(pDib_Res->m_pImgData + i * lineByte + j) == 0)	pcount++;
		}
		if (pcount > 10) {
			if (flag == 0) {
				y1 = i;
				y2 = i;
				flag = 1;
			}
			else {
				y2 = i;
			}
		}
		else {
			if (flag == 1) {
				if ((y2 - y1) < 5) {
					flag = 0;
					y1 = 1;
					y2 = 1;
				}
			}
		}
	}
	yy1 = y1;
	yy2 = y2;
	factor = (float)(xx2 - xx1) / (yy2 - yy1);


	int Refpcount, frq, dirc;
	Refpcount = 0;
	frq = 0;
	dirc = 1;

	for (j = yy1; j < yy2; j++) {
		if (*(pDib->m_pImgData + j * lineByte + xx1) == 0)	Refpcount++;
	}

	for (i = xx1; i < xx2; i++) {
		for (j = yy1; j < yy2; j++) {
			if (*(pDib->m_pImgData + j * lineByte + i) == 0)	pcount++;
		}
		if (dirc == 1) {
			if (pcount > Refpcount) Refpcount = pcount;
			if (Refpcount > 2 * pcount) {
				frq++;
				dirc = 0;
				Refpcount = pcount;
			}
			pcount = 0;
		}
		else {
			if (pcount < Refpcount) Refpcount = pcount;
			if (pcount > 2 * Refpcount) {
				frq++;
				dirc = 1;
				Refpcount = pcount;
			}
			pcount = 0;
		}
	}


	if (factor > 2 && factor < 4 && frq>5) {
		for (i = yy1 - 20; i < yy2 + 20; i++) {
			*(pDib->m_pImgData + i * lineByte + xx1) = 0;
			*(pDib->m_pImgData + i * lineByte + xx2) = 0;
		}
		for (i = xx1 - 20; i < xx2 + 20; i++) {
			*(pDib->m_pImgData + yy1 * lineByte + i) = 0;
			*(pDib->m_pImgData + yy2 * lineByte + i) = 0;
		}

		int pangle[100], pavg[500], pnum, psum, pdiff;

		for (i = 0; i < 100; i++) pangle[i] = 0;

		for (i = xx1; i < xx2; i++) {
			pnum = 0;
			psum = 0;
			for (j = yy1; j < yy2; j++) {
				if (*(pDib_Res->m_pImgData + j * lineByte + i) == 0) {
					psum += j;
					pnum++;
				}
			}
			pavg[i - xx1] = psum / pnum;
		}

		for (i = xx1 + 10; i < xx2 - 50; i++) {
			pnum = 0;
			psum = 0;
			pdiff = pavg[i - xx1 + 40] - pavg[i - xx1];
			if (pdiff > 49) pdiff = 49;
			if (pdiff < -50) pdiff = -50;
			pangle[50 + pdiff]++;
		}

		int pglb, lglb;
		float angle;

		pglb = pangle[0];
		lglb = 0;
		for (i = 1; i < 100; i++) {
			if (pangle[i] > pglb) {
				pglb = pangle[i];
				lglb = i;
			}
		}


		// 保存结果
		pDib_Res->ReplaceDib(pDib->GetDimensions(), pDib->m_nBitCount, pDib->m_lpColorTable, pDib->m_pImgData);

		angle = atan((float)(lglb - 50) / 40.0) * 180 / 3.14;
		CString angle_str;
		angle_str.Format("%4.2f", angle);
		MessageBox("角度:" + angle_str + "度", "旋转校正角度", MB_ICONINFORMATION);

	}


	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Onplaterelocate()
{
	//获取相关联的文档类指针
	CMSMoneyDemoDoc* pDoc = GetDocument();

	//获取新文档中的ImgCenterDib类对象指针
	ImageDib *pDib1 = pDoc->m_dib;
	ImageDib *pDib2 = pDoc->m_dib_Result;
	ImageDib *pDib3 = pDoc->m_dib_SoleProc;
	// 保存原始图像
	pDib3->ReplaceDib(pDib2->GetDimensions(), pDib2->m_nBitCount, pDib2->m_lpColorTable, pDib2->m_pImgData);

	//处理一幅彩色和一幅二值图像
	if (pDib1->m_nBitCount == 8) {
		if (pDib2->m_nBitCount == 24) {
			::MessageBox(0, "不需要彩色图像", MB_OK, 0);
			return;
		}
	}
	else {
		::MessageBox(0, "不需要彩色图像", MB_OK, 0);
		return;
	}

	int i, j;
	CSize size = pDib2->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i < size.cy; i++) {
		for (j = 0; j < size.cx; j++) {
			//如果图像的数据像素灰度值既不为0也不为255,则非二值图像
			if (*(pDib2->m_pImgData + i * lineByte + j) != 0
				&& *(pDib2->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//非二值图像
	if (!flag) {
		::MessageBox(0, "需要输入二值图像", MB_OK, 0);
		return;
	}

	Morphology morph(size, pDib2->m_nBitCount, pDib2->m_lpColorTable, pDib2->m_pImgData);

	int m_defineMask[25];
	for (i = 0; i < 25; i++) m_defineMask[i] = 0;
	m_defineMask[7] = 1;
	m_defineMask[11] = 1;
	m_defineMask[12] = 1;
	m_defineMask[13] = 1;
	m_defineMask[17] = 1;
	morph.InputMask(m_defineMask, 5, 5);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	pDib2->ReplaceDib(morph.GetDimensions(), morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);


	int times;

	for (times = 0; times < 10; times++) {
		morph.ImgErosion(morph.m_pImgData, morph.m_pImgDataOut, morph.m_imgWidth, morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		pDib2->ReplaceDib(size, morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData = pDib2->m_pImgData;
	}

	for (times = 0; times < 12; times++) {
		morph.ImgDilation(morph.m_pImgData, morph.m_pImgDataOut, morph.m_imgWidth, morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		pDib2->ReplaceDib(size, morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData = pDib2->m_pImgData;
	}

	for (times = 0; times < 2; times++) {
		morph.ImgErosion(morph.m_pImgData, morph.m_pImgDataOut, morph.m_imgWidth, morph.m_imgHeight,
			morph.m_TempBuf, morph.TempW, morph.TempH);
		pDib2->ReplaceDib(size, morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);
		morph.m_pImgData = pDib2->m_pImgData;
	}

	///粗寻找车牌纵向坐标
	int x1, x2, xx1, xx2, y1, y2, yy1, yy2, pcount;
	float factor;
	flag = 0;
	x1 = x2 = y1 = y2 = 1;
	xx1 = xx2 = yy1 = yy2 = 1;
	for (i = 20; i < size.cy - 20; i++) {
		pcount = 0;
		for (j = 20; j < size.cx - 20; j++) {
			if (*(pDib2->m_pImgData + i * lineByte + j) == 0)	pcount++;
		}
		if (pcount > 20)
		{
			if (flag == 0) {
				y1 = i;
				y2 = i;
				flag = 1;
			}
			else {
				y2 = i;
			}
		}
		else {
			if (flag == 1) {
				if ((y2 - y1) < 5) {
					flag = 0;
					y1 = 1;
					y2 = 1;
				}
				else {
					yy1 = y1;
					yy2 = y2;
					flag = 0;
					break;
				}
			}
		}
	}

	///粗寻找车牌横向坐标
	flag = 0;
	for (i = 20; i < size.cx - 20; i++) {
		pcount = 0;
		for (j = yy1; j <= yy2; j++) {
			if (*(pDib2->m_pImgData + j * lineByte + i) == 0)	pcount++;
		}
		if (pcount > 10) {
			if (flag == 0) {
				x1 = i;
				x2 = i;
				flag = 1;
			}
			else {
				x2 = i;
			}
		}
		else {
			if (flag == 1) {
				if ((x2 - x1) < 5) {
					flag = 0;
					x1 = 1;
					x2 = 1;
				}
				else {
					xx1 = x1;
					xx2 = x2;
					flag = 0;
				}
			}
		}
	}

	///细寻找车牌纵向坐标
	flag = 0;
	for (i = yy1; i <= yy2; i++) {
		pcount = 0;
		for (j = xx1; j <= xx2; j++) {
			if (*(pDib2->m_pImgData + i * lineByte + j) == 0)	pcount++;
		}
		if (pcount > 10) {
			if (flag == 0) {
				y1 = i;
				y2 = i;
				flag = 1;
			}
			else {
				y2 = i;
			}
		}
		else {
			if (flag == 1) {
				if ((y2 - y1) < 5) {
					flag = 0;
					y1 = 1;
					y2 = 1;
				}
			}
		}
	}
	yy1 = y1;
	yy2 = y2;
	factor = (float)(xx2 - xx1) / (yy2 - yy1);


	int Refpcount, frq, dirc;
	Refpcount = 0;
	frq = 0;
	dirc = 1;

	for (j = yy1; j < yy2; j++) {
		if (*(pDib1->m_pImgData + j * lineByte + xx1) == 0)	Refpcount++;
	}

	for (i = xx1; i < xx2; i++) {
		for (j = yy1; j < yy2; j++) {
			if (*(pDib1->m_pImgData + j * lineByte + i) == 0)	pcount++;
		}
		if (dirc == 1) {
			if (pcount > Refpcount) Refpcount = pcount;
			if (Refpcount > 2 * pcount) {
				frq++;
				dirc = 0;
				Refpcount = pcount;
			}
			pcount = 0;
		}
		else {
			if (pcount < Refpcount) Refpcount = pcount;
			if (pcount > 2 * Refpcount) {
				frq++;
				dirc = 1;
				Refpcount = pcount;
			}
			pcount = 0;
		}
	}


	if (factor > 2 && factor < 4 && frq>5) {
		for (i = yy1 - 20; i < yy2 + 20; i++) {
			*(pDib3->m_pImgData + i * lineByte + xx1) = 0;
			*(pDib3->m_pImgData + i * lineByte + xx2) = 0;
		}
		for (i = xx1 - 20; i < xx2 + 20; i++) {
			*(pDib3->m_pImgData + yy1 * lineByte + i) = 0;
			*(pDib3->m_pImgData + yy2 * lineByte + i) = 0;
		}
	}


	size.cx = xx2 - xx1 + 10;
	size.cy = yy2 - yy1 + 10;

	int lineByte1 = (size.cx + 3) / 4 * 4;

	unsigned char *pDib_imgdata = new BYTE[size.cy*((size.cx + 3) / 4 * 4)];

	for (i = 0; i < size.cx; i++) {
		for (j = 0; j < size.cy; j++) {
			*(pDib_imgdata + j * lineByte1 + i) = *(pDoc->m_dib_Temp->m_pImgData + (yy1 + j - 5)*lineByte + i + xx1 - 5);
		}
	}

	// 重新复制回保存的结果
	pDib2->ReplaceDib(size, pDib3->m_nBitCount, pDib3->m_lpColorTable, pDib_imgdata);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Oncharseparate()
{
	//获取文档类指针
	CMSMoneyDemoDoc *pDoc = GetDocument();
	//获取ImgCenterDib类对象m_dib的指针，访问当前DIB数据
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;


	//只处理灰度图像
	if (pDib_Res->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	int i, j, thd;
	CSize size = pDib_Res->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	long pixel_scales = 0;

	for (i = 0; i < size.cx; i++) {
		for (j = 0; j < size.cy; j++) {
			pixel_scales += *(pDib_Res->m_pImgData + j * lineByte + i);
		}
	}

	thd = pixel_scales / (size.cx*size.cy);

	GrayTrans graytrans(size, pDib_Res->m_nBitCount, pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.BinaryImage(thd);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	pDib_Res->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();

	//去除上下左边框
	int pixels = 0, curpixels = 0, yy1, yy2, xx1, xx2, flag = 0;

	yy1 = 0;
	yy2 = size.cy - 1;

	for (j = 0; j < size.cx; j++) {
		if (*(pDib_Res->m_pImgData + j) == 255) curpixels++;
	}
	pixels = curpixels;

	for (i = 1; i < size.cy / 2; i++) {
		curpixels = 0;
		for (j = 0; j < size.cx; j++) {
			if (*(pDib_Res->m_pImgData + i * lineByte + j) == 255) curpixels++;
		}
		if (flag == 0) {
			if (curpixels > pixels)	pixels = curpixels;
			if (pixels - curpixels > 0.5*pixels) {
				flag = 1;
				pixels = curpixels;
			}
		}
		else {
			if (curpixels < pixels)	pixels = curpixels;
			if (curpixels - pixels > 0.5*curpixels) {
				yy1 = i;
				flag = 0;
			}
		}
	}

	flag = 0;
	for (j = 0; j < size.cx; j++) {
		if (*(pDib_Res->m_pImgData + (size.cy - 1)*lineByte + j) == 255) curpixels++;
	}
	pixels = curpixels;

	for (i = size.cy - 2; i > size.cy / 2; i--) {
		curpixels = 0;
		for (j = 0; j < size.cx; j++) {
			if (*(pDib_Res->m_pImgData + i * lineByte + j) == 255) curpixels++;
		}
		if (flag == 0) {
			if (curpixels > pixels)	pixels = curpixels;
			if (pixels - curpixels > 0.5*pixels) {
				flag = 1;
				pixels = curpixels;
			}
		}
		else {
			if (curpixels < pixels)	pixels = curpixels;
			if (curpixels - pixels > 0.5*curpixels) {
				yy2 = i;
				flag = 0;
			}
		}
	}

	flag = 0;
	for (i = yy1; i < yy2; i++) {
		if (*(pDib_Res->m_pImgData + i * lineByte) == 255) curpixels++;
	}
	pixels = curpixels;

	for (j = 1; j < size.cx / 10; j++) {
		curpixels = 0;
		for (i = yy1; i < yy2; i++) {
			if (*(pDib_Res->m_pImgData + i * lineByte + j) == 255) curpixels++;
		}
		if (flag == 0) {
			if (curpixels > pixels)	pixels = curpixels;
			if (pixels - curpixels > 0.5*pixels) {
				flag = 1;
				pixels = curpixels;
			}
		}
		else {
			if (curpixels < pixels)	pixels = curpixels;
			if (curpixels - pixels > 0.5*curpixels) {
				xx1 = j;
				flag = 0;
			}
		}
	}

	xx2 = size.cx - 1;


	size.cx = xx2 - xx1 + 1;
	size.cy = yy2 - yy1 + 1;

	int lineByte1 = (size.cx + 3) / 4 * 4;

	unsigned char *pDib_imgdata1 = new BYTE[size.cy*lineByte1];

	for (i = 0; i < size.cx; i++) {
		for (j = 0; j < size.cy; j++) {
			*(pDib_imgdata1 + j * lineByte1 + i) = *(pDib_Res->m_pImgData + (yy1 + j)*lineByte + i + xx1);
		}
	}

	pDib->ReplaceDib(size, pDib_Res->m_nBitCount, pDib_Res->m_lpColorTable, pDib_imgdata1);
	pDib_Res->ReplaceDib(size, pDib->m_nBitCount, pDib->m_lpColorTable, pDib->m_pImgData);

	//字符分割
	int char_x1[7], char_x2[7], char_xx1[7], char_xx2[7], k = 0, char_width, char_gap;
	yy1 = 0;
	yy2 = size.cy;
	xx1 = 0;
	xx2 = size.cx;
	flag = 0;
	lineByte = (size.cx + 3) / 4 * 4;
	curpixels = 0;

	char_x1[k] = 0;

	for (i = 0; i < size.cy; i++) {
		if (*(pDib_Res->m_pImgData + i * lineByte) == 255) curpixels++;
	}
	pixels = curpixels;

	for (j = 1; j < size.cx+20; j++) {// 不知道为什么，在这加了20的扩展，否则后面会出现内存不足
		curpixels = 0;
		for (i = 0; i < size.cy; i++) {
			if (*(pDib_Res->m_pImgData + i * lineByte + j) == 255) curpixels++;
		}

		if (flag == 0) {
			if (curpixels > pixels)	pixels = curpixels;
			if (curpixels < 3) {
				char_x2[k] = j;
				flag = 1;
				pixels = curpixels;
				k++;
				if (k == 7) break;
			}
		}
		else {
			if (curpixels < pixels)	pixels = curpixels;
			if (curpixels > 3) {
				char_x1[k] = j;
				flag = 0;
			}
		}
	}

	char_gap = char_x1[1] - char_x2[0];
	for (i = 2; i < 7; i++) {
		if (char_x1[i] - char_x2[i - 1] < char_gap) char_gap = char_x1[i] - char_x2[i - 1];
	}

	char_width = char_gap + char_x2[0] - char_x1[0] + 1;

	char_xx1[0] = char_x1[0] - char_gap / 2;
	char_xx2[0] = min(char_x2[0] + max(char_gap, char_width - char_x2[0] + char_x1[0]) / 2 - 1, char_x1[1]);

	for (i = 1; i < 6; i++)
	{
		char_xx1[i] = max(char_x1[i] - max(char_gap, char_width - char_x2[i] + char_x1[i]) / 2 - 1, char_xx2[i - 1]);
		char_xx2[i] = min(char_x2[i] + max(char_gap, char_width - char_x2[i] + char_x1[i]) / 2 - 1, char_x1[i + 1]);

	}

	char_xx1[6] = max(char_x1[6] - max(char_gap, char_width - char_x2[6] + char_x1[6]) / 2 - 1, char_xx2[5]);
	char_xx2[6] = char_x2[6] + max(char_gap, char_width - char_x2[6] + char_x1[6]) / 2 - 1;



	CSize size1;
	size1.cx = char_xx2[6] - char_xx1[0] + 1;
	size1.cy = size.cy;
	int lineByte2 = (size1.cx + 3) / 4 * 4;

	unsigned char *pDib_imgdata2 = new BYTE[size1.cy*lineByte2];

	for (i = 0; i < size1.cx; i++) {
		for (j = 0; j < size1.cy; j++) {
			if (i + char_xx1[0] >= 0 && i + char_xx1[0] < size.cx) {
				*(pDib_imgdata2 + j * lineByte2 + i) = *(pDib_Res->m_pImgData + j * lineByte1 + i + char_xx1[0]);
			}
			else {
				*(pDib_imgdata2 + j * lineByte2 + i) = 0;
			}
		}
	}

	pDib_Res->ReplaceDib(size1, pDib->m_nBitCount, pDib->m_lpColorTable, pDib_imgdata2);

	for (j = 0; j < size1.cy; j++) {
		*(pDib_Res->m_pImgData + j * lineByte2 + char_xx2[0] - char_xx1[0]) = 255;
	}
	for (i = 1; i < 6; i++) {
		for (j = 0; j < size1.cy; j++) {
			*(pDib_Res->m_pImgData + j * lineByte2 + char_xx1[i] - char_xx1[0]) = 255;
			*(pDib_Res->m_pImgData + j * lineByte2 + char_xx2[i] - char_xx1[0]) = 255;
		}
	}
	for (j = 0; j < size1.cy; j++) {
		*(pDib_Res->m_pImgData + j * lineByte2 + char_xx1[6] - char_xx1[0]) = 255;
	}

	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();


	//字符归一化


	CSize size2;
	size2.cx = 160;
	size2.cy = 60;
	int lineByte3 = (size2.cx + 3) / 4 * 4;

	unsigned char *pDib_imgdata3 = new BYTE[size2.cy*lineByte3];

	float x_ratio, y_ratio;
	int pnums, x1, y1;

	for (i = 0; i < size2.cx; i++)
		for (j = 0; j < size2.cy; j++)
			*(pDib_imgdata3 + j * lineByte3 + i) = 255;

	for (i = 0; i < 7; i++) {
		x_ratio = (char_xx2[i] - char_xx1[i] + 1) / 20.0;
		y_ratio = size1.cy / 40.0;
		for (j = 0; j < 20; j++) {
			for (k = 0; k < 40; k++) {
				pnums = 0;
				for (x1 = j * x_ratio; x1 < (j + 1)*x_ratio; x1++) {
					for (y1 = k * y_ratio; y1 < (k + 1)*y_ratio; y1++) {
						if (*(pDib_imgdata2 + y1 * lineByte2 + char_xx1[i] + x1) == 255)	pnums++;
					}
				}
				if ((pnums / (x_ratio*y_ratio)) > 0.5) {
					*(pDib_imgdata3 + (k + 10)*lineByte3 + i * 20 + j + 9) = 0;
				}
				else {
					*(pDib_imgdata3 + (k + 10)*lineByte3 + i * 20 + j + 9) = 255;
				}
			}
		}
	}

	pDib->ReplaceDib(size1, pDib_Res->m_nBitCount, pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	pDib_Res->ReplaceDib(size2, pDib->m_nBitCount, pDib->m_lpColorTable, pDib_imgdata3);

	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Oncharrecognition()
{
	//获取文档类指针
	CMSMoneyDemoDoc *pDoc = GetDocument();
	//获取ImgCenterDib类对象m_dib的指针，访问当前DIB数据
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib_Res->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	int i, j, k, lineByte;
	lineByte = (20 * 7 + 20 + 3) / 4 * 4;
	///去噪声
	for (i = 1; i < 159; i++)
		for (j = 1; j < 59; j++)
			if (*(pDib_Res->m_pImgData + j * lineByte + i) == 0) {
				if (*(pDib_Res->m_pImgData + j * lineByte + i - 1) == 255) {
					if (*(pDib_Res->m_pImgData + (j - 1)*lineByte + i) == 255) {
						if (*(pDib_Res->m_pImgData + (j + 1)*lineByte + i) == 255) {
							if (*(pDib_Res->m_pImgData + j * lineByte + i + 1) == 255) {
								*(pDib_Res->m_pImgData + j * lineByte + i) = 255;
							}
						}
					}
				}
			}

	PlateTrans charrecog(pDib_Res->GetDimensions(), pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData, NULL);

	charrecog.charrecognize();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

// 印刷体汉字识别
void CMSMoneyDemoView::OnPaperBinary()
{
	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	int i, j, gray, thd;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;

	double p[256];
	LONG hist[256];
	memset(hist, 0, 256 * 4);//set buffers to a specified characters
	memset(p, 0, 256 * 8);

	for (i = 0; i < pDib->m_imgHeight; i++) {
		for (j = 0; j < pDib->m_imgWidth; j++) {
			// 指向DIB第i行，第j个象素的指针
			gray = *(pDib->m_pImgData + i * lineByte + j);
			hist[gray]++;
		}
	}

	double sum = (double)pDib->m_imgHeight*pDib->m_imgWidth;

	for (i = 0; i < 256; i++) {
		p[i] = (double)hist[i] / sum;
	}

	double u = 0;
	for (i = 0; i < 256; i++) {
		u += i * p[i];
	}

	double ut[256], ot[256], bt[257];
	memset(ut, 0, 256 * 8);
	memset(ot, 0, 256 * 8);
	memset(bt, 0, 257 * 8);
	i = 0;
	do {
		i++;
		for (j = 0; j < i; j++) {
			ut[i] += j * p[j];
			ot[i] += p[j];
		}
		bt[i] = (ut[i] - u * ot[i])*(ut[i] - u * ot[i]) / ot[i] / (1 - ot[i]);
	} while ((i == 1 || (ot[i] - ot[i - 1] >= 0)) && i < 256);

	thd = i - 2;

	GrayTrans graytrans(size, pDib->m_nBitCount, pDib->m_lpColorTable, pDib->m_pImgData);
	//调用Binary()对图像进行二值化，缺省状态下阈值为
	graytrans.BinaryImage(thd);

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	pDib_Res->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Onlocation()
{

	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib_Result;

	//只处理灰度图像
	if (pDib_Res->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	int i, j;

	CSize size = pDib_Res->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;

	charsRecognition charslocation(size, pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);

	charslocation.HprojX(); //行投影
	int lWm = charslocation.LineCluster();  //行合并
	charslocation.MergeProjX(lWm);//虚假行合并
	charslocation.CutImage(lWm);  //文字分割

	charslocation.DrawRect();

	//      单文档显示变换结果
	state2 = 1; // 设置右视图可绘
	CClientDC dc(this);
	pDib_Res->ReplaceDib(size, charslocation.m_nBitCountOut, charslocation.m_lpColorTableOut, charslocation.m_pImgDataOut);
	pDoc->m_dib_SoleProc->ReplaceDib(size, charslocation.m_nBitCountOut, charslocation.m_lpColorTableOut, charslocation.m_pImgDataOut);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoView::Onrecognition()
{
	CMSMoneyDemoDoc *pDoc = GetDocument();
	ImageDib *pDib = pDoc->m_dib;
	ImageDib *pDib_Res = pDoc->m_dib;

	//只处理灰度图像
	if (pDib_Res->m_nBitCount != 8) {
		::MessageBox(0, "只处理灰度图像", MB_OK, 0);
		return;
	}

	int i, j;

	CSize size = pDib_Res->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;

	charsRecognition charslocation(size, pDib_Res->m_nBitCount,
		pDib_Res->m_lpColorTable, pDib_Res->m_pImgData);


	charslocation.HprojX(); //行投影
	int lWm = charslocation.LineCluster();  //行合并
	charslocation.MergeProjX(lWm);//虚假行合并
	charslocation.CutImage(lWm);  //文字分割

	charslocation.BlockNormalization();			//文字归一化
	charslocation.GetCrudeFeature();				//文字特征提取
	charslocation.CrudeMatchChinese();			//识别结果生成


	string strResult = "";											//字符识别结果
	for (list<CFtChinese>::iterator itcc = charslocation.m_fChinese.begin();
		itcc != charslocation.m_fChinese.end(); ++itcc) {
		/////////////////汉字
		//strResult += strIndex[itcc->m_dIndex];
		strResult += itcc->m_iImage[itcc->i_row][itcc->m_dIndex];
	}

	charslocation.DrawRect();

	CResult dlg;
	dlg.m_str.Format("%s", strResult.c_str());
	//dlg.m_str.Format("%s",strResult);
	dlg.DoModal();
}

// 运动图像分析
void CMSMoneyDemoView::OnObjectDetect()
{

	int i, j;
	CMSMoneyDemoDoc *pdoc = GetDocument();

	unsigned char *frame = pdoc->m_avi->GetSingleFrame(0);
	int frameWidth = pdoc->m_avi->GetFrameWidth();
	int frameHeight = pdoc->m_avi->GetFrameHeight();
	int frameCount = pdoc->m_avi->GetFrameCount();

	ChafenMul cm(frameWidth, frameHeight, 200);
	// 输出缓冲
	unsigned char *outBuffer = new unsigned char[frameHeight*frameWidth * 3];
	unsigned char *gray = new unsigned char[frameWidth*frameHeight];

	for (i = 0; i < frameCount - 1; i++)
	{
		// 获得一帧图像	
		frame = pdoc->m_avi->GetSingleFrame(i);
		int k = 0;
		for (j = 0; j < frameHeight*frameWidth; j++) {
			gray[j] = (unsigned char)((long)((double)frame[k + 2])*0.299 + (long)((double)frame[k + 1])*0.587 +
				(long)((double)frame[k])*0.114);
			k = k + 3;
		}
		cm.PrepareData(gray, frameWidth, frameHeight);
		if (i % 4 == 0 && i != 0) {
			cm.process();
			for (j = 0; j < frameHeight*frameWidth; j++) {
				outBuffer[j * 3] = cm.frame1[j];
				outBuffer[j * 3 + 1] = cm.frame1[j];
				outBuffer[j * 3 + 2] = cm.frame1[j];
			}
			pdoc->Avidata = outBuffer;

			CClientDC pDC(this);
			BITMAPINFO bitmapInfo;
			bitmapInfo.bmiHeader.biHeight = (LONG)frameHeight;
			bitmapInfo.bmiHeader.biWidth = (LONG)frameWidth;
			bitmapInfo.bmiHeader.biSizeImage = (LONG)frameWidth * (LONG)frameHeight * 3;
			bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bitmapInfo.bmiHeader.biBitCount = 24;
			bitmapInfo.bmiHeader.biCompression = 0;
			bitmapInfo.bmiHeader.biPlanes = 1;

			pDC.SetStretchBltMode(STRETCH_DELETESCANS);
			StretchDIBits(pDC.m_hDC, 0, 0, frameWidth, frameHeight, 0, 0, frameWidth, frameHeight,
				outBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

			// 强制刷新窗体
			CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
			pFrame->InvalidateRect(NULL, FALSE);
			pFrame->UpdateWindow();

		}
	}

	// 释放内存
	if (outBuffer != NULL)
	{
		delete []outBuffer;
		outBuffer = NULL;
	}

	if (gray != NULL)
	{
		delete[]gray;
		gray = NULL;
	}

}

void CMSMoneyDemoView::OnObjectTrace()
{

	CMSMoneyDemoDoc *pdoc = GetDocument();
	int frameLength = pdoc->m_avi->GetFrameCount();
	int frameWidth = pdoc->m_avi->GetFrameWidth();
	int frameHeight = pdoc->m_avi->GetFrameHeight();
	int x, y, width, height;
	x = 248;
	y = 70;
	width = 20;
	height = 65;
	MeanShiftSegger *mss;
	mss = new MeanShiftSegger();
	unsigned char *frame = pdoc->m_avi->GetSingleFrame(0);
	mss->InitMeanShiftTracker(frame, frameWidth, frameHeight, x, y, width, height);
	for (int i = 0; i < frameLength - 1; i++) {
		frame = pdoc->m_avi->GetSingleFrame(i);		// 获得一帧图像	
		mss->MeanShiftTrackProcess(frame, i);
		pdoc->Avidata = frame;
		CClientDC pDC(this);
		BITMAPINFO bitmapInfo;
		bitmapInfo.bmiHeader.biHeight = (LONG)frameHeight;
		bitmapInfo.bmiHeader.biWidth = (LONG)frameWidth;
		bitmapInfo.bmiHeader.biSizeImage = (LONG)frameWidth*(LONG)frameHeight * 3;
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biBitCount = 24;
		bitmapInfo.bmiHeader.biCompression = 0;
		bitmapInfo.bmiHeader.biPlanes = 1;
		pDC.SetStretchBltMode(STRETCH_DELETESCANS);
		StretchDIBits(pDC.m_hDC, 0, 0, frameWidth, frameHeight, 0, 0, frameWidth, frameHeight,
			frame, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		// 强制刷新窗体
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->InvalidateRect(NULL, FALSE);
		pFrame->UpdateWindow();

	}

	// 释放内存
	if (mss != NULL)
	{
		delete mss;
		mss = 0;
	}
}




/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoView printing

BOOL CMSMoneyDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMSMoneyDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMSMoneyDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoView diagnostics

#ifdef _DEBUG
void CMSMoneyDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CMSMoneyDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMSMoneyDemoDoc* CMSMoneyDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMSMoneyDemoDoc)));
	return (CMSMoneyDemoDoc*)m_pDocument;
}
#endif //_DEBUG
