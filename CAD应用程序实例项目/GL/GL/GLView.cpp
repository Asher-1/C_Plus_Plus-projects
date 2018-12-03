// GLView.cpp : implementation of the CGLView class
//

#include "stdafx.h"
#include "GL.h"

#include "GLDoc.h"
#include "GLView.h"

//#define GLUT_DISABLE_ATEXIT_HACK
//#include <gl\glut.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLView

IMPLEMENT_DYNCREATE(CGLView, CView)

BEGIN_MESSAGE_MAP(CGLView, CView)
	//{{AFX_MSG_MAP(CGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLView construction/destruction

CGLView::CGLView()
{
	m_hDC = NULL;
	m_hRC = NULL;
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_Scale = 1.0f;

}

CGLView::~CGLView()
{
}

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{

	//  Add Window style required for OpenGL before window is created
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CS_OWNDC;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGLView drawing

void CGLView::OnDraw(CDC* pDC)
{
	wglMakeCurrent(m_hDC,m_hRC); // 当前化渲染场境

	RenderScene(); // 调用虚拟函数，具体绘制OpenGL图形

	SwapBuffers(m_hDC); // 交换帧存

	wglMakeCurrent(m_hDC,NULL); // 非当前化渲染场境
}

/////////////////////////////////////////////////////////////////////////////
// CGLView diagnostics

#ifdef _DEBUG

CGLDoc* CGLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLDoc)));
	return (CGLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	GLInit();// 视图窗口创建完成后，进行OpenGL Windows的初始化设置

	// 该函数为系统设置一个序号为0的定时器
	//SetTimer(0,50,NULL); // 每隔50ms发出消息WM_TIMER

	return 0;
}



BOOL CGLView::OnEraseBkgnd(CDC* pDC) // 窗口背景擦除
{
	//return CView::OnEraseBkgnd(pDC);
	return FALSE; // 屏蔽掉MFC的窗口擦除，避免两次背景擦除，造成屏幕闪动效果
}

void CGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	VERIFY(wglMakeCurrent(m_hDC,m_hRC));

	GLResize(cx,cy); // 用于处理窗口尺寸的变化

	VERIFY(wglMakeCurrent(NULL,NULL));
}

void CGLView::GLInit()
{
	m_hDC = ::GetDC(m_hWnd); // Get the Device context 获取一个设备场境
	ASSERT(m_hDC);

	static	PIXELFORMATDESCRIPTOR pfdWnd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Structure size.
		1,                             // Structure version number.
		PFD_DRAW_TO_WINDOW |           // Property flags.像素缓存属性
		PFD_SUPPORT_OPENGL |
 		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,			        // 设置颜色模式(备选:PFD_TYPE_COLORINDEX,即索引颜色模式)
		24,                            // 24-bit color.24位真彩色
		0, 0, 0, 0, 0, 0,              // Not concerned with these.
		0, 0, 0, 0, 0, 0, 0,           // No alpha or accum buffer.
		32,                            // 32-bit depth buffer.深度缓存位数
		0, 0,                          // No stencil or aux buffer.
		PFD_MAIN_PLANE,                // Main layer type.
		0,                             // Reserved.
		0, 0, 0                        // Unsupported.
	};

	int pixelformat;
	// 返回一个整型的序号，该序号表示与所要求的像素最为匹配的像素格式
    pixelformat = ChoosePixelFormat(m_hDC, &pfdWnd); // 为设备场境选择像素格式

    ASSERT(SetPixelFormat(m_hDC, pixelformat, &pfdWnd));// 为指定的设备场境设置像素格式

	m_hRC=wglCreateContext(m_hDC); // 创建与设备场境关联的渲染场境

	//make the rending context current, perform initialization
	VERIFY(wglMakeCurrent(m_hDC,m_hRC));// 选择渲染场境m_hRc为当前场境

	GLSetupRC(); // 初始化渲染场境 

	//deselect it
	VERIFY(wglMakeCurrent(NULL,NULL));// 关闭渲染场境
}


void CGLView::GLResize(int cx,int cy)
{
	double nRange = 1200.0;

	// Prevent a divide by zero
	if(cy == 0)
		cy = 1;

	// Set Viewport to window dimensions 设置视口
    glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION); // 切换当前矩阵为投影矩阵

	// Reset coordinate system 初始化矩阵
	glLoadIdentity(); 

	// Establish clipping volume (left, right, bottom, top, near, far) 
	// 设置视景体大小
	if(cx <= cy)
		glOrtho(-nRange,nRange,-nRange*cy/cx,nRange*cy/cx,nRange,-nRange);
	else
		glOrtho(-nRange*cx/cy,nRange*cx/cy,-nRange,nRange,nRange,-nRange);


	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// 设置视点和方向
	double eye[3], ref[3], up_dir[3];
	eye[0] = 0; eye[1] = 0; eye[2] = nRange;
	ref[0] = 0; ref[1] = 0; ref[2] = 0;
	up_dir[0] = 0; up_dir[1] = 1; up_dir[2] = 0;

	gluLookAt(eye[0], eye[1], eye[2], ref[0], ref[1], ref[2],
		up_dir[0], up_dir[1], up_dir[2]);

}

void CGLView::GLSetupRC()// 初始化渲染场境函数
{
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glEnable(GL_COLOR_MATERIAL);

	// Lighting components 光源设置
	GLfloat  ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat  diffuseLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat	 lightPos[] = { 1.0f, 1.0f, -1.0f, 0.0f };

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);

	// 材料设置
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR,specular);
	glMateriali(GL_FRONT,GL_SHININESS,100);

	// 背景颜色设置（黑色）
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f ); //background color

	// default color 设置材料的默认颜色（黑色）
	glColor3ub(0, 0, 255);

}

void CGLView::RenderScene() // OpenGL图形绘制函数
{
	// 清楚颜色缓存和深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rotate
	glRotatef(3.0f,0.0f, 0.0f, 1.0f); // Rock Z 绕z轴旋转3°
	glRotatef(3.0f,1.0f, 0.0f, 0.0f); // Roll X 绕x轴旋转3°

	glPushMatrix();
	glColor3ub(255,0,0);//red
	auxSolidSphere(360); //0,0,0

	glColor3ub(0,0,255);//blue
	glTranslatef(600,0,200);//600,0,0
	auxSolidSphere(120);

	glColor3ub(0,255,0);//green
	glTranslatef(-1200,600,0);//-600,0,0
	auxSolidSphere(240);

	glPopMatrix();
	glFlush();

}


void CGLView::OnTimer(UINT nIDEvent) 
{
	Invalidate(FALSE);	// 刷新屏幕（每秒钟刷新屏幕20次）
	CView::OnTimer(nIDEvent);
}

void CGLView::OnDestroy()
{
	GLRelease();
	CView::OnDestroy();
}

//clear up rendering context
void CGLView::GLRelease()
{
	wglDeleteContext(m_hRC);
	::ReleaseDC(m_hWnd, m_hDC);
}


void CGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MouseDownPoint = CPoint(0, 0);
	ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}


void CGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_MouseDownPoint = point;
	SetCapture();

	CView::OnLButtonDown(nFlags, point);
}


void CGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (nChar)
	{
	case VK_UP:        m_yPos = m_yPos + 0.1f;
		break;
	case VK_DOWN:    m_yPos = m_yPos - 0.1f;
		break;
	case VK_LEFT:    m_xPos = m_xPos - 0.1f;
		break;
	case VK_RIGHT:  m_xPos = m_xPos + 0.1f;
		break;
	default:        //MessageBox(_T("Press the arrow keys only"));
		break;
	}

	InvalidateRect(NULL, FALSE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CGLView::OnMouseMove(UINT nFlags, CPoint point)
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

	CView::OnMouseMove(nFlags, point);
}


BOOL CGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_Scale += 0.01 *zDelta / 120;
	InvalidateRect(NULL, FALSE);


	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
