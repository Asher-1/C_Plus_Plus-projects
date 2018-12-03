
// FeatureReuseView.cpp : CFeatureReuseView 类的实现
//

#include "stdafx.h"
#include "FeatureReuse.h"
#include "FeatureReuseDoc.h"
#include "FeatureReuseView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CFeatureReuseView
IMPLEMENT_DYNCREATE(CFeatureReuseView, CGLView)

BEGIN_MESSAGE_MAP(CFeatureReuseView, CGLView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFeatureReuseView::OnFilePrintPreview)

	ON_COMMAND(ID_VIEW_BACK, OnViewBack)
	ON_COMMAND(ID_VIEW_BOTTOM, OnViewBottom)
	ON_COMMAND(ID_VIEW_FRONT, OnViewFront)
	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_COMMAND(ID_VIEW_TOP, OnViewTop)
	ON_COMMAND(ID_VIEW_SW_ISOMETRIC, OnViewSWIsometric)
	ON_COMMAND(ID_VIEW_SE_ISOMETRIC, OnViewSEIsometric)
	ON_COMMAND(ID_VIEW_NE_ISOMETRIC, OnViewNEIsometric)
	ON_COMMAND(ID_VIEW_NW_ISOMETRIC, OnViewNWIsometric)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_VIEW_ZOOMALL, OnViewZoomall)
	ON_COMMAND(ID_VIEW_SHADE, OnViewShade)

	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CFeatureReuseView 构造/析构

CFeatureReuseView::CFeatureReuseView()
{
	// TODO: 在此处添加构造代码
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_Scale = 1.0f;

}

CFeatureReuseView::~CFeatureReuseView()
{
}

BOOL CFeatureReuseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

int CFeatureReuseView::time = 200;

void CFeatureReuseView::myIdle(void)
{
	++time;
	if (time >= 360)
		time = 0;
	//Redraw the view
	InvalidateRect(NULL, FALSE);
}


// 用于屏幕刷新帧率的显示FPS
double CFeatureReuseView::CalFrequency()
{
	static int count;
	static double save;
	static clock_t last, current;
	double timegap;

	++count;
	if (count <= 50)
		return save;
	count = 0;
	last = current;
	current = clock();
	timegap = (current - last) / (double)CLK_TCK;
	save = 50.0 / timegap;
	return save;
}

void CFeatureReuseView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (CMainFrame::m_Mode == 4) // 绘制模式是动态显示
	{
	
		CMainFrame::m_delta = CMainFrame::m_delta + (CFeatureReuseDoc::steps)/6;

		if (CMainFrame::m_delta > CFeatureReuseDoc::steps - 1)
		{
			CMainFrame::m_delta = CFeatureReuseDoc::steps - 1;
		}
		else
			CMainFrame::m_Counts = CMainFrame::m_Counts + 1;

		InvalidateRect(NULL, FALSE);  // 强制刷新场景
	}

	CGLView::OnTimer(nIDEvent);
}

int CFeatureReuseView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGLView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
		SetTimer(0, 1000, NULL); // 设置计时器，且间隔时间为1s刷新一次

	return 0;
}

// CFeatureReuseView 绘制
void CFeatureReuseView::RenderScene(COpenGLDC* pDC)
{
	CFeatureReuseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 动态设置缩放比例
	pDoc->SetScale(m_Scale);
	// 动态设置旋转角度
	pDoc->SetMouseRotate(m_xAngle, m_yAngle);
	// 设置绘制模型的材质颜色属性
	pDC->SetMaterialColor(CMainFrame::m_Color); 

	// 设置绘制模式
	pDoc->SetMode(CMainFrame::m_Mode);

	// 设置绘制矢量箭头模式
	pDoc->SetArrows(CMainFrame::is_PrintArrows);

	// 设置绘制彩色云图模式
	pDoc->Set_cloudPicture(CMainFrame::is_Draw_cloudPicture);	

	// 设置模型显示阶段
	pDoc->SetStage(CMainFrame::m_stage);

	// 设置模型动态显示循环变量
	pDoc->SetDelta(CMainFrame::m_delta);
	pDoc->SetCounter(CMainFrame::m_Counts); // 待测试

	// 绘制坐标轴
	pDC->DrawCoord();

	// 绘制模型
	pDoc->DrawModel(pDC);

}

void CFeatureReuseView::OnInitialUpdate()
{
	CGLView::OnInitialUpdate();
	OnViewType(VIEW_SW_ISOMETRIC);
	ZoomAll();
}
// CFeatureReuseView 打印

void CFeatureReuseView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CFeatureReuseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFeatureReuseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFeatureReuseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CFeatureReuseView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFeatureReuseView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

// CFeatureReuseView 诊断

#ifdef _DEBUG
void CFeatureReuseView::AssertValid() const
{
	CView::AssertValid();
}

void CFeatureReuseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFeatureReuseDoc* CFeatureReuseView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFeatureReuseDoc)));
	return (CFeatureReuseDoc*)m_pDocument;
}
#endif //_DEBUG

// CFeatureReuseView 消息处理程序
void CFeatureReuseView::OnSize(UINT nType, int cx, int cy)
{
	CGLView::OnSize(nType, cx, cy);
	ZoomAll();
}
//
BOOL CFeatureReuseView::GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1)
{
	CFeatureReuseDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	if(pDoc->meshes.size()>0)
	{
		x0 = (pDoc->getbox()).min[0]; 
		y0 = (pDoc->getbox()).min[1]; 
		z0 = (pDoc->getbox()).min[2]; 
		x1 = (pDoc->getbox()).max[0]; 
		y1 = (pDoc->getbox()).max[1];   
		z1 = (pDoc->getbox()).max[2]; 
		return TRUE;
	}

	return FALSE;
}
//


void CFeatureReuseView::OnViewBack()
{
	OnViewType(VIEW_BACK); // 模型的后视图
}

void CFeatureReuseView::OnViewBottom()
{
	OnViewType(VIEW_BOTTOM); // 模型的底视图
}

void CFeatureReuseView::OnViewFront()
{
	OnViewType(VIEW_FRONT); // 模型的前视图
}

void CFeatureReuseView::OnViewLeft()
{
	OnViewType(VIEW_LEFT); // 模型的左视图
}

void CFeatureReuseView::OnViewRight()
{
	OnViewType(VIEW_RIGHT); // 模型的右视图
}

void CFeatureReuseView::OnViewTop()
{
	OnViewType(VIEW_TOP); // 模型的顶视图
}

void CFeatureReuseView::OnViewSWIsometric()
{
	OnViewType(VIEW_SW_ISOMETRIC); // 模型的SW侧视图
}

void CFeatureReuseView::OnViewSEIsometric()
{
	OnViewType(VIEW_SE_ISOMETRIC); // 模型的SE侧视图
}

void CFeatureReuseView::OnViewNEIsometric()
{
	OnViewType(VIEW_NE_ISOMETRIC); // 模型的NE侧视图
}

void CFeatureReuseView::OnViewNWIsometric()
{
	OnViewType(VIEW_NW_ISOMETRIC); // 模型的NW侧视图
}

void CFeatureReuseView::OnViewZoomall()
{
	ZoomAll(); // 将模型显示在整个窗口内
}

void CFeatureReuseView::OnViewZoomin()
{
	Zoom(0.9); // 将视景区域缩小为原来的0.9，模型在屏幕上的实际显示尺寸放大
}

void CFeatureReuseView::OnViewZoomout()
{
	Zoom(1.1); // 将视景区域放大至1.1倍，模型在屏幕上的实际显示尺寸缩小
}

void CFeatureReuseView::OnViewShade()
{
	m_pGLDC->Shading(!m_pGLDC->IsShading()); // 打开或关闭OpenGL的着色处理
	Invalidate();
}

void CFeatureReuseView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_UP:
		MoveView(0.0, 0.04); // 向上移动场景4%
		break;
	case VK_DOWN:
		MoveView(0.0, -0.04); // 向下移动场景4%
		break;
	case VK_RIGHT:
		MoveView(0.04, 0); // 向右移动场景4%
		break;
	case VK_LEFT:
		MoveView(-0.04, 0); // 向左移动场景4%
		break;
	}

	CGLView::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CFeatureReuseView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MouseDownPoint = point;
	SetCapture();

	CGLView::OnLButtonDown(nFlags, point);
}

void CFeatureReuseView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MouseDownPoint = CPoint(0, 0);
	ReleaseCapture();

	CGLView::OnLButtonUp(nFlags, point);
}

void CFeatureReuseView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CGLView::OnMButtonDown(nFlags, point);
}

void CFeatureReuseView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_CONTROL)
	{
		CGLView::OnMouseMove(nFlags, point);
		return;
	}

	if (GetCapture() == this)
	{
		//Increment the object rotation angles
		m_xAngle += (point.y - m_MouseDownPoint.y) / 3.6;
		m_yAngle += (point.x - m_MouseDownPoint.x) / 3.6;

		//Redraw the view
		InvalidateRect(NULL, FALSE);

		//Set the mouse point
		m_MouseDownPoint = point;
	};

	CGLView::OnMouseMove(nFlags, point);
}

BOOL CFeatureReuseView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_Scale += 0.8 *zDelta / 120; // 当鼠标滚轮转动时，改变比例值，模型大小按此比例进行缩放


	InvalidateRect(NULL, FALSE);
	return CGLView::OnMouseWheel(nFlags, zDelta, pt);
}



