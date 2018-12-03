// VideoDemoView.cpp : implementation of the CVideoDemoView class
//

#include "stdafx.h"
#include "VideoDemo.h"

#include "VideoDemoDoc.h"
#include "VideoDemoView.h"
#include "AVIHandle.h"
#include "chafenMul.h"
#include "MeanShiftSegger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoView

IMPLEMENT_DYNCREATE(CVideoDemoView, CView)

BEGIN_MESSAGE_MAP(CVideoDemoView, CView)
	//{{AFX_MSG_MAP(CVideoDemoView)
	ON_COMMAND(id_ObjectDetect, OnObjectDetect)
	ON_COMMAND(id_ObjectTrace, OnObjectTrace)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoView construction/destruction

CVideoDemoView::CVideoDemoView()
{
	// TODO: add construction code here

}

CVideoDemoView::~CVideoDemoView()
{
}

BOOL CVideoDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoView drawing

void CVideoDemoView::OnDraw(CDC* pDC)
{
	CVideoDemoDoc *pdoc = GetDocument();
	if(pdoc->isAvi){
		int frameWidth=pdoc->m_avi->GetFrameWidth();
		int frameHeight=pdoc->m_avi->GetFrameHeight();
		BITMAPINFO bitmapInfo;
		bitmapInfo.bmiHeader.biHeight =(LONG)frameHeight;
		bitmapInfo.bmiHeader.biWidth = (LONG)frameWidth;
		bitmapInfo.bmiHeader.biSizeImage = (LONG)frameWidth * (LONG)frameHeight* 3;
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biBitCount = 24;
		bitmapInfo.bmiHeader.biCompression = 0;
		bitmapInfo.bmiHeader.biPlanes = 1;

		pDC->SetStretchBltMode(STRETCH_DELETESCANS);
		StretchDIBits(pDC->m_hDC,0,0,frameWidth,frameHeight,0,0,frameWidth,frameHeight,
			pdoc->Avidata, &bitmapInfo, DIB_RGB_COLORS,SRCCOPY);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoView printing

BOOL CVideoDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVideoDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVideoDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoView diagnostics

#ifdef _DEBUG
void CVideoDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CVideoDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVideoDemoDoc* CVideoDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVideoDemoDoc)));
	return (CVideoDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVideoDemoView message handlers

void CVideoDemoView::OnObjectDetect() 
{

	int i,j;
	CVideoDemoDoc *pdoc = GetDocument();

	unsigned char *frame=pdoc->m_avi->GetSingleFrame(0);
	int frameWidth=pdoc->m_avi->GetFrameWidth();
	int frameHeight=pdoc->m_avi->GetFrameHeight();
	int frameCount=pdoc->m_avi->GetFrameCount();
	
	ChafenMul cm(frameWidth,frameHeight,200);
	// 输出缓冲
	unsigned char *outBuffer=new unsigned char[frameHeight*frameWidth*3];
	unsigned char *gray=new unsigned char[frameWidth*frameHeight];
	
	for(i=0;i<frameCount-1;i++)
	{
		// 获得一帧图像	
		frame=pdoc->m_avi->GetSingleFrame(i);
		int k = 0;
		for(j=0;j<frameHeight*frameWidth;j++){
	 		gray[j]=(unsigned char)((long)((double)frame[k+2])*0.299+(long)((double)frame[k+1])*0.587+
				(long)((double)frame[k])*0.114);
			k=k+3;
		}
		cm.PrepareData(gray,frameWidth,frameHeight);
		if(i%4==0&&i!=0){
			cm.process();
			for(j=0;j<frameHeight*frameWidth;j++){
	 			outBuffer[j*3]=cm.frame1[j];
				outBuffer[j*3+1]=cm.frame1[j];
				outBuffer[j*3+2]=cm.frame1[j];
			}
			pdoc->Avidata = outBuffer;

			CClientDC pDC(this);
			BITMAPINFO bitmapInfo;
			bitmapInfo.bmiHeader.biHeight =(LONG)frameHeight;
			bitmapInfo.bmiHeader.biWidth = (LONG)frameWidth;
			bitmapInfo.bmiHeader.biSizeImage = (LONG)frameWidth * (LONG)frameHeight* 3;
			bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bitmapInfo.bmiHeader.biBitCount = 24;
			bitmapInfo.bmiHeader.biCompression = 0;
			bitmapInfo.bmiHeader.biPlanes = 1;
			
			pDC.SetStretchBltMode(STRETCH_DELETESCANS);
			StretchDIBits(pDC.m_hDC,0,0,frameWidth,frameHeight,0,0,frameWidth,frameHeight,
				outBuffer, &bitmapInfo, DIB_RGB_COLORS,SRCCOPY);
		}
	}
}

void CVideoDemoView::OnObjectTrace() 
{

	CVideoDemoDoc *pdoc=GetDocument();
	int frameLength=pdoc->m_avi->GetFrameCount();
	int frameWidth=pdoc->m_avi->GetFrameWidth();
	int frameHeight=pdoc->m_avi->GetFrameHeight();
	int x,y,width,height;
	x=248;
	y=70;
	width=20;
	height=65;
	MeanShiftSegger *mss;
	mss=new MeanShiftSegger();
	unsigned char *frame=pdoc->m_avi->GetSingleFrame(0);
	mss->InitMeanShiftTracker(frame,frameWidth,frameHeight,x,y,width,height);
	for(int i=0;i<frameLength-1;i++){
		frame=pdoc->m_avi->GetSingleFrame(i);		// 获得一帧图像	
		mss->MeanShiftTrackProcess(frame,i);
		pdoc->Avidata=frame;
		CClientDC pDC(this);
		BITMAPINFO bitmapInfo;
		bitmapInfo.bmiHeader.biHeight=(LONG)frameHeight;
		bitmapInfo.bmiHeader.biWidth=(LONG)frameWidth;
		bitmapInfo.bmiHeader.biSizeImage=(LONG)frameWidth*(LONG)frameHeight* 3;
		bitmapInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biBitCount=24;
		bitmapInfo.bmiHeader.biCompression=0;
		bitmapInfo.bmiHeader.biPlanes=1;
		pDC.SetStretchBltMode(STRETCH_DELETESCANS);
		StretchDIBits(pDC.m_hDC,0,0,frameWidth,frameHeight,0,0,frameWidth,frameHeight,
			frame, &bitmapInfo, DIB_RGB_COLORS,SRCCOPY);
	}
}
