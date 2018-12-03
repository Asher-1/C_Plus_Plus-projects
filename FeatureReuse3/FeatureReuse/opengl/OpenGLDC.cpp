// OpenGLDC.cpp: implementation of the COpenGLDC class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "OpenGLDC.h"



#define GL_PI 3.1415f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenGLDC::COpenGLDC(HWND hWnd):m_hWnd(hWnd)
{
	m_bSelectionMode = FALSE;
	m_clrBkMaterial = RGB(0,0,0); 
}

COpenGLDC::~COpenGLDC()
{
	//if (r != NULL)
	//{
	//	delete[] r;
	//	r = NULL;
	//}
	//if (g != NULL)
	//{
	//	delete[] g;
	//	g = NULL;
	//}
	//if (b != NULL)
	//{
	//	delete[] b;
	//	b = NULL;
	//}
}

BOOL COpenGLDC::InitDC()
{
	if (m_hWnd == NULL) return FALSE;
	
	m_Camera.init();

	m_hDC = ::GetDC(m_hWnd);			// Get the Device context

	int pixelformat;
	PIXELFORMATDESCRIPTOR pfdWnd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Structure size.
		1,                             // Structure version number.
		PFD_DRAW_TO_WINDOW |           // Property flags.
		PFD_SUPPORT_OPENGL |
 		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,                            // 24-bit color.
		0, 0, 0, 0, 0, 0,              // Not concerned with these.
		0, 0, 0, 0, 0, 0, 0,           // No alpha or accum buffer.
		32,                            // 32-bit depth buffer.
		0, 0,                          // No stencil or aux buffer.
		PFD_MAIN_PLANE,                // Main layer type.
		0,                             // Reserved.
		0, 0, 0                        // Unsupported.
	};

    if ( (pixelformat = ChoosePixelFormat(m_hDC, &pfdWnd)) == 0 )
	{
		AfxMessageBox("ChoosePixelFormat to wnd failed");
		return FALSE;
	}

    if (SetPixelFormat(m_hDC, pixelformat, &pfdWnd) == FALSE)
        AfxMessageBox("SetPixelFormat failed");

	m_hRC=wglCreateContext(m_hDC);

	VERIFY(wglMakeCurrent(m_hDC,m_hRC));
	GLSetupRC();
	wglMakeCurrent(NULL,NULL);
	return m_hRC!=0;
}


void COpenGLDC::GLResize(int w,int h)
{
	wglMakeCurrent(m_hDC,m_hRC);

	// Prevent a divide by zero
	if(h == 0) h = 1;
	if(w == 0) w = 1;
	m_Camera.set_screen(w,h);
}

void COpenGLDC::GLSetupRC()
{
	//initialize color and rendering
	m_bShading = TRUE;

	//bright white light - full intensity RGB values
	GLfloat lightAmbient[] = {0.6f,0.6f,0.6f,1.0f};
	GLfloat lightDiffuse[] = {0.6f,0.6f,0.6f,1.0f};
		//////////////////////////////////////////////////////////////////////////
	glShadeModel(GL_SMOOTH);  //刘斌添加
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_CCW);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//刘斌添加
	glEnable(GL_BLEND);//刘斌添加

	//glCullFace(GL_BACK);
	//////////////////////////////////////////////////////////////////////////

	glEnable(GL_DEPTH_TEST);		//Hidden surface removal
	//glEnable(GL_CULL_FACE);			//Do not calculate inside of object
	glFrontFace(GL_CCW);			//counter clock-wise polygons face out
//	glCullFace(GL_BACK);


	CreateLight();

	//Initialize Material Color to Gray
	SetBkColor(RGB(0,0,0));							//black background****************屏幕背景
	SetMaterialColor(RGB(180,100,100));		//golden material color  模型正面
	SetBkMaterialColor(RGB(128,128,128));   //模型背面
	SetColor(RGB(255,255,255));					//white frame color
	SetHighlightColor(RGB(255,0,0));				//red highlight color

	//Point Size
	glPointSize(1.0);
	//glLineWidth(1);
}



void COpenGLDC::Ready()
{
	wglMakeCurrent(m_hDC,m_hRC);
	ClearBkground();
	OnShading();
	m_Camera.projection();
}

void COpenGLDC::Finish()
{
	glFlush();
	SwapBuffers(m_hDC);
	wglMakeCurrent(m_hDC,NULL);
}

void COpenGLDC::FinshForEps()
{
	SwapBuffers(m_hDC);
}


//////////////////LIGHT && MATERIALS SETTING//////////
void COpenGLDC::ClearBkground()
{
	GLclampf	r,g,b;
	r = (GLclampf)GetRValue(m_clrBk)/255.0;
	g = (GLclampf)GetGValue(m_clrBk)/255.0;
	b = (GLclampf)GetBValue(m_clrBk)/255.0;

	glClearColor(r,g,b,0.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

//setting model
void COpenGLDC::OnShading()
{
	if(m_bShading )
	{
		glEnable( GL_LIGHTING );
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	}
	else
	{
		COLORREF bkcol;
		GetBkColor(bkcol);
		BYTE r,g,b;
		r = GetRValue(bkcol);
		g = GetGValue(bkcol);
		b = GetBValue(bkcol);
		if (r==255 && g==255 && b==255)
		{
			r=128;
			g=128;
			b=128;
		}
		COLORREF clr(RGB(255-r,255-g,255-b));
		
		SetColor(clr);

		glDisable( GL_LIGHTING );
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
}

void COpenGLDC::Shading(BOOL bShading)
{
	m_bShading = bShading;
}

BOOL COpenGLDC::IsShading()
{
	return m_bShading;
}

void COpenGLDC::Lighting(BOOL bLighting)
{
	if(bLighting)
		glEnable( GL_LIGHTING );
	else
		glDisable( GL_LIGHTING );
}

BOOL COpenGLDC::IsLighting()
{
	GLboolean bLighting;
	glGetBooleanv(GL_LIGHTING,&bLighting);   
	return bLighting;
}

void COpenGLDC::SetLightDirection(float dx,float dy,float dz)
{
	m_vecLight[0] = dx;
	m_vecLight[1] = dy;
	m_vecLight[2] = dz;
	GLfloat lightPos[] = { dx, dy, dz, 0.0f };
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
}

void COpenGLDC::GetLightDirection(float& dx,float& dy,float& dz)
{
	dx = m_vecLight[0];
	dy = m_vecLight[1];
	dz = m_vecLight[2];
}



//rendering color
void COpenGLDC::SetMaterialColor(COLORREF clr)
{
	float mat[4];

	mat[0] = 0.5;
	mat[1] = 0.5;
	mat[2] = 0.6;
	mat[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);

	m_clrMaterial = clr;
	BYTE r,g,b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
	mat[0] = (GLfloat)r/255;
	mat[1] = (GLfloat)g/255;
	mat[2] = (GLfloat)b/255;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	glMaterialf(GL_FRONT, GL_SHININESS,128.0);
	
}

void COpenGLDC::GetMaterialColor(COLORREF& clr)
{
	clr = m_clrMaterial;
}

void COpenGLDC::GetBkMaterialColor(COLORREF& clr)
{
	clr = m_clrBkMaterial;
}
void COpenGLDC::SetBkMaterialColor(COLORREF clr)
{
	float mat[4];

	mat[0] = 0.5;
	mat[1] = 0.5;
	mat[2] = 0.5;
	mat[3] = 1.0;
	glMaterialfv(GL_BACK, GL_AMBIENT, mat);

	m_clrBkMaterial = clr;
	BYTE r,g,b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
	//GLfloat mat_amb_diff[] = {(GLfloat)r/255,(GLfloat)g/255,(GLfloat)b/255,1.0};
	//glMaterialfv(GL_BACK,GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
	glMaterialfv(GL_BACK, GL_DIFFUSE, mat);

	glMaterialfv(GL_BACK, GL_SPECULAR, mat);
	glMaterialf(GL_BACK, GL_SHININESS, 0.25 * 128.0);
}

void COpenGLDC::SetBkColor(COLORREF clr)
{
	m_clrBk = clr;
}

void COpenGLDC::GetBkColor(COLORREF& clr)
{
	clr = m_clrBk;
}

void COpenGLDC::SetColor(COLORREF clr)
{
	m_clr = clr;
	BYTE r,g,b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
	glColor3ub(r,g,b);
}

void COpenGLDC::GetColor(COLORREF& clr)
{
	clr =  m_clr;
}

void COpenGLDC::SetHighlightColor(COLORREF clr)
{
	m_clrHighlight = clr;
}

void COpenGLDC::GetHighlightColor(COLORREF& clr)
{
	clr = m_clrHighlight;
}

void COpenGLDC::Highlight(BOOL bHighlight)
{
	BYTE r,g,b;
	if(bHighlight){
		r = GetRValue(m_clrHighlight);
		g = GetGValue(m_clrHighlight);
		b = GetBValue(m_clrHighlight);
	}
	else{
		r = GetRValue(m_clrMaterial);
		g = GetGValue(m_clrMaterial);
		b = GetBValue(m_clrMaterial);
	}
	GLfloat mat_amb_diff[] = {(GLfloat)r/255,(GLfloat)g/255,(GLfloat)b/255,1.0};
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
}


//////////////////////// drawing functions ////////////////////////
//draw point
void COpenGLDC::DrawPoint(const CPoint3D& pt)
{
	glBegin(GL_POINTS);
		glVertex3f(pt.x,pt.y,pt.z);
	glEnd();
}
void COpenGLDC::DrawPoint(const CPoint3D& pt, COLORREF ctr)
{
	BOOL bLighting = IsLighting();
	Lighting(FALSE);
	COLORREF old_clr;
	GetColor(old_clr);
	SetColor(ctr);

	glBegin(GL_POINTS);
	glVertex3f(pt.x,pt.y,pt.z);
	glEnd();

	Lighting(bLighting);
	SetColor(old_clr);
}
void COpenGLDC::DrawLine(const CPoint3D& sp,const CPoint3D& ep)
{
	BOOL bLighting = IsLighting();
	Lighting(FALSE);
	glBegin(GL_LINES);
		glVertex3f(sp.x,sp.y,sp.z);
		glVertex3f(ep.x,ep.y,ep.z);
	glEnd();
	Lighting(bLighting);
}
void COpenGLDC::DrawLine(const CPoint3D& sp,const CPoint3D& ep,int R,int G,int B)
{
	BOOL bLighting = IsLighting();
	Lighting(FALSE);
	COLORREF old_clr;
	GetColor(old_clr);
	SetColor(RGB(R,G,B));

	glBegin(GL_LINES);
	glVertex3f(sp.x,sp.y,sp.z);
	glVertex3f(ep.x,ep.y,ep.z);
	glEnd();

	Lighting(bLighting);
	SetColor(old_clr);
}

//
void COpenGLDC::DrawJuxing(const CPoint3D& p1,const CPoint3D& p2,const CPoint3D& p3,const CPoint3D& p4)
{
	BOOL bLighting = IsLighting();
	Lighting(FALSE);

	glBegin(GL_QUADS);						// 绘制正方形
	glVertex3f( p1.x, p1.y, 0.0f);				// 左上
	glVertex3f( p2.x, p2.y, 0.0f);				// 右上
	glVertex3f( p3.x, p3.y, 0.0f);				// 左下
	glVertex3f( p4.x, p4.y, 0.0f);				// 右下
	glEnd();	

	Lighting(bLighting);
}
void COpenGLDC::DrawPolyline(const CPoint3D* pt,int size)
{
	BOOL bLighting = IsLighting();
	Lighting(FALSE);
	COLORREF old_clr;
	GetColor(old_clr);
	SetColor(RGB(255,0,0));

	glBegin(GL_LINE_STRIP);
	for(int i=0;i<size;i++)
		glVertex3f(pt[i].x,pt[i].y,pt[i].z);
	glEnd();
	Lighting(bLighting);
	SetColor(old_clr);
}
void COpenGLDC::DrawPolyline(const vector<CPoint3D> pts, int R, int G, int B)
{
	BOOL bLighting = IsLighting();
	Lighting(FALSE);
	COLORREF old_clr;
	GetColor(old_clr);
	SetColor(RGB(R,G,B));

	glBegin(GL_LINE_STRIP);
	for(int i=0;i<pts.size();i++)
		glVertex3f(pts[i].x,pts[i].y,pts[i].z);
	glEnd();
	Lighting(bLighting);
	SetColor(old_clr);
}

//void COpenGLDC::DrawArrows(double x0, double y0, double Vx, double Vy)
//{
//	float originX = 0;
//	float originY = 0;
//	double len = 2;
//	float signX = 1.0;
//	float signY = 1.0;				 // X,Y轴向的符号标记
//	float deltX = Vx - 0;
//	float deltY = Vy - 0;
//	float angle = 10;				// 箭头夹角
//
//	float cosalph = deltX / sqrt(deltX*deltX + deltY*deltY); //算一算旋转的角度
//	float sinalph = deltY / sqrt(deltX*deltX + deltY*deltY);
//
//	float tem_x = cosalph*(len*signX);
//	float tem_y = sinalph*(len*signY);
//
//	float dstX = x0 + 10 * tem_x;
//	float dstY = y0 + 10 * tem_y;
//
//	if (deltX >= 0)
//	{
//		signX = -1.0;
//	}
//	if (deltY >= 0)
//	{
//		signY = -1.0;
//	}
//
//
//
//	//float  alph = asinf(sinalph);
//	//float Angle1 = alph - angle;
//	//float Angle2 = 90 - alph - angle;
//	//float x1 = dstX + 3*(-len)*cos(Angle1);
//	//float y1 = dstY + 3* (-len)*sin(Angle1);
//	//float x2 = dstX + 3* (-len)*sin(Angle2);
//	//float y2 = dstY + 3* (-len)*cos(Angle2);
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 以填充模式绘制
//	//glBegin(GL_TRIANGLES);
//	//glVertex2f(x1, y1);
//	//glVertex2f(dstX, dstY);
//	//glVertex2f(x2, y2);
//	//glEnd();
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 以边线绘制
//
//	//float side = 0.0;
//	//if (tem_y > tem_x)
//	//	side = tem_x;
//	//else
//	//	side = tem_y;
//	//float x0 = dstX;
//	//float y0 = dstY - side;
//
//		// 画带箭头的直线
//		glLineWidth(2.0);
//		glColor3f(0.0, 1, 0.0);
//		glEnable(GL_LINE_SMOOTH);
//
//		glBegin(GL_LINES);
//			glVertex2f(x0, y0);
//			glVertex2f(dstX, dstY);
//		glEnd();
//
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 以填充模式绘制
//		glBegin(GL_TRIANGLES);
//			glVertex2f(dstX, dstY - 3*tem_y);
//			glVertex2f(dstX, dstY);
//			glVertex2f(dstX - 3*tem_x, dstY);
//		glEnd();
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 以边线绘制
//
//	glDisable(GL_LINE_SMOOTH);
//}


void COpenGLDC::DrawArrows(double x0, double y0, double Vx, double Vy)
{
	float originX = x0;
	float originY = y0;
	double len = 2;
	float signX = 1.0;
	float signY = 1.0;				 // X,Y轴向的符号标记
	float deltX = Vx - 0;
	float deltY = Vy - 0;

	float cosalph = deltX / sqrt(deltX*deltX + deltY*deltY); //算一算旋转的角度
	float sinalph = deltY / sqrt(deltX*deltX + deltY*deltY);
	//float angle = asinf(sinalph);				// 夹角

	float tem_x = cosalph*(len*signX);
	float tem_y = sinalph*(len*signY);
	float dstX = originX +  tem_x;
	float dstY = originY +  tem_y;

	if (deltX >= 0)
	{
		signX = -1.0;
	}
	if (deltY >= 0)
	{
		signY = -1.0;
	}

	// 画带箭头的直线
	glLineWidth(2.0);
	glColor3f(0.0, 1, 0.0);
	glEnable(GL_LINE_SMOOTH);

	glPushMatrix();
	glTranslatef(dstX, dstY,0.0f);
	glRotatef(90.0f, 0.0f,dstX- originX, dstY- originY);
	glutWireCone(0.05*len, 0.2*len, 10, 10);
	glPopMatrix();

	glBegin(GL_LINES);
	glVertex3f(originX, originY,0.0f);
	glVertex3f(dstX, dstY,0.0f);
	glEnd();

	glDisable(GL_LINE_SMOOTH);
}


void COpenGLDC::DrawSingleModel(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glRectf(0.0f, 0.0f, 0.5f, 0.5f);// 换一个矩形  
	
	glPointSize(5.0f);// 设置点的大小
 
	glBegin(GL_TRIANGLES);// 设置绘制模式为三角面片
	glColor3f(0.0f, 0.0f, 1.0f);// 设置画笔颜色 为红色 
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();

	glBegin(GL_POINTS);// 绘制节点
	glColor3f(1.0f, 0.0f, 0.0f);// 设置画笔颜色 为红色 
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();

}




// 构建颜色填充表函数
void COpenGLDC::TColor(int m)
{
	//if (r != NULL)
	//{
	//	delete[] r;
	//	r = NULL;
	//}
	//if (g != NULL)
	//{
	//	delete[] g;
	//	g = NULL;
	//}
	//if (b != NULL)
	//{
	//	delete[] b;
	//	b = NULL;
	//}


	r = new double[m];
	g = new double[m];
	b = new double[m];
	r[0] = 1.0;
	g[m - 1] = 1.0;
	r[m - 1] = 0.0;
	g[0] = 0.0;
	b[0] = 0.0;
	b[m - 1] = 0.0;
	double dr = (r[m - 1] - r[0]) / m;
	double dg = (g[m - 1] - g[0]) / m;
	double db = (b[m - 1] - b[0]) / m;
	for (int i = 1; i < m - 1; i++)
	{
		r[i] = r[i - 1] + dr;
		g[i] = g[i - 1] + dg;
		b[i] = b[i - 1] + db;
	}
}

// 指定文本在窗口中的输出信息及位置
void COpenGLDC::gltRasterText(double x, double y, const char *text, void *font)
{
	if (text == NULL) return;

	glRasterPos2d(x, y);
	for (int i = 0; text[i] != '\0'; i++)
	{
		glutBitmapCharacter(font, text[i]);
	}
}

// 绘制色码标识
void COpenGLDC::DrawCodeIdentifier(double x, double y, int Ncolor, int column, int Dc, int min)
{
	int i, j;
	char text[32];
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glClearColor(0, 0, 0, 0);
	//glClear(GL_COLOR_BUFFER_BIT);
	for (i = 0; i < Ncolor; i++)
	{
		//COLORREF rgb = RGB(r[i], g[i], b[i]);
		//SetMaterialColor(rgb);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE);
		glShadeModel(GL_SMOOTH);
		glColor3d(r[i], g[i], b[i]);
		glRectd(x + 20 + (column - 1)*20.0 + 10, y + (i + 1)*15.0, x + 20 + column*20.0, y + i*15.0);
		glColor3d(0.0f, 0.0f, 0.0f);
		// 画直线
		glBegin(GL_LINES);
		glVertex2d(x + 20 + (column - 1)*20.0 + 10, y + i*15.0);
		glVertex2d(x + 20 + column*20.0, y + i*15.0);
		glEnd();

		sprintf(text, "%d", min + i * Dc);
		glColor3d(0, 0, 0);
		gltRasterText(x + 20 + column*20.0, y + i*15.0 - 5, text, GLUT_BITMAP_HELVETICA_18);

	}
	glFlush();

	sprintf(text, "%d", min + Ncolor*Dc);
	glColor3d(0, 0, 0);
	gltRasterText(x + 20 + column*20.0, y + Ncolor*15.0 - 5, text, GLUT_BITMAP_HELVETICA_18);

	// 画直线
	glBegin(GL_LINES);
	glVertex2d(x + 20 + (column - 1)*20.0 + 10, y);
	glVertex2d(x + 20 + (column - 1)*20.0 + 10, y + Ncolor*15.0);
	glEnd();
	// 画直线
	glBegin(GL_LINES);
	glVertex2d(x + 20 + column*20.0, y);
	glVertex2d(x + 20 + column*20.0, y + Ncolor*15.0);
	glEnd();
	// 画直线
	glBegin(GL_LINES);
	glVertex2d(x + 20 + (column - 1)*20.0 + 10, y + Ncolor*15.0);
	glVertex2d(x + 20 + column*20.0, y + Ncolor*15.0);
	glEnd();
}

// 循环递归调用四叉树算法填充网格矩形颜色值
void COpenGLDC::TreeMath(double x, double y, double dx, double dy, double z1, double z2, double z3, double z4, double min, int Dc)
{
	int Ic1, Ic2, Ic3, Ic4;
	Ic1 = int((z1 - min)*1.0 / Dc);
	Ic2 = int((z2 - min)*1.0 / Dc);
	Ic3 = int((z3 - min)*1.0 / Dc);
	Ic4 = int((z4 - min)*1.0 / Dc);



	//glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	//glClearColor(0, 0, 0, 0);
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glShadeModel(GL_SMOOTH);

	if (Ic1 == Ic2 && Ic2 == Ic3 && Ic3 == Ic4)
	{

		glColor3f(r[Ic1], g[Ic1], b[Ic1]);
		glRectd(x, y, x + dx, y + dy);
	}
	else if (dx <= 1 && dy <= 1)
	{
		double dx1 = dx / 10;
		double dy1 = dy / 10;
		for (int n = 0; n < 10; n++)
		{
			glColor3d(0.0, 0.0, 0.0);
			glBegin(GL_POINTS);
			glVertex2d(x + n*dx1, y + n*dy1);
			glEnd();
		}
	}
	else
	{
		dx = dx / 2;
		dy = dy / 2;
		TreeMath(x, y, dx, dy, z1, (z1 + z2) / 2, (z1 + z2 + z3 + z4) / 4, (z1 + z4) / 2, min, Dc);
		TreeMath(x, y + dy, dx, dy, (z1 + z4) / 2, (z1 + z2 + z3 + z4) / 4, (z3 + z4) / 2, z4, min, Dc);
		TreeMath(x + dx, y + dy, dx, dy, (z1 + z2 + z3 + z4) / 4, (z2 + z3) / 2, z3, (z3 + z4) / 2, min, Dc);
		TreeMath(x + dx, y, dx, dy, (z1 + z2) / 2, z2, (z2 + z3) / 2, (z1 + z2 + z3 + z4) / 4, min, Dc);
	}
}


// 绘制边缘线
void COpenGLDC::DrawEdge(CPoint2D p1,CPoint2D p2)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glRectf(0.0f, 0.0f, 0.5f, 0.5f);// 换一个矩形  
	glPointSize(5.0f);// 设置点的大小

	glColor3f(1.0f, 0.0f, 0.0f);// 设置画笔颜色 为红色
	glBegin(GL_LINES);// 设置绘制模式为三角面片
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glEnd();

}


void COpenGLDC::DrawNonClosedRegion(CPoint3D  p1,CPoint3D  p2)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glRectf(0.0f, 0.0f, 0.5f, 0.5f);// 换一个矩形  
	glPointSize(8.0f);// 设置点的大小

	int RGB_rank = int(p1.z);
	glColor3d(r[RGB_rank], g[RGB_rank], b[RGB_rank]);

	glBegin(GL_LINES);
		glVertex2d(p1.x, p1.y);
		glVertex2d(p2.x, p2.y);
	glEnd();
}

// 绘制非封闭区域
void COpenGLDC::DrawNonClosedRegion(vector<CPoint3D> p_set)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glRectf(0.0f, 0.0f, 0.5f, 0.5f);// 换一个矩形  
	glPointSize(8.0f);// 设置点的大小

	int RGB_rank = int(p_set.at(0).z);
	glColor3d(r[RGB_rank], g[RGB_rank], b[RGB_rank]);

	glBegin(GL_LINES);
	for (int j = 1; j < p_set.size(); j++)
	{
		glVertex2d(p_set.at(j - 1).x, p_set.at(j - 1).y);
		glVertex2d(p_set.at(j).x, p_set.at(j).y);
		if (j == p_set.size() - 1)
		{
			glVertex2d(p_set.at(j).x, p_set.at(j).y);
			glVertex2d(p_set.at(0).x, p_set.at(0).y);
		}
	}
	glEnd();


	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glBegin(GL_POLYGON);
	//for (int j = 1; j < p_set.size(); j++)
	//{
	//	glVertex2d(p_set.at(j-1).x, p_set.at(j-1).y);
	//	glVertex2d(p_set.at(j).x, p_set.at(j).y);
	//	//if (j == p_set.size() - 1)
	//	//{
	//	//	glVertex2d(p_set.at(j).x, p_set.at(j).y);
	//	//	glVertex2d(p_set.at(0).x, p_set.at(0).y);
	//	//}
	//}
	//glEnd();
}

// 绘制彩色云图
void COpenGLDC::DrawCloudPicture(Scanner scanner)
{
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//GLUT_DOUBLE为双缓冲
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glClearColor(0, 0, 0, 0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);
	glLineWidth(6);

	glBegin(GL_LINE_STRIP); // 绘制扫描线
		for (int i = 0; i < scanner.rank.size(); i++)
		{
			if (i == 0)
			{
				glVertex2d(scanner.x_Set.at(i), scanner.y);
				continue;
			}
			int RGB_rank = int(scanner.rank.at(i));
			glColor3d(r[RGB_rank], g[RGB_rank], b[RGB_rank]);
			glVertex2d(scanner.x_Set.at(i), scanner.y);

			if (i == scanner.rank.size()-1)
			{
				glVertex2d(scanner.x_Set.at(i), scanner.y);
			}
		}
	glEnd();

}

// 绘制等值线
void COpenGLDC::DrawCloudPicture(double p1x, double p1y, double p2x, double p2y)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_LINE_SMOOTH);

	glBegin(GL_LINES);
	glVertex2d(p1x,p1y);
	glVertex2d(p2x, p2y);
	glEnd();

	glDisable(GL_LINE_SMOOTH);
}



void COpenGLDC::DrawArrowsModel(double x1, double y1, double x2, double y2, double x3, double y3,
																		double Vx1, double Vy1, double Vx2, double Vy2, double Vx3, double Vy3)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glRectf(0.0f, 0.0f, 0.5f, 0.5f);// 换一个矩形  
	glPointSize(5.0f);// 设置点的大小
	glColor3f(1.0f, 0.0f, 0.0f);// 设置画笔颜色 

	DrawArrows(x1, y1, Vx1, Vy1);
	DrawArrows(x2, y2, Vx2, Vy2);
	DrawArrows(x3, y3, Vx3, Vy3);
}

void COpenGLDC::DrawSingleModel(double x1, double y1, double x2, double y2, double x3, double y3,
													double Vx1, double Vy1, double Vx2, double Vy2, double Vx3, double Vy3 )
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glRectf(0.0f, 0.0f, 0.5f, 0.5f);// 换一个矩形  
	glPointSize(5.0f);// 设置点的大小

	glBegin(GL_TRIANGLES);// 设置绘制模式为三角面片
	glColor3f(0.0f, 0.0f, 1.0f);// 设置画笔颜色 为蓝色 
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();

	glBegin(GL_POINTS);// 绘制节点
	glColor3f(1.0f, 0.0f, 0.0f);// 设置画笔颜色 为红色 
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);// 设置画笔颜色 
	DrawArrows(x1, y1,Vx1, Vy1);
	DrawArrows(x2, y2,Vx2, Vy2);
	DrawArrows(x3, y3,Vx3, Vy3);
}

void COpenGLDC::DrawSingleModel(double x1, double y1, double x2, double y2, double x3, double y3,
	double Vx1, double Vy1, double Vx2, double Vy2, double Vx3, double Vy3,
	double strain1, double stress1, double strain2, double stress2, double strain3, double stress3)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glRectf(0.0f, 0.0f, 0.5f, 0.5f);// 换一个矩形  

	glPointSize(5.0f);// 设置点的大小

	glBegin(GL_TRIANGLES);// 设置绘制模式为三角面片
	glColor3f(0.0f, 0.0f, 1.0f);// 设置画笔颜色 为红色 
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();

	glBegin(GL_POINTS);// 绘制节点
	glColor3f(1.0f, 0.0f, 0.0f);// 设置画笔颜色 为红色 
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();

	DrawArrows(x1, y1, Vx1, Vy1);
	DrawArrows(x2, y2, Vx2, Vy2);
	DrawArrows(x3, y3, Vx3, Vy3);
}


//
void COpenGLDC::DrawTriChip(vector<CPoint3D> &dingdian, vector<CPoint3D> &dingdianfashi)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glBegin(GL_TRIANGLES);
	glNormal3d(dingdianfashi[0].x,dingdianfashi[0].y,dingdianfashi[0].z);
	glVertex3d(dingdian[0].x,dingdian[0].y,dingdian[0].z);

	glNormal3d(dingdianfashi[1].x,dingdianfashi[1].y,dingdianfashi[1].z);
	glVertex3d(dingdian[1].x,dingdian[1].y,dingdian[1].z);

	glNormal3d(dingdianfashi[2].x,dingdianfashi[2].y,dingdianfashi[2].z);
	glVertex3d(dingdian[2].x,dingdian[2].y,dingdian[2].z);
	glEnd();
}
//
void COpenGLDC::DrawTriChip(vector<CPoint3D> &dingdian, vector<CPoint3D> &dingdianfashi,vector< vector<float> > &vcolors)
{
	glEnable(GL_NORMALIZE);
	BOOL bLighting = IsLighting();
	Lighting(FALSE);

	glBegin(GL_TRIANGLES);
	glNormal3f(dingdianfashi[0].x,dingdianfashi[0].y,dingdianfashi[0].z);
	glColor3f(vcolors[0][0],vcolors[0][1],vcolors[0][2]);
	glVertex3f(dingdian[0].x,dingdian[0].y,dingdian[0].z);

	glNormal3f(dingdianfashi[1].x,dingdianfashi[1].y,dingdianfashi[1].z);
	glColor3f(vcolors[1][0],vcolors[1][1],vcolors[1][2]);
	glVertex3f(dingdian[1].x,dingdian[1].y,dingdian[1].z);

	glNormal3f(dingdianfashi[2].x,dingdianfashi[2].y,dingdianfashi[2].z);
	glColor3f(vcolors[2][0],vcolors[2][1],vcolors[2][2]);
	glVertex3f(dingdian[2].x,dingdian[2].y,dingdian[2].z);
	glEnd();

	Lighting(bLighting);

}
//
void COpenGLDC::DrawTriChip(double n0,double n1,double n2,double v00,double v01,double v02,
							double v10,double v11,double v12,double v20,double v21,double v22,vector< vector<float> > &vcolors)
{
	BOOL bLighting = IsLighting();
	Lighting(FALSE);

	glBegin(GL_TRIANGLES);
	glNormal3d(n0,n1,n2);
	glColor3f(vcolors[0][0],vcolors[0][1],vcolors[0][2]);
	glVertex3d(v00,v01,v02);
	glColor3f(vcolors[1][0],vcolors[1][1],vcolors[1][2]);
	glVertex3d(v10,v11,v12);
	glColor3f(vcolors[2][0],vcolors[2][1],vcolors[2][2]);
	glVertex3d(v20,v21,v22);
	glEnd();

	Lighting(bLighting);
}
//
void COpenGLDC::DrawTriChipwithtecture(double n0,double n1,double n2,double v00,double v01,double v02, double v10,double v11,double v12,double v20,double v21,double v22)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glBegin(GL_TRIANGLES);
	glNormal3d(n0,n1,n2);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(v00,v01,v02);
	glTexCoord2f(1.0f, 0.0f);glVertex3d(v10,v11,v12);
	glTexCoord2f(0.5f, 1.0f); glVertex3d(v20,v21,v22);
	glEnd();

}

//
void COpenGLDC::DrawTriChip(double n0,double n1,double n2,double v00,double v01,double v02,
					double v10,double v11,double v12,double v20,double v21,double v22,bool beselect)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	if (beselect)
	{
		COLORREF old_clr;
		GetMaterialColor(old_clr);
		SetMaterialColor(RGB(255,0,0));

		glBegin(GL_TRIANGLES);
		glNormal3d(n0,n1,n2);
		glVertex3d(v00,v01,v02);
		glVertex3d(v10,v11,v12);
		glVertex3d(v20,v21,v22);
		glEnd();
		SetMaterialColor(old_clr);
	}
	else
	{
		glBegin(GL_TRIANGLES);
		glNormal3d(n0,n1,n2);
		glVertex3d(v00,v01,v02);
		glVertex3d(v10,v11,v12);
		glVertex3d(v20,v21,v22);
		glEnd();
	}
}

void COpenGLDC::DrawTriChip(double n0, double n1, double n2, double v00, double v01, double v02,
	double v10, double v11, double v12, double v20, double v21, double v22)
{

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glBegin(GL_TRIANGLES);
	glNormal3d(n0, n1, n2);
	glVertex3d(v00, v01, v02);
	glVertex3d(v10, v11, v12);
	glVertex3d(v20, v21, v22);
	glEnd();
}

void COpenGLDC::DrawTriChip(double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glBegin(GL_TRIANGLES);
	glVertex3d(v00, v01, v02);
	glVertex3d(v10, v11, v12);
	glVertex3d(v20, v21, v22);
	glEnd();
}

void COpenGLDC::DrawTriChip(double v00, double v01, double v02, double v10, double v11,
	double v12, double v20, double v21, double v22, double v30, double v31, double v32, boolean b1,boolean b2)
{

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// 面1
	glBegin(GL_TRIANGLES);
	glVertex3d(v00, v01, v02);
	glVertex3d(v10, v11, v12);
	glVertex3d(v20, v21, v22);
	glEnd();
	// 面2
	glBegin(GL_TRIANGLES);
	glVertex3d(v00, v01, v02);
	glVertex3d(v10, v11, v12);
	glVertex3d(v30, v31, v32);
	glEnd();
	// 面3
	glBegin(GL_TRIANGLES);
	glVertex3d(v00, v01, v02);
	glVertex3d(v20, v21, v22);
	glVertex3d(v30, v31, v32);
	glEnd();
	// 面4
	glBegin(GL_TRIANGLES);
	glVertex3d(v10, v11, v12);
	glVertex3d(v20, v21, v22);
	glVertex3d(v30, v31, v32);
	glEnd();
}


void COpenGLDC::DrawQuads(vector<CPoint3D> waist1, vector<CPoint3D> waist2, vector<CVector3D> norm)
{
	//glFrontFace(GL_CW);
	BOOL bLighting = IsLighting();
	Lighting(TRUE);

	/*COLORREF old;
	GetMaterialColor(old);

	SetMaterialColor(RGB(128,128,256));*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin( GL_TRIANGLES );
	int n_gon=waist2.size();
	for( int a = 0; a < n_gon; a++ ) 
	{
		glNormal3f( norm[a].dx, norm[a].dy, norm[a].dz );

		//glVertex3f( waist1[a].x, waist1[a].y, waist1[a].z );
		//glVertex3f( waist2[a].x, waist2[a].y, waist2[a].z );

		//glVertex3f( waist2[(a+1)%n_gon].x, waist2[(a+1)%n_gon].y, waist2[(a+1)%n_gon].z );
		//glVertex3f( waist1[(a+1)%n_gon].x, waist1[(a+1)%n_gon].y, waist1[(a+1)%n_gon].z );
		glVertex3f( waist1[a].x, waist1[a].y, waist1[a].z );
		glVertex3f( waist2[a].x, waist2[a].y, waist2[a].z );
		glVertex3f( waist2[(a+1)%n_gon].x, waist2[(a+1)%n_gon].y, waist2[(a+1)%n_gon].z );

		glVertex3f( waist1[a].x, waist1[a].y, waist1[a].z );
		glVertex3f( waist2[(a+1)%n_gon].x, waist2[(a+1)%n_gon].y, waist2[(a+1)%n_gon].z );
		glVertex3f( waist1[(a+1)%n_gon].x, waist1[(a+1)%n_gon].y, waist1[(a+1)%n_gon].z );

	}
	glEnd();

	Lighting(bLighting);
	//SetMaterialColor(old);
	//glFrontFace(GL_CCW);
}
//Draw 3D Solid
void COpenGLDC::DrawSphere(const CPoint3D& cen,double r,const CVector3D& vec)
{
	glPushMatrix();
	glTranslatef(cen.x,cen.y,cen.z);

	CVector3D vecNY(0,-1,0);
	CVector3D axis = vecNY*vec;
	double ang = _AngleBetween(vecNY,vec);
	ang = ang*180/GL_PI;
	glRotatef(ang,axis.dx,axis.dy,axis.dz);

	auxSolidSphere(r);
	glPopMatrix();
}

void COpenGLDC::DrawSphere(const CPoint3D& cen,double r)
{
	double width,height;
	m_Camera.get_view_rect(width,height);
	double len = min(width,height);
	len *= 0.01;
	glPushMatrix();
	glTranslatef(cen.x,cen.y,cen.z);
	auxSolidSphere(len);
	glPopMatrix();
}

void COpenGLDC::DrawSphereb(const CPoint3D& cen,double r)
{
	COLORREF old_clr;
	GetMaterialColor(old_clr);
	SetMaterialColor(RGB(0,0,255));

	double width,height;
	m_Camera.get_view_rect(width,height);
	double len = min(width,height);
	len *= 0.01;
	glPushMatrix();
	glTranslatef(cen.x,cen.y,cen.z);
	auxSolidSphere(len);
	SetMaterialColor(old_clr);
	glPopMatrix();
}
void COpenGLDC::DrawSpherep(const CPoint3D& cen,double r)
{
	COLORREF old_clr;
	GetMaterialColor(old_clr);
	SetMaterialColor(RGB(255,0,255));

	double width,height;
	m_Camera.get_view_rect(width,height);
	double len = min(width,height);
	len *= 0.01;
	glPushMatrix();
	glTranslatef(cen.x,cen.y,cen.z);
	auxSolidSphere(len);
	SetMaterialColor(old_clr);
	glPopMatrix();
}
void COpenGLDC::DrawCylinder(const vector<CPoint3D> pdata, double r)
{
	COLORREF old_clr;
	GetMaterialColor(old_clr);
	SetMaterialColor(RGB(255,0,255));
	int n=pdata.size();
	for (int i=1;i<n;i++)
	{
		CVector3D li=pdata[i]-pdata[i-1];
		DrawCylinder(pdata[i-1],r,li);
	}
	SetMaterialColor(old_clr);
}
void COpenGLDC::DrawCylinder(const CPoint3D& cen,double r,const CVector3D& h)
{
	glPushMatrix();

	glTranslatef(cen.x,cen.y,cen.z);

	CVector3D vecNY(0,-1,0);
	CVector3D axis = vecNY*h;
	double ang = _AngleBetween(vecNY,h);
	ang = ang*180/GL_PI;
	glRotatef(ang,axis.dx,axis.dy,axis.dz);


	auxSolidCylinder(r,h.GetLength());
	glPopMatrix();
}

void COpenGLDC::DrawTorus(const CPoint3D& cen,const CVector3D& ax,double r_in,double r_out)
{
	glPushMatrix();

	glTranslatef(cen.x,cen.y,cen.z);

	CVector3D vecNY(0,-1,0);
	CVector3D axis = vecNY*ax;
	double ang = _AngleBetween(vecNY,ax);
	ang = ang*180/GL_PI;
	glRotatef(ang,axis.dx,axis.dy,axis.dz);


	auxSolidTorus(r_in,r_out);
	glPopMatrix();
}

void COpenGLDC::DrawCone(const CPoint3D& cen,double r,const CVector3D& h)
{
	glPushMatrix();

	glTranslatef(cen.x,cen.y,cen.z);

	CVector3D vecNY(0,-1,0);
	CVector3D axis = vecNY*h;
	double ang = _AngleBetween(vecNY,h);
	ang = ang*180/GL_PI;

	glRotatef(ang,axis.dx,axis.dy,axis.dz);

	GLfloat angle,x,y;
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0,0,h.GetLength());
		for(angle =0.0f;angle<(2.0f*GL_PI);angle += (GL_PI/8.0f))
		{
			x = r*sin(angle);
			y = r*cos(angle);
			glVertex2f(x,y);
		}
	glEnd();

	// Begin a new triangle fan to cover the bottom
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0f,0.0f);
		for(angle =0.0f;angle<(2.0f*GL_PI);angle += (GL_PI/8.0f))
		{
			x = r*sin(angle);
			y = r*cos(angle);
			glVertex2f(x,y);
		}
	glEnd();
	glPopMatrix();
}

////////////////////////////////////////

void COpenGLDC::DrawCoord()
{
	//const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
	//const GLubyte* gluVersion= gluGetString(GLU_VERSION); //返回当前GLU工具库版本
	//printf("OOpenGL实现的版本号：%s\n",OpenGLVersion );
	//printf("OGLU工具库版本：%s\n", gluVersion);
	//
	BOOL bLighting = IsLighting();
	Lighting(FALSE);

	double width,height;
	m_Camera.get_view_rect(width,height);
	double len = min(width,height);
	len *= 0.1;

	glRasterPos3d(1.1*len, 0, 0);
	DrawText("X");
	glRasterPos3d(0, 1.1*len, 0);
	DrawText("Y");
	glRasterPos3d(0, 0, 1.1*len);
	DrawText("Z");

	CPoint3D cPt,xPt,yPt,zPt;
	xPt.x = yPt.y = zPt.z = len;

	COLORREF old_clr;
	GetColor(old_clr);

	SetColor(RGB(255,0,0));

	glPushMatrix();	
	glTranslatef(0.8*len, 0.0f, 0.0f);
	glRotatef(90.0f,0.0f,1.0f,0.0f);
	glutWireCone(0.05*len,0.2*len,10,10);
	glPopMatrix();

	//axis-x: red
	DrawLine(cPt,xPt);

	//axis-y: green
	SetColor(RGB(0,255,0));

	glPushMatrix();	
	glTranslatef(0.0f, 0.8*len, 0.0f);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	glutWireCone(0.05*len,0.2*len,10,10);
	glPopMatrix();

	DrawLine(cPt,yPt);

	//axis-z: blue
	SetColor(RGB(0,0,255));

	glPushMatrix();	
	glTranslatef(0.0f, 0.0f, 0.8*len);
	glutWireCone(0.05*len,0.2*len,10,10);
	glPopMatrix();

	DrawLine(cPt,zPt);

	Lighting(bLighting);
	SetColor(old_clr);

}
//世界坐标转为屏幕坐标
void COpenGLDC::TransWorldtoScreen()
{
	wglMakeCurrent(m_hDC,m_hRC);
}
void COpenGLDC::EndTransWorldtoScreen()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glRenderMode(GL_RENDER);

	wglMakeCurrent(m_hDC,NULL);

}
//////////////////////////////////////////////////
void COpenGLDC::BeginSelection(int xPos,int yPos)
{
	m_bSelectionMode = TRUE;
	wglMakeCurrent(m_hDC,m_hRC);
	
	//set up selection buffer	
	glSelectBuffer(BUFFER_LENGTH,m_selectBuff);
	
	//switch to projection and save the matrix
	m_Camera.selection(xPos,yPos);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	InitNames();
}

int	COpenGLDC::EndSelection(UINT* items)
{
	m_bSelectionMode = FALSE;
	GLint hits = glRenderMode(GL_RENDER);
	for(int i=0;i<hits;i++)
	{
		items[i] = m_selectBuff[i*4+3];
	}

	wglMakeCurrent(m_hDC,NULL);
	return hits;
}

BOOL COpenGLDC::IsSelectionMode()
{
	return m_bSelectionMode;
}

void COpenGLDC::InitNames()
{
	glInitNames();
	glPushName(0);
}

void COpenGLDC::LoadName(UINT name)
{
	glLoadName(name);
}

void COpenGLDC::PushName(UINT name)
{
	glPushName(name);
}

void COpenGLDC::PopName()
{
	glPopName();
}

void COpenGLDC::CreateLight()
{
	GLfloat fGloablAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};

	GLfloat m_lightAmb0[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat m_lightDif0[]={0.5f,0.5f,0.5f,1.0f};
	GLfloat m_lightSpe0[]={0.3f,0.3f,0.3f,0.5f};
	//GLfloat m_lightPos0[]={1.0f,1.0f,1.0f,0.0f};

	GLfloat m_lightAmb1[]={0.1f,0.1f,0.1f,0.5f};
	GLfloat m_lightDif1[]={0.3f,0.3f,0.3f,0.5f};
	GLfloat m_lightSpe1[]={0.3f,0.3f,0.3f,1.0f};
	//GLfloat m_lightPos1[]={-0.3f,0.0f,1.0f,0.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, m_lightAmb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lightDif0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, m_lightSpe0);
	//glLightfv(GL_LIGHT0, GL_POSITION, m_lightPos0);
	SetLightDirection(1.0, 1.0, 0.5);

	glLightfv(GL_LIGHT1, GL_AMBIENT, m_lightAmb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, m_lightDif1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, m_lightSpe1);
	//glLightfv(GL_LIGHT1, GL_POSITION, m_lightPos1);
	SetLightDirection(-0.3f,0.5f,0.2f);

	//全局环境光设置，产生环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fGloablAmbient);/////
	GLfloat local_view[] =
	{0.0};
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	//双面光照（需要显示背面)
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);/////

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

}

//旋转视图
void COpenGLDC::RotateView(double dRotX,double dRotY,double dRotZ)
{
	if (dRotX)
	{
		m_Camera.rotate_x_view(dRotX);
	}

	if (dRotY)
	{
		m_Camera.rotate_y_view(dRotY);
	}

	if (dRotZ)
	{
		m_Camera.rotate_z_view(dRotZ);
	}
}

//绕给定矢量旋转
void COpenGLDC::RotateView(double dRot,VECTOR3D vect)
{
	m_Camera.rotate_view(dRot, vect);
}

//绕屏幕给定轴旋转
void COpenGLDC::RotateView(double dRot,short snXyz)
{
	m_Camera.rotate_view(dRot,snXyz);
}

//三维空间写文字
void COpenGLDC::DrawText(char* string,int flag)
{
	char* p = NULL;

	for (p = string; *p; p++)
	{
		if(flag==24)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
		else if(flag==18)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
		else
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
	}
}




//新添加+++++++++>>2011.6.10
void COpenGLDC::DrawTriChipG(double n0,double n1,double n2,double v00,double v01,double v02,
							double v10,double v11,double v12,double v20,double v21,double v22,bool beselect)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	if (beselect)
	{
		COLORREF old_clr;
		GetMaterialColor(old_clr);
		SetMaterialColor(RGB(8,140,160));

		glBegin(GL_TRIANGLES);
		glNormal3d(n0,n1,n2);
		glVertex3d(v00,v01,v02);
		glVertex3d(v10,v11,v12);
		glVertex3d(v20,v21,v22);
		glEnd();
		SetMaterialColor(old_clr);
	}
	else
	{
		glBegin(GL_TRIANGLES);
		glNormal3d(n0,n1,n2);
		glVertex3d(v00,v01,v02);
		glVertex3d(v10,v11,v12);
		glVertex3d(v20,v21,v22);
		glEnd();
	}
}

AUX_RGBImageRec * COpenGLDC::LoadBMP(CHAR *Filename)
{
	FILE *File=NULL;									// 文件句柄
	if (!Filename)												// 确保文件名已提供
	{
		return NULL;										// 如果没提供，返回 NULL
	}
	File=fopen(Filename,"r");							// 尝试打开文件
	if (File)														// 文件存在么?
	{
		fclose(File);											// 关闭句柄
		return auxDIBImageLoadA(Filename);	// 载入位图并返回指针
	}
	return NULL;											// 如果载入失败，返回 NULL
}

BOOL COpenGLDC::LoadTexture(char *file)
{
	int Status=FALSE;													// 状态指示器
	AUX_RGBImageRec *TextureImage[1];					// 创建纹理的存储空间
	memset(TextureImage,0,sizeof(void *)*1);					// 将指针设为 NULL
	// 载入位图，检查有无错误，如果位图没找到则退出
	if (TextureImage[0]=LoadBMP(file))
	{
		Status=TRUE;														// 将 Status 设为 TRUE
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texture[0]);								// 创建纹理

		// 使用来自位图数据生成 的典型纹理
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
	}

	if (TextureImage[0])												// 纹理是否存在
	{
		if (TextureImage[0]->data)									// 纹理图像是否存在
		{
			free(TextureImage[0]->data);							// 释放纹理图像占用的内存
		}

		free(TextureImage[0]);										// 释放图像结构
	}
	return Status;															// 返回 Status
}

//新添加+++++++++>>2011.7.7
BOOL  COpenGLDC::BuildTexture(char *szPathName, GLuint &texid)						// 载入图片并转换为纹理
{
	HDC			hdcTemp;												// DC用来保存位图
	HBITMAP		hbmpTemp;												// 保存临时位图
	IPicture	*pPicture;												// 定义IPicture Interface
	OLECHAR		wszPath[MAX_PATH+1];									// 图片的完全路径
	char		szPath[MAX_PATH+1];										// 图片的完全路径
	long		lWidth;													// 图像宽度
	long		lHeight;												// 图像高度
	long		lWidthPixels;											// 图像的宽带(以像素为单位)
	long		lHeightPixels;											// 图像的高带(以像素为单位)
	GLint		glMaxTexDim ;											// 保存纹理的最大尺寸

	if (strstr(szPathName, "http://"))									// 如果路径包含 http:// 则...
	{
		strcpy(szPath, szPathName);										// 把路径拷贝到 szPath
	}
	else																// 否则从文件导入图片
	{
		GetCurrentDirectory(MAX_PATH, szPath);							// 取得当前路径
		strcat(szPath, "\\");											// 添加字符"\"
		strcat(szPath, szPathName);										// 添加图片的相对路径
	}

	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);		// 把ASCII码转化为Unicode标准码
	HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);

	if(FAILED(hr))														// 如果导入失败
	{
		// 图片载入失败出错信息
		MessageBox (HWND_DESKTOP, "图片导入失败!\n(TextureLoad Failed!)", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// 返回 FALSE
	}

	hdcTemp = CreateCompatibleDC(GetDC(0));								// 建立窗口设备描述表
	if(!hdcTemp)														// 建立失败?
	{
		pPicture->Release();											// 释放IPicture
		// 图片载入失败出错信息
		MessageBox (HWND_DESKTOP, "图片导入失败!\n(TextureLoad Failed!)", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// 返回 FALSE
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// 取得支持的纹理最大尺寸

	pPicture->get_Width(&lWidth);										// 取得IPicture 宽度 (转换为Pixels格式)
	lWidthPixels	= MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);										// 取得IPicture 高度 (转换为Pixels格式)
	lHeightPixels	= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// 调整图片到最好的效果
	if (lWidthPixels <= glMaxTexDim)									// 图片宽度是否超过显卡最大支持尺寸
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f); 
	else																// 否则,将图片宽度设为显卡最大支持尺寸
		lWidthPixels = glMaxTexDim;

	if (lHeightPixels <= glMaxTexDim)									// 图片高度是否超过显卡最大支持尺寸
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
	else																// 否则,将图片高度设为显卡最大支持尺寸
		lHeightPixels = glMaxTexDim;

	// 建立一个临时位图
	BITMAPINFO	bi = {0};												// 位图的类型
	DWORD		*pBits = 0;												// 指向位图Bits的指针

	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// 设置结构大小
	bi.bmiHeader.biBitCount		= 32;									// 32 位
	bi.bmiHeader.biWidth		= lWidthPixels;							// 宽度像素值
	bi.bmiHeader.biHeight		= lHeightPixels;						// 高度像素值
	bi.bmiHeader.biCompression	= BI_RGB;								// RGB 格式
	bi.bmiHeader.biPlanes		= 1;									// 一个位平面

	// 建立一个位图这样我们可以指定颜色和深度 并访问每位的值
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);

	if(!hbmpTemp)														// 建立失败?
	{
		DeleteDC(hdcTemp);												// 删除设备描述表
		pPicture->Release();											// 释放IPicture
		// 图片载入失败出错信息
		MessageBox (HWND_DESKTOP, "图片导入失败!\n(TextureLoad Failed!)", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// 返回 FALSE
	}

	SelectObject(hdcTemp, hbmpTemp);									// 选择临时DC句柄和临时位图对象

	// 在位图上绘制IPicture
	pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

	// 将BGR转换为RGB　将ALPHA值设为255
	for(long i = 0; i < lWidthPixels * lHeightPixels; i++)				// 循环遍历所有的像素
	{
		BYTE* pPixel	= (BYTE*)(&pBits[i]);							// 获取当前像素
		BYTE  temp		= pPixel[0];									// 临时存储第一个颜色像素(蓝色)
		pPixel[0]		= pPixel[2];									// 将红色值存到第一位
		pPixel[2]		= temp;											// 将蓝色值存到第三位
		pPixel[3]		= 255;											// ALPHA值设为255
	}

	glGenTextures(1, &texid);											// 创建纹理

	// 使用来自位图数据生成 的典型纹理
	glBindTexture(GL_TEXTURE_2D, texid);								// 绑定纹理
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// 线形滤波
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 线形滤波

    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// 生成纹理
	glTexImage2D(GL_TEXTURE_2D, 0, 3, lWidthPixels, lHeightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);

	DeleteObject(hbmpTemp);												// 删除对象
	DeleteDC(hdcTemp);													// 删除设备描述表

	pPicture->Release();												// 释放 IPicture

	return TRUE;														// 返回 TRUE
}
//新添加+++++++++<<2011.7.7


