// GLDrawView.cpp : 实现文件
//

#include "stdafx.h"
#include "GLDrawView.h"
#include "IndexLocationDlg.h"
// CGLDrawView
#define MAX_VERTICES 1000000
#define DATA_SIZE 100000

IMPLEMENT_DYNCREATE(CGLDrawView, CView)

CGLDrawView::CGLDrawView()
{
    data_ = NULL;
    dataSize_ = 0;
    m_hGLContext = NULL;
    textLists_ = 0;
    clientWidth_ = 1;
    clientHeight_ = 1;
    vcWidthRatio_ = 1.0;
    vcHightRatio_ = 1.0;
    viewHalfWidth_ = 20;
    viewHalfHight_ = 20;
    marginWidth_ = 50;
    marginHight_ = 50;
    translateX_ = 0.0;
    translateY_ = 0.0;
    bBtnDown_ = false;
    bCtrlDown_ = false;
    scaleX_ = 1;
    scaleY_ = 1;
    dispWidthPerUnitX_ = 2*viewHalfWidth_/20;
    dispHightPerUnitY_ = 2*viewHalfHight_/1000;
    charWidth_ = 1;
    charHight_ = 1;
}

CGLDrawView::~CGLDrawView()
{
    resetData();
}

BEGIN_MESSAGE_MAP(CGLDrawView, CView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()

    ON_WM_RBUTTONUP()
    ON_COMMAND(ID_DISP_INIT, OnDispInit)
    ON_COMMAND(ID_LOCATE_INDEX, OnLocateIndex)
    ON_COMMAND(ID_DISP_HELP, OnDispHelp)
END_MESSAGE_MAP()


// CGLDrawView 绘图

void CGLDrawView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
    DrawGLScene();
}


// CGLDrawView 诊断

#ifdef _DEBUG
void CGLDrawView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGLDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGLDrawView 消息处理程序

BOOL CGLDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此添加专用代码和/或调用基类
    cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);//openGL必需的

    return CView::PreCreateWindow(cs);
}

BOOL CGLDrawView::SetWindowPixelFormat( HDC hDC )
{
    PIXELFORMATDESCRIPTOR pixelDesc=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|
        PFD_DOUBLEBUFFER|PFD_SUPPORT_GDI,
        PFD_TYPE_RGBA,
        24,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
    };

    int glPixelIndex = 0;
    glPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);//选择最相近的像素格式
    if(glPixelIndex==0)
    {//选择失败
        glPixelIndex = 1;//默认的像素格式
        if(DescribePixelFormat(hDC,glPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
        {//用默认的像素格式进行设置
            return FALSE;
        }
    }

    if(SetPixelFormat(hDC,glPixelIndex,&pixelDesc)==FALSE)
    {
        return FALSE;
    }
    return TRUE;

}

BOOL CGLDrawView::CreateViewGLContext( HDC hDC )
{
    m_hGLContext = wglCreateContext(hDC);//创建RC
    if(m_hGLContext==NULL)
    {//创建失败
        return FALSE;
    }

    if(wglMakeCurrent(hDC,m_hGLContext)==FALSE)
    {//选为当前RC失败
        return FALSE;
    }
    return TRUE;
}

BOOL CGLDrawView::InitGL( GLvoid )
{
    glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
    glClearColor(1.0,1.0,1.0,0.0);// Background
    glClearDepth(1.0f);                                    // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations
    return TRUE;                                        // Initialization Went OK

}

BOOL CGLDrawView::InitTextLists()
{
    // 申请MAX_CHAR个连续的显示列表编号
    textLists_ = glGenLists(128);

    // 把每个字符的绘制命令都装到对应的显示列表中
    wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, textLists_);

    TEXTMETRIC tm;
    GetTextMetrics(wglGetCurrentDC(),&tm);
    charWidth_ = tm.tmAveCharWidth;
    charHight_ = tm.tmHeight;

    return TRUE;
}

BOOL CGLDrawView::InitVBO()
{
    glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
    if (0 == glGenBuffersARB)
    {
        return FALSE;
    }
    glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
    glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
    glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
    glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
    glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
    glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
    glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

    glGenBuffersARB(1, &vbo_);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, MAX_VERTICES*6*sizeof(GLfloat), 0, GL_DYNAMIC_DRAW_ARB);
    glVertexPointer(2, GL_FLOAT, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);

    return TRUE;
}

int CGLDrawView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    hDC_ = GetDC()->GetSafeHdc();
    if(SetWindowPixelFormat(hDC_)==FALSE)
    {//设置像素格式
        return 0;
    }
    if(CreateViewGLContext(hDC_)==FALSE)
    {//创建RC并选为所用
        return 0;
    }
    if(!InitGL())
    {//初始化openGL
        return 0;
    }
    if(!InitTextLists())
    {//初始化显示列表
        return 0;
    }
    if(!InitVBO())
    {//初始化vbo
        return 0;
    }

    return 0;
}

void CGLDrawView::OnDestroy()
{
    CView::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    if(wglGetCurrentContext()!=NULL)
    {
        wglMakeCurrent(NULL,NULL);
    }
    if(m_hGLContext!=NULL)
    {
        wglDeleteContext(this->m_hGLContext);
        m_hGLContext = NULL;
    }
}

void CGLDrawView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    
    //以下两行保证窗口大小变化后图像中心点不发生变化
    translateX_ *= cx*1.0/clientWidth_;
    translateY_ *= cy*1.0/clientHeight_;

    clientWidth_ = cx;
    clientHeight_ = cy;
    if (clientHeight_==0)                                        // Prevent A Divide By Zero By
    {
        clientHeight_=1;                                        // Making Height Equal One
    }
    if (clientWidth_==0)                                        // Prevent A Divide By Zero By
    {
        clientWidth_=1;                                        // Making Height Equal One
    }

    glViewport(0,0,clientWidth_,clientHeight_);                        // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix

    vcWidthRatio_ = viewHalfWidth_*2 / clientWidth_;
    vcHightRatio_ = viewHalfHight_*2 / clientHeight_;
    glOrtho(-viewHalfWidth_, viewHalfWidth_, -viewHalfHight_, viewHalfHight_, -100, 100);
    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();                                    // Reset The Modelview Matrix
}

int CGLDrawView::DrawGLScene( GLvoid )
{
    pixelPerUnitY_ = dispHightPerUnitY_*scaleY_/vcHightRatio_;
    pixelPerUnitX_ = dispWidthPerUnitX_*scaleX_/vcWidthRatio_;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear Screen And Depth Buffer

    drawCoordinate();
    drawRulerX();
    drawRulerY();
    drawData();
    drawMouseRect();

    ::SwapBuffers(hDC_);
    glFlush();
    return TRUE;                                        // Everything Went OK
}

void CGLDrawView::drawString( const char* str )
{
    // 调用每个字符对应的显示列表，绘制每个字符
    for(; *str!='\0'; ++str)
        glCallList(textLists_ + *str);
}

void CGLDrawView::drawCoordinate()
{
    glPushMatrix();

    glLoadIdentity();
    glTranslatef( -viewHalfWidth_+marginWidth_*vcWidthRatio_, -viewHalfHight_+marginHight_*vcHightRatio_, 0);

    //两条坐标轴
    glBegin(GL_LINES );
    glColor3f(0.0f,0.0f,1.0f);
    glVertex2f(0.0f,0.0f);
    glVertex2f(1000.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    glVertex2f(0.0f,1000.0f);
    glEnd();

    glPopMatrix();
}

void CGLDrawView::drawRulerX()
{
    //横坐标
    glPushMatrix();
    glLoadIdentity();
    glScalef(scaleX_,1,1);
    glTranslatef( -viewHalfWidth_+marginWidth_*vcWidthRatio_, -viewHalfHight_+marginHight_*vcHightRatio_, 0);

    glEnable(GL_SCISSOR_TEST);
    glScissor(marginWidth_,0,clientWidth_-marginWidth_,clientHeight_);
    glTranslatef( translateX_*vcWidthRatio_, 0, 0);

    //原点
    glBegin(GL_LINES );
    glColor3f(0.0f,0.0f,1.0f);
    glVertex2f(0.0,0.0);
    glVertex2f(0.0,5*vcHightRatio_);
    glEnd();
    glColor3f(1.0f,0.0f,1.0f);
    glRasterPos2f(0,-12*vcHightRatio_);
    drawString("0");

    UINT xInterval = 1;
    GLdouble stringWidth = charWidth_*7;
    if ( pixelPerUnitX_ > stringWidth )
        xInterval = 1;  //每个刻度都标识数字
    else if ( (pixelPerUnitX_ > stringWidth/5.0)&&(pixelPerUnitX_ <= stringWidth) )
        xInterval = 5;  //每5个刻度标识一个数字
    else if ( (pixelPerUnitX_ > stringWidth/10.0)&&(pixelPerUnitX_ <= stringWidth/5.0) )
        xInterval = 10; //每10个刻度标识一个数字
    else if ( (pixelPerUnitX_ > stringWidth/50.0)&&(pixelPerUnitX_ <= stringWidth/10.0) )
        xInterval = 50; //每50个刻度标识一个数字
    else if ( (pixelPerUnitX_ > stringWidth/100.0)&&(pixelPerUnitX_ <= stringWidth/50.0) )
        xInterval = 100; //每100个刻度标识一个数字
    else if ( (pixelPerUnitX_ > stringWidth/500.0)&&(pixelPerUnitX_ <= stringWidth/100.0) )
        xInterval = 500; //每500个刻度标识一个数字
    else if ( (pixelPerUnitX_ > stringWidth/1000.0)&&(pixelPerUnitX_ <= stringWidth/500.0) )
        xInterval = 1000; //每1000个刻度标识一个数字
	else
		xInterval = 5000;

    char strXValue[20];
    if (data_&&dataSize_)
    {
        //大致计算下横坐标用以显示的范围，以节省计算量。
        GLint indexSpan = (GLint)(clientWidth_/pixelPerUnitX_)/2+2;
        GLint startIndex = max(0,(GLint)((clientWidth_/2-translateX_)*scaleX_/pixelPerUnitX_)-indexSpan);
        GLint endIndex = min(DATA_SIZE,startIndex + 2*indexSpan);

        for (int i=startIndex;i<=endIndex;i++)
        {
            //刻度
            if ((xInterval==1)||(xInterval==5)||(2*i%xInterval==0))
            {
                glBegin(GL_LINES );
                glColor3f(0.0f,0.0f,1.0f);
                glVertex2f(i*dispWidthPerUnitX_,0.0);
                glVertex2f(i*dispWidthPerUnitX_,5*vcHightRatio_);
                glEnd();
            }
            //文字
            if ( i%xInterval == 0)
            {
                glColor3f(1.0f,0.0f,1.0f);
                glRasterPos2f(i*dispWidthPerUnitX_,-12*vcHightRatio_);
                sprintf(strXValue,"%d",i);
                drawString(strXValue);
            }
        }
        glPopMatrix();
    }

    glDisable(GL_SCISSOR_TEST);
    glPopMatrix();
}

void CGLDrawView::drawRulerY()
{
    //纵坐标
    glPushMatrix();
    glLoadIdentity();
    glScalef(1,scaleY_,1);
    glTranslatef( -viewHalfWidth_+marginWidth_*vcWidthRatio_, -viewHalfHight_+marginHight_*vcHightRatio_, 0);

    glEnable(GL_SCISSOR_TEST);
    glScissor(0,marginHight_,clientWidth_,clientHeight_-marginHight_);
    glTranslatef( 0, translateY_*vcHightRatio_, 0);

    //原点
    glBegin(GL_LINES );
    glColor3f(0.0f,0.0f,1.0f);
    glVertex2f(0.0,0.0);
    glVertex2f(0.0,5*vcHightRatio_);
    glEnd();
    glColor3f(1.0f,0.0f,1.0f);
    glRasterPos2f(-2*charWidth_*vcWidthRatio_,0);
    drawString("0");

    UINT yInterval = 1;
    GLdouble stringHight = charHight_*3;
    if ( pixelPerUnitY_ > stringHight )
        yInterval = 1;  //每个刻度都标识数字
    else if ( (pixelPerUnitY_ > stringHight/5.0)&&(pixelPerUnitY_ <= stringHight) )
        yInterval = 5;  //每5个刻度标识一个数字
    else if ( (pixelPerUnitY_ > stringHight/10.0)&&(pixelPerUnitY_ <= stringHight/5.0) )
        yInterval = 10; //每10个刻度标识一个数字
    else if ( (pixelPerUnitY_ > stringHight/50.0)&&(pixelPerUnitY_ <= stringHight/10.0) )
        yInterval = 50; //每50个刻度标识一个数字
    else if ( (pixelPerUnitY_ > stringHight/100.0)&&(pixelPerUnitY_ <= stringHight/50.0) )
        yInterval = 100; //每100个刻度标识一个数字
    else if ( (pixelPerUnitY_ > stringHight/500.0)&&(pixelPerUnitY_ <= stringHight/100.0) )
        yInterval = 500; //每500个刻度标识一个数字
    else if ( (pixelPerUnitY_ > stringHight/1000.0)&&(pixelPerUnitY_ <= stringHight/500.0) )
        yInterval = 1000; //每1000个刻度标识一个数字
    else if ( (pixelPerUnitY_ > stringHight/5000.0)&&(pixelPerUnitY_ <= stringHight/1000.0) )
        yInterval = 5000;
    else
        yInterval = 10000;

    //刻度
    char strYValue[10];
    for (int j=1;j<=30000;j++)
    {
        //刻度
        if ((yInterval==1)||(yInterval==5)||(2*j%yInterval==0))
        {
            glBegin(GL_LINES );
            glColor3f(0.0f,0.0f,1.0f);
            glVertex2f(0.0,j*dispHightPerUnitY_);
            glVertex2f(5*vcWidthRatio_,j*dispHightPerUnitY_);
            glEnd();
        }
        //文字
        if (j%yInterval==0)
        {
            glColor3f(1.0f,0.0f,1.0f);
            glRasterPos2f(-40*vcWidthRatio_,j*dispHightPerUnitY_);
            sprintf(strYValue,"%d",j);
            drawString(strYValue);
        }
    }
    glPopMatrix();
}

void CGLDrawView::drawData()
{
    //绘制具体坐标点
    glPushMatrix();
    glLoadIdentity();                                    // Reset The Current Modelview Matrix
    glScalef(scaleX_,scaleY_,1);
    glTranslatef( -viewHalfWidth_+marginWidth_*vcWidthRatio_, -viewHalfHight_+marginHight_*vcHightRatio_, 0);

    glEnable(GL_SCISSOR_TEST);
    glScissor(marginWidth_,marginWidth_,clientWidth_-marginWidth_,clientHeight_-marginWidth_);

    glTranslatef( translateX_*vcWidthRatio_, translateY_*vcHightRatio_, 0);

    if (data_&&dataSize_)
    {
        glColor3f(1.0,0.0,0.0);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_);
        glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, dataSize_* 2 *sizeof(GLfloat), data_);
        glDrawArrays(GL_LINE_STRIP , 0, dataSize_);
    }

    glDisable(GL_SCISSOR_TEST);
    glPopMatrix();

}

void CGLDrawView::drawMouseRect()
{
    if (bCtrlDown_)
    {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef( -viewHalfWidth_, -viewHalfHight_, 0);
        glEnable(GL_LINE_STIPPLE); //启动虚线模式 
        glLineStipple(2, 0xAAAA);  
        glBegin(GL_LINE_STRIP );
        glColor3f(0.0,0.0,1.0);
        glVertex2f(initPoint_.x*vcWidthRatio_, (clientHeight_-initPoint_.y)*vcHightRatio_);
        glVertex2f(initPoint_.x*vcWidthRatio_, (clientHeight_-lastPoint_.y)*vcHightRatio_);
        glVertex2f(lastPoint_.x*vcWidthRatio_, (clientHeight_-lastPoint_.y)*vcHightRatio_);
        glVertex2f(lastPoint_.x*vcWidthRatio_, (clientHeight_-initPoint_.y)*vcHightRatio_);
        glVertex2f(initPoint_.x*vcWidthRatio_, (clientHeight_-initPoint_.y)*vcHightRatio_);
        glEnd();
        glDisable(GL_LINE_STIPPLE);
        glPopMatrix();
    }
}

void CGLDrawView::resetData()
{
    if (data_)
    {
        delete []data_;
        data_ = NULL;
        dataSize_ = 0;
    }
}

BOOL CGLDrawView::loadData()
{
    resetData();

	srand(time(0));
    dataSize_ = DATA_SIZE;	//随机十万个点
    data_ = new GLfloat[dataSize_*2];     //二维平面绘制
    for (UINT i=0;i<DATA_SIZE;i++)
    {
        data_[2*i] = (i)*dispWidthPerUnitX_;
        data_[2*i+1] = (rand()%1000)*dispHightPerUnitY_;
    }
    return TRUE;
}

void CGLDrawView::OnLButtonDown( UINT nFlags, CPoint point )
{
    //::SetCursor(AfxGetApp()->LoadCursor(IDC_CROSS));
    SetCapture();
    bBtnDown_ = true;
    if (nFlags&MK_CONTROL)
    {
        bCtrlDown_ = true;
    }
    lastPoint_ = point;
    initPoint_=point;

    CView::OnLButtonDown(nFlags, point);
}

void CGLDrawView::OnLButtonUp( UINT nFlags, CPoint point )
{
    ReleaseCapture();
    bBtnDown_ = false;
    if (bCtrlDown_)
    {
        CRect rt(initPoint_,lastPoint_);
        fit2Rect(rt);
        bCtrlDown_ = false;
    }
    DrawGLScene();

    CView::OnLButtonUp(nFlags, point);
}

void CGLDrawView::OnMouseMove( UINT nFlags, CPoint point )
{
    if(GetCapture()!=this)
        return;

    if (!bBtnDown_)
        return;

    if (bCtrlDown_)
    {
        //窗口缩放只允许右下方向拖动
        lastPoint_.x = max(point.x,initPoint_.x);
        lastPoint_.y = max(point.y,initPoint_.y);
    }
    else
    {
        //平移
        translateX_ += (point.x - lastPoint_.x)/scaleX_;
        translateY_ -= (point.y - lastPoint_.y)/scaleY_;    //屏幕坐标与gl坐标y向相反
        lastPoint_ = point;
    }
    DrawGLScene();

    CView::OnMouseMove(nFlags, point);
}

BOOL CGLDrawView::OnMouseWheel( UINT nFlags,short zDelta,CPoint pt )
{
    if(nFlags&MK_LBUTTON)
    {
        return FALSE;
    }

    scaleX_ *= (1 + ((double)zDelta/WHEEL_DELTA)/10.0);
    scaleX_ = max(scaleX_,0.001);
    scaleY_ *= (1 + ((double)zDelta/WHEEL_DELTA)/10.0);
    scaleY_ = max(scaleY_,0.001);
    DrawGLScene();

    return TRUE;
}

void CGLDrawView::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CMenu m_menu,*psubmenu;
    if   (!m_menu.LoadMenu(IDR_MENU_DISP))   
    { 
        return   ; 
    } 
    if   (!(psubmenu=m_menu.GetSubMenu(0)))   
    { 
        return   ; 
    } 
    SetForegroundWindow();  
    SetMenu(psubmenu);
    ClientToScreen(&point);   
    psubmenu-> TrackPopupMenu(   TPM_LEFTALIGN   |   TPM_RIGHTBUTTON,   point.x,   point.y,   this); 
    m_menu.DestroyMenu(); 
    CView::OnRButtonDown(nFlags, point);

    CView::OnRButtonUp(nFlags, point);
}

void CGLDrawView::OnDispInit()
{
    // TODO: 在此添加命令处理程序代码
    scaleX_ = scaleY_ = 1;
    translateX_=translateY_=0;
    DrawGLScene();
}

void CGLDrawView::OnLocateIndex()
{
    // TODO: 在此添加命令处理程序代码
    if ((!data_)||(!dataSize_))
        return;
    
    CIndexLocationDlg dlg;
    if (IDOK != dlg.DoModal())
        return;
    
    UINT Index = dlg.m_EditIndex;
    if (Index>dataSize_)
    {
        AfxMessageBox(_T("超出范围!!!"));
        return;
    }
    translateX_ = -((Index-3)*pixelPerUnitX_)/scaleX_;
	translateY_ = 0;
    scaleX_ = 1;
    scaleY_ = 1;
    DrawGLScene();
}

void CGLDrawView::fit2Rect( CRect &rect )
{
    if ( (!rect.Width())||(!rect.Height()) )
    {
        return;
    }
    //CRect drawRect(marginWidth_,0,clientWidth_,clientHeight_-marginHight_);
    CRect drawRect(0,0,clientWidth_,clientHeight_);
    LONG deltaX = rect.CenterPoint().x - drawRect.CenterPoint().x;
    LONG deltaY = rect.CenterPoint().y - drawRect.CenterPoint().y;
    translateX_ -= deltaX/scaleX_;
    translateY_ += deltaY/scaleY_;
    scaleX_ *= drawRect.Width()/rect.Width();
    scaleY_ *= drawRect.Height()/rect.Height();
}
void CGLDrawView::OnDispHelp()
{
    // TODO: 在此添加命令处理程序代码
    AfxMessageBox(_T(" 鼠标左键+拖动：平移\n 滚轮：缩放\n ctrl+鼠标左键+拖动：窗口缩放\n"));
}
