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



// CGLDrawView 绘图

void CGLDrawView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
    DrawGLScene();
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



void CGLDrawView::resetData()
{
    if (data_)
    {
        delete []data_;
        data_ = NULL;
        dataSize_ = 0;
    }
}





