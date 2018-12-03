// DrawCubeDemoView.cpp : implementation of the CDrawCubeDemoView class
//

#include "stdafx.h"
#include "DrawCubeDemo.h"

#include "DrawCubeDemoDoc.h"
#include "DrawCubeDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoView

IMPLEMENT_DYNCREATE(CDrawCubeDemoView, CView)

BEGIN_MESSAGE_MAP(CDrawCubeDemoView, CView)
	//{{AFX_MSG_MAP(CDrawCubeDemoView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoView construction/destruction

CDrawCubeDemoView::CDrawCubeDemoView()
{
	// TODO: add construction code here
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_xAngle = 0.0f;
    m_yAngle = 0.0f;
	m_Scale = 1.0f;
}

CDrawCubeDemoView::~CDrawCubeDemoView()
{
}

BOOL CDrawCubeDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//An OpenGL Window must be created with the following flags
	
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoView drawing

void CDrawCubeDemoView::OnDraw(CDC* pDC)
{
	CDrawCubeDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	// Clear out the color & depth buffers
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	RenderScene();
	// Tell OpenGL to flush its pipeline
	::glFinish();
	// Now Swap the buffers
     ::SwapBuffers( m_pDC->GetSafeHdc() );
}

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoView printing

BOOL CDrawCubeDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawCubeDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawCubeDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoView diagnostics

#ifdef _DEBUG
void CDrawCubeDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawCubeDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawCubeDemoDoc* CDrawCubeDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawCubeDemoDoc)));
	return (CDrawCubeDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawCubeDemoView message handlers

int CDrawCubeDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//Initialize OpenGL Here
	
    InitializeOpenGL();
	return 0;
}

void CDrawCubeDemoView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	//Make the RC non-current
	if(::wglMakeCurrent (0,0) == FALSE)
	{
		MessageBox(_T("Could not make RC non-current"));
	}
	
	//Delete the rendering context
	if(::wglDeleteContext (m_hRC)==FALSE)
	{
		MessageBox(_T("Could not delete RC"));
	}
	//Delete the DC
	if(m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL
     m_pDC = NULL;
}

void CDrawCubeDemoView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	GLdouble aspect_ratio; // width/height ratio
	
	if ( 0 >= cx || 0 >= cy )
	{
		return;
	}
	// select the full client area
	::glViewport(0, 0, cx, cy);
	// compute the aspect ratio
	// this will keep all dimension scales equal
	aspect_ratio = (GLdouble)cx/(GLdouble)cy;
	// select the projection matrix and clear it
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	// select the viewing volume
	::gluPerspective(45.0f, aspect_ratio, .01f, 200.0f);
	
	// switch back to the modelview matrix and clear it
	::glMatrixMode(GL_MODELVIEW);
     ::glLoadIdentity();
}

BOOL CDrawCubeDemoView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}
BOOL CDrawCubeDemoView::InitializeOpenGL()
{
	//Get a DC for the Client Area
	
    m_pDC = new CClientDC(this);
	
    //Failure to Get DC
	
    if(m_pDC == NULL)
		
    {
		
        MessageBox(_T("Error Obtaining DC"));
		
        return FALSE;
		
    }
	
    //Failure to set the pixel format
	
    if(!SetupPixelFormat())
		
    {
		
        return FALSE;
		
    }
	
    //Create Rendering Context
	
    m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());
	
    //Failure to Create Rendering Context
	
    if(m_hRC == 0)
		
    {
		
        MessageBox(_T("Error Creating RC"));
		
        return FALSE;
		
    }
	
    //Make the RC Current
	
    if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
		
    {
		
        MessageBox(_T("Error making RC Current"));
		
        return FALSE;
		
    }
	
    //Specify Black as the clear color
	
    ::glClearColor(0.0f,0.0f,0.0f,0.0f);
	
    //Specify the back of the buffer as clear depth
	
    ::glClearDepth(1.0f);
	
    //Enable Depth Testing
	
    ::glEnable(GL_DEPTH_TEST);
	
    return TRUE;

}
BOOL CDrawCubeDemoView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd = 
		
    {
		
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
			1,                              // version number
			
			PFD_DRAW_TO_WINDOW |            // support window
			
			PFD_SUPPORT_OPENGL |            // support OpenGL
			
			PFD_DOUBLEBUFFER,                // double buffered
			
			PFD_TYPE_RGBA,                  // RGBA type
			24,                             // 24-bit color depth
			0, 0, 0, 0, 0, 0,               // color bits ignored
			0,                              // no alpha buffer
			0,                              // shift bit ignored
			0,                              // no accumulation buffer
			0, 0, 0, 0,                     // accum bits ignored
			16,                             // 16-bit z-buffer
			0,                              // no stencil buffer
			0,                              // no auxiliary buffer
			
			PFD_MAIN_PLANE,                 // main layer
			0,                              // reserved
			0, 0, 0                         // layer masks ignored
			
    };
	
    int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	
    if ( m_nPixelFormat == 0 )
		
    {
		
		return FALSE;
		
    }
	
    if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
		
    {
		
		return FALSE;
		
    }
	
    return TRUE;
}
void CDrawCubeDemoView::RenderScene()
{		    

		glLoadIdentity();
		glTranslatef(m_xPos, m_yPos, -5.0f);
		glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
		glRotatef(m_yAngle, 0.0f,1.0f,0.0f);
		glScalef(m_Scale,m_Scale,m_Scale);
		 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
         //Front Face
         glBegin(GL_POLYGON);
             glColor3f(1.0f,0.0f,0.0f);
             glVertex3f(-1.0f,-1.0f,0.0f);
             glColor3f(1.0f,1.0f,0.0f);
             glVertex3f( 1.0f,-1.0f,0.0f);
             glColor3f(1.0f,0.0f,1.0f);
             glVertex3f( 1.0f, 1.0f,0.0f);
             glColor3f(1.0f,1.0f,1.0f);
             glVertex3f(-1.0f, 1.0f,0.0f);
         glEnd();
         glColor3f(1.0f,1.0f,0.0f);
         //Back Face
         glBegin(GL_POLYGON);
             glVertex3f(-1.0f,-1.0f,-1.0f);
             glVertex3f(-1.0f, 1.0f,-1.0f);
             glVertex3f( 1.0f, 1.0f,-1.0f);
             glVertex3f( 1.0f,-1.0f,-1.0f);
         glEnd();
         glColor3f(1.0f,0.0f,1.0f);
         //Left Face
         glBegin(GL_POLYGON);
             glVertex3f(-1.0f,-1.0f, 0.0f);
             glVertex3f(-1.0f, 1.0f, 0.0f);
             glVertex3f(-1.0f, 1.0f,-1.0f);
             glVertex3f(-1.0f,-1.0f,-1.0f);
         glEnd();
         glColor3f(0.0f,1.0f,0.0f);
         //Right Face
         glBegin(GL_POLYGON);
             glVertex3f(1.0f,-1.0f, 0.0f);
             glVertex3f(1.0f,-1.0f,-1.0f);
             glVertex3f(1.0f, 1.0f,-1.0f);
             glVertex3f(1.0f, 1.0f, 0.0f);
         glEnd();
         glColor3f(0.0f,1.0f,1.0f);
         //Top Face
         glBegin(GL_POLYGON);
             glVertex3f(-1.0f, 1.0f,  0.0f);
             glVertex3f( 1.0f, 1.0f,  0.0f);
             glVertex3f( 1.0f, 1.0f, -1.0f);
             glVertex3f(-1.0f, 1.0f, -1.0f);
         glEnd();
         glColor3f(0.0f,0.0f,1.0f);
         //Botton Face
         glBegin(GL_POLYGON);
		 glVertex3f(-1.0f, -1.0f,  0.0f);
		 glVertex3f(-1.0f, -1.0f, -1.0f);
		 glVertex3f( 1.0f, -1.0f, -1.0f);
		 glVertex3f( 1.0f, -1.0f,  0.0f);
         glEnd();
 
}

void CDrawCubeDemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
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
	default:        MessageBox(_T("Press the arrow keys only"));
		break;
	}        
	
    InvalidateRect(NULL,FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDrawCubeDemoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_MouseDownPoint=point;
     SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

void CDrawCubeDemoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_MouseDownPoint=CPoint(0,0);
     ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

void CDrawCubeDemoView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetCapture()==this)
	{
		//Increment the object rotation angles
		m_xAngle+=(point.y-m_MouseDownPoint.y)/3.6;
		m_yAngle+=(point.x-m_MouseDownPoint.x)/3.6;
		//Redraw the view
		InvalidateRect(NULL,FALSE);
		//Set the mouse point
		m_MouseDownPoint=point;
     };
	CView::OnMouseMove(nFlags, point);
}

BOOL CDrawCubeDemoView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	m_Scale += 0.01 *zDelta /120;
	InvalidateRect(NULL,FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
