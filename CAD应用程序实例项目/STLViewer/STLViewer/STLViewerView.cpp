// STLViewerView.cpp : implementation of the CSTLViewerView class
//

#include "stdafx.h"
#include "STLViewer.h"

#include "MainFrm.h"

#include "STLViewerDoc.h"
#include "STLViewerView.h"
#include <iostream>

#include "tetgen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerView

IMPLEMENT_DYNCREATE(CSTLViewerView, CGLView)

BEGIN_MESSAGE_MAP(CSTLViewerView, CGLView)
	//{{AFX_MSG_MAP(CSTLViewerView)
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
	ON_COMMAND(ID_VIEW_ZOOMALL, OnViewZoomall)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_VIEW_SHADE, OnViewShade)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerView construction/destruction

CSTLViewerView::CSTLViewerView()
{
	// TODO: add construction code here
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_Scale = 1.0f;
}

CSTLViewerView::~CSTLViewerView()
{
}

BOOL CSTLViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerView drawing
/////////////////////////////////////////////////////////////////////////////
// CSTLViewerView diagnostics

#ifdef _DEBUG
void CSTLViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CSTLViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSTLViewerDoc* CSTLViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSTLViewerDoc)));
	return (CSTLViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSTLViewerView message handlers
void CSTLViewerView::RenderScene(COpenGLDC* pDC)
{
	CSTLViewerDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	// 绘制一个坐标系框架
	pDC->DrawCoord();

	// 设置pSTLModel模型对象的旋转角度和随着鼠标的缩放比例
	if (!pDoc->m_Part.IsEmpty()) {// 如果模型m_Part不为空

		for (int i = 0;i< pDoc->m_Part.GetEntitySize();i++)
		{// 循环模型实现，实现多模型同步缩放大小和旋转模型
			CSTLModel* s = (CSTLModel*)(pDoc->m_Part.GetEntity(i));
			s->SetScale(m_Scale);
			s->SetMouseRo(m_xAngle, m_yAngle);
			s->SetMode(CMainFrame::m_Model); // 设置绘制模式
		}

		pDoc->m_Part.Draw(pDC); // 用OpenGL绘制模型m_Part
	}

	//glLoadIdentity();
	//glTranslatef(m_xPos, m_yPos, -5.0f);
	//glRotatef(m_xAngle, 1.0f, 0.0f, 0.0f);
	//glRotatef(m_yAngle, 0.0f, 1.0f, 0.0f);
	//glScalef(m_Scale, m_Scale, m_Scale);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	////Front Face
	//glBegin(GL_POLYGON);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(-1.0f, -1.0f, 0.0f);
	//glColor3f(1.0f, 1.0f, 0.0f);
	//glVertex3f(1.0f, -1.0f, 0.0f);
	//glColor3f(1.0f, 0.0f, 1.0f);
	//glVertex3f(1.0f, 1.0f, 0.0f);
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(-1.0f, 1.0f, 0.0f);
	//glEnd();
	//glColor3f(1.0f, 1.0f, 0.0f);
	////Back Face
	//glBegin(GL_POLYGON);
	//glVertex3f(-1.0f, -1.0f, -1.0f);
	//glVertex3f(-1.0f, 1.0f, -1.0f);
	//glVertex3f(1.0f, 1.0f, -1.0f);
	//glVertex3f(1.0f, -1.0f, -1.0f);
	//glEnd();
	//glColor3f(1.0f, 0.0f, 1.0f);
	////Left Face
	//glBegin(GL_POLYGON);
	//glVertex3f(-1.0f, -1.0f, 0.0f);
	//glVertex3f(-1.0f, 1.0f, 0.0f);
	//glVertex3f(-1.0f, 1.0f, -1.0f);
	//glVertex3f(-1.0f, -1.0f, -1.0f);
	//glEnd();
	//glColor3f(0.0f, 1.0f, 0.0f);
	////Right Face
	//glBegin(GL_POLYGON);
	//glVertex3f(1.0f, -1.0f, 0.0f);
	//glVertex3f(1.0f, -1.0f, -1.0f);
	//glVertex3f(1.0f, 1.0f, -1.0f);
	//glVertex3f(1.0f, 1.0f, 0.0f);
	//glEnd();
	//glColor3f(0.0f, 1.0f, 1.0f);
	////Top Face
	//glBegin(GL_POLYGON);
	//glVertex3f(-1.0f, 1.0f, 0.0f);
	//glVertex3f(1.0f, 1.0f, 0.0f);
	//glVertex3f(1.0f, 1.0f, -1.0f);
	//glVertex3f(-1.0f, 1.0f, -1.0f);
	//glEnd();
	//glColor3f(0.0f, 0.0f, 1.0f);
	////Botton Face
	//glBegin(GL_POLYGON);
	//glVertex3f(-1.0f, -1.0f, 0.0f);
	//glVertex3f(-1.0f, -1.0f, -1.0f);
	//glVertex3f(1.0f, -1.0f, -1.0f);
	//glVertex3f(1.0f, -1.0f, 0.0f);
	//glEnd();

}

BOOL CSTLViewerView::GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1)
{
	CSTLViewerDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	if(!pDoc->m_Part.IsEmpty()){//如果模型不为空，提取包容盒信息
		CBox3D box;
		if(pDoc->m_Part.GetBox(box)){
			x0 = box.x0; y0 = box.y0;	z0 = box.z0;
			x1 = box.x1; y1 = box.y1;   z1 = box.z1;
			return TRUE;
		}
	}

	return FALSE; // 模型为空
}

void CSTLViewerView::OnViewBack() 
{
	OnViewType(VIEW_BACK); // 模型的后视图
}

void CSTLViewerView::OnViewBottom() 
{
	OnViewType(VIEW_BOTTOM); // 模型的底视图
}

void CSTLViewerView::OnViewFront() 
{
	OnViewType(VIEW_FRONT); // 模型的前视图
}

void CSTLViewerView::OnViewLeft() 
{
	OnViewType(VIEW_LEFT); // 模型的左视图
}

void CSTLViewerView::OnViewRight() 
{
	OnViewType(VIEW_RIGHT); // 模型的右视图
}

void CSTLViewerView::OnViewTop() 
{
	OnViewType(VIEW_TOP); // 模型的顶视图
}

void CSTLViewerView::OnViewSWIsometric()
{
	OnViewType(VIEW_SW_ISOMETRIC); // 模型的SW侧视图
}

void CSTLViewerView::OnViewSEIsometric()
{
	OnViewType(VIEW_SE_ISOMETRIC); // 模型的SE侧视图
}

void CSTLViewerView::OnViewNEIsometric()
{
	OnViewType(VIEW_NE_ISOMETRIC); // 模型的NE侧视图
}

void CSTLViewerView::OnViewNWIsometric()
{
	OnViewType(VIEW_NW_ISOMETRIC); // 模型的NW侧视图
}

void CSTLViewerView::OnViewZoomall() 
{
	ZoomAll(); // 将模型显示在整个窗口内
}

void CSTLViewerView::OnViewZoomin() 
{
	Zoom(0.9); // 将视景区域缩小为原来的0.9，模型在屏幕上的实际显示尺寸放大
}

void CSTLViewerView::OnViewZoomout() 
{
	Zoom(1.1); // 将视景区域放大至1.1倍，模型在屏幕上的实际显示尺寸缩小
}

void CSTLViewerView::OnViewShade() 
{
	m_pGLDC->Shading(!m_pGLDC->IsShading()); // 打开或关闭OpenGL的着色处理
	Invalidate();
}

void CSTLViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar){
	case VK_UP:
		MoveView(0.0,0.02); // 向上移动场景2%
		break;
	case VK_DOWN:
		MoveView(0.0,-0.02); // 向下移动场景2%
		break;
	case VK_RIGHT:
		MoveView(0.02,0); // 向右移动场景2%
		break;
	case VK_LEFT:
		MoveView(-0.02,0); // 向左移动场景2%
		break;
	}
	
	CGLView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSTLViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MouseDownPoint = point;
	SetCapture();

	CGLView::OnLButtonDown(nFlags, point);
}


void CSTLViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MouseDownPoint = CPoint(0, 0);
	ReleaseCapture();

	CGLView::OnLButtonUp(nFlags, point);
}


void CSTLViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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


BOOL CSTLViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_Scale += 0.1 *zDelta / 120; // 当鼠标滚轮转动时，改变比例值，模型大小按此比例进行缩放

	InvalidateRect(NULL, FALSE);
	return CGLView::OnMouseWheel(nFlags, zDelta, pt);
}
