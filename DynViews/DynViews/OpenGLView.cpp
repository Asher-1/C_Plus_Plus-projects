// OpenGLView.cpp : implementation file
//

#include "stdafx.h"
#include <math.h> 

#include "DynViews.h"
#include "DynViewsDlg.h"
#include "OpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void auxWireCube (GLdouble size);
	void auxSolidCube (GLdouble size);
	void auxWireBox (GLdouble width, GLdouble height, GLdouble depth);
	void auxSolidBox (GLdouble width, GLdouble height, GLdouble depth);
	void auxWireTorus (GLdouble innerRadius, GLdouble outerRadius);
	void auxSolidTorus (GLdouble innerRadius, GLdouble outerRadius);
	void auxWireCylinder (GLdouble radius, GLdouble height);
	void auxSolidCylinder (GLdouble radius, GLdouble height);
	void auxWireIcosahedron (GLdouble radius);//20 faces
	void auxSolidIcosahedron (GLdouble radius);
	void auxWireOctahedron (GLdouble radius);// 8 faces
	void auxSolidOctahedron (GLdouble radius);
	void auxWireTetrahedron (GLdouble radius);//4 faces
	void auxSolidTetrahedron (GLdouble radius);
	void auxWireDodecahedron (GLdouble radius);//12 faces
	void auxSolidDodecahedron (GLdouble radius);
	void auxWireCone (GLdouble base, GLdouble height);
	void auxSolidCone (GLdouble base, GLdouble height);
	void auxSolidSphere (GLdouble radius);
	void auxWireSphere (GLdouble radius);
// extra
	void doughnut(GLdouble r, GLdouble R, GLint nsides, GLint rings, GLenum type);
	void ArcChamfer(GLdouble r,GLdouble R, GLint nsides, GLint rings, GLenum type);
	void ArcWheel(GLdouble r,GLdouble R, GLint nsides, GLint rings, GLenum type);
	void ArcSlot(GLdouble r,GLdouble R, GLint nsides, GLint rings, GLenum type);
	void cylinder(GLdouble radius,GLint stacks,GLint slices,GLdouble length,GLenum type);
	void Roundwall(GLdouble radius,GLdouble height,GLint slices,GLenum type);
	void Circle(GLdouble radius,GLint slices,GLenum type);
	void Cone(GLdouble baseR,GLdouble height,GLint slices,GLenum type);
	void LineChamfer(GLdouble baseR,GLdouble topR,GLdouble height,GLint slices,GLenum type);
	void Torus(float rc, int numc, float rt, int numt);
	void Cage(void);
	void Cylinder(int numEdges);
	void gear(GLfloat inner_radius,GLfloat outer_radius,GLfloat width,GLint teeth,GLfloat tooth_depth);
}
/////////////////////////////////////////////////////////////////////////////
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

COpenGLView::COpenGLView()
{
	m_idView=0;
}

COpenGLView::~COpenGLView()
{
}


BEGIN_MESSAGE_MAP(COpenGLView, CView)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLView drawing

void COpenGLView::OnDraw(CDC* pDC)
{
//The wglMakeCurrent function makes a specified OpenGL 
//rendering context the calling thread's current rendering context.
//All subsequent OpenGL calls made by the thread are drawn 
//on the device identified by hdc. 	
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
//The glClear function clears buffers to preset values.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//The function multiply the current matrix by a rotation matrix.
// view->tabctrl->dlg 
	CDynViewsDlg *pDlg=(CDynViewsDlg *)(GetParent()->GetParent());
	switch (pDlg->m_RotAxis[m_idView])
	{
	case 1:
		glRotated(1, 1.0, 0.0, 0.0);//x
		break;
	case 2:
		glRotated(1, 0.0, 1.0, 0.0);//y
		break;
	case 3:
		glRotated(1, 0.0, 0.0, 1.0);//z no roation !
		break;
	}
//The function push the current matrix stack.
	glPushMatrix();
//The function set the current color
	glColor3f(1.0f, 1.0f, 1.0f);//red green blue
/**/
//	Roundwall(0.4,1.0,100,GL_TRIANGLE_STRIP);//solid GL_LINE_LOOP wired
//	cylinder(0.5,10,10,1.0,GL_QUAD_STRIP);//GL_LINE_LOOP);
//	auxSolidBox (0.5,1.0,1.0);
//	auxWireBox (0.5,1.0,1.0);
//	auxSolidCube(1.0);
//	auxWireCube (1.0);
//	auxSolidTorus (0.2,1.0);// roughly !
//  doughnut(0.2, 1.0, 20, 50, GL_LINE_LOOP);// fine 
//	doughnut(0.2, 1.0, 20, 49, GL_QUADS);
//	auxWireCylinder (0.5,0.5);
//	auxSolidCylinder (0.5,0.5);
//	auxSolidIcosahedron (0.5);
//	auxSolidDodecahedron(0.5);
//	auxWireOctahedron(0.5);
//	auxSolidCone (0.5,1.0);
/**/
	switch(m_idView)
	{
	case 0:
//		gluCylinder(m_quadric, 0.5, 0.1, 0.4, 32, 32);
		Cone(0.6,0.4,16,GL_TRIANGLE_STRIP);//GL_LINE_LOOP);// with 0.0, degrated to circle !
		glRotated(180, 1.0, 0.0, 0.0);//z roation !
		cylinder(0.6,16,16,1.0,GL_LINE_LOOP);
		glTranslated(0.0, 0.0, 1.0);//move z
		Cone(0.6,0.4,16,GL_LINE_LOOP);
	break;
	case 1:
//		auxWireSphere (0.5f);
		gear( 0.5f, 0.8f, 0.25f, 20, 0.2f );
//
//		Cylinder(16);

//		glScalef(0.2f,0.2f,0.2f);
//		Cage();

		//Torus(0.6f,16,0.0f,16);//sphere
		//doughnut(0.2, 0.5, 16, 16, GL_LINE_LOOP); 
		//ArcWheel(0.2, 0.6, 16, 16, GL_QUADS);//GL_LINE_LOOP); 
	break;
	case 2:
//		ArcSlot(0.2, 0.6, 16, 16, GL_QUADS);//GL_LINE_LOOP); 
		cylinder(0.6,16,16,0.6,GL_TRIANGLE_STRIP);//GL_LINE_LOOP);
		ArcWheel(0.2, 0.6, 16, 16, GL_QUADS);//GL_LINE_LOOP); 
		glTranslated(0.0, 0.0, -0.6);//move z
		cylinder(0.6,16,16,0.6,GL_TRIANGLE_STRIP);//GL_LINE_LOOP);
	break;
	case 3:
// create a cylinder
		cylinder(0.6,32,32,0.6,GL_TRIANGLE_STRIP);//GL_LINE_LOOP);
//move in z, cylinder length 
		glTranslated(0.0, 0.0, 0.6);
// add a chamfer
		ArcChamfer(0.1, 0.5, 32, 32, GL_QUADS);//GL_LINE_LOOP);
// move in z, chamfer length
		glTranslated(0.0, 0.0, 0.1);
// pyt a cover
		Circle(0.5,32,GL_TRIANGLE_STRIP);
	break;
	case 4:
		Roundwall(0.4,1.0,16,GL_LINE_STRIP);//GL_TRIANGLE_STRIP);
	break;
	case 5:
		auxWireCone (0.5,0.5);
		glRotated(180, 0.0, 1.0, 0.0);//y 
//
		cylinder(0.5,10,10,1.0,GL_QUAD_STRIP);
	break;
	}
//The function pop the current matrix stack.
	glPopMatrix();
//The function exchanges the front and back buffers.	
	SwapBuffers(m_pDC->GetSafeHdc());
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1) return -1;
//	
	InitGL();
//	
	return 0;
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	return CView::PreCreateWindow(cs);
}

void COpenGLView::InitGL()
{
	m_pDC = new CClientDC(this);
//	
	SetupPixelFormat();
//The function creates a new OpenGL rendering context, 
//which is suitable for drawing on the device referenced by hdc.
//The rendering context has the same pixel format as the device context.
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
//The wglMakeCurrent function makes a specified OpenGL 
//rendering context the calling thread's current rendering context.
//All subsequent OpenGL calls made by the thread are drawn 
//on the device identified by hdc. 	
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
//The glClear function clears buffers to preset values.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//The function enable OpenGL capabilities
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	GLfloat color[4] = { 1.0f, 1.0f, 1.0f, 1.0f  };
	GLfloat lightPos[4] = {1.0f, 1.0f, 1.0f, 1.0f };
//The function set light source parameters.
	glLightfv(GL_LIGHT0, GL_AMBIENT, color);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//
	m_quadric = gluNewQuadric();
//chage it
	SetTimer(m_idView, 10, NULL);//10
}

void COpenGLView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0,
	};
//The function attempts to match an appropriate pixel format
//supported by a device context to a given pixel format specification.
	int pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
//The function sets the pixel format of the specified device context
//to the format specified by the iPixelFormat index.
	SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);
}

BOOL COpenGLView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void COpenGLView::OnDestroy() 
{// If hglrc is NULL, the function makes the calling 
//	thread's current rendering context no longer current, 
//  and releases the device context that is used by the rendering context.
//  In this case, hdc is ignored.
	wglMakeCurrent(NULL, NULL);
//The function deletes a specified OpenGL rendering context. 
	wglDeleteContext(m_hRC);
	if(m_pDC) delete m_pDC;
//The function destroys a quadric object.
	gluDeleteQuadric(m_quadric);
//
	CView::OnDestroy();
}

void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
//The wglMakeCurrent function makes a specified OpenGL 
//rendering context the calling thread's current rendering context.
//All subsequent OpenGL calls made by the thread are drawn 
//on the device identified by hdc. 	
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	glViewport(0, 0, cx, cy);
	double aspect=(double) cx / (double) cy;
//The function specifies which matrix is the current matrix.
	glMatrixMode(GL_PROJECTION);
//The function replaces the current matrix with the identity matrix.
	glLoadIdentity();
//	glOrtho(-aspect, aspect, -1.0, 1.0, 1.0, 100.0);
	gluPerspective(45.0, aspect, 0.1, 100.0);
//The function specifies which matrix is the current matrix.
	glMatrixMode(GL_MODELVIEW);
//The function replaces the current matrix with the identity matrix.
	glLoadIdentity();
//The function defines a viewing transformation.
	gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void COpenGLView::OnTimer(UINT nIDEvent) 
{
	Invalidate();
//	
	CView::OnTimer(nIDEvent);
}
