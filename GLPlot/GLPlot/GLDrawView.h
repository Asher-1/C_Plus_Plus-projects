#pragma once

#include "glut\glut.h"
#include "glut\glInfo.h"
#include "glut\glext.h"

using namespace std;

// CGLDrawView 视图

class CGLDrawView : public CView
{
	DECLARE_DYNCREATE(CGLDrawView)

public:
	CGLDrawView();
	virtual ~CGLDrawView();

    PFNGLGENBUFFERSARBPROC glGenBuffersARB;                     // VBO Name Generation Procedure
    PFNGLBINDBUFFERARBPROC glBindBufferARB;                     // VBO Bind Procedure
    PFNGLBUFFERDATAARBPROC glBufferDataARB;                     // VBO Data Loading Procedure
    PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;               // VBO Sub Data Loading Procedure
    PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;               // VBO Deletion Procedure
    PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB; // return various parameters of VBO
    PFNGLMAPBUFFERARBPROC glMapBufferARB;                       // map VBO procedure
    PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;                   // unmap VBO procedure

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    BOOL SetWindowPixelFormat(HDC hDC);	//设置像素格式
    BOOL CreateViewGLContext(HDC hDC);	//创建绘制环境(RC)并使之成为当前绘制环境
    BOOL InitGL(GLvoid);				//初始化openGL
    BOOL InitTextLists();				//初始化文字显示列表
    BOOL InitVBO();						//初始化VBO
    int DrawGLScene(GLvoid);			//绘图代码区
    void drawString(const char* str);   //显示文字
    void drawCoordinate();				//坐标线
    void drawRulerX();					//横坐标标尺、刻度、数字
    void drawRulerY();					//纵坐标标尺、刻度、数字
    void drawData();					//具体坐标值
    void drawMouseRect();				//窗口缩放的矩形框
    void fit2Rect(CRect &rect);			//窗口缩放
	void resetData();

    HGLRC m_hGLContext;     //绘制环境
    HDC hDC_;				//设备环境
    GLuint vbo_;			//vbo标识
    GLdouble viewHalfWidth_,viewHalfHight_;     //视景体半宽高
    GLsizei clientWidth_,clientHeight_;         //窗口宽高
    GLfloat vcWidthRatio_,vcHightRatio_;        //视窗比
    GLsizei marginWidth_,marginHight_;          //非绘图区域
    
    GLuint textLists_;	//文字显示列表
    LONG charWidth_;	//字符平均字宽
    LONG charHight_;	//字符高度

    GLfloat translateX_,translateY_;	//横纵坐标平移量
    GLfloat scaleX_,scaleY_;			//横纵坐标缩放量
    bool bBtnDown_;		//鼠标左键按下
    bool bCtrlDown_;	//CTRL键按下
    CPoint initPoint_;	//鼠标左键按下时的坐标
    CPoint lastPoint_;	//移动中保存的临时坐标

    GLdouble dispHightPerUnitY_;	//Y轴每单位在视景体中的长度
    GLdouble pixelPerUnitY_;		//Y轴每单位占窗口的像素值
    GLdouble dispWidthPerUnitX_;	//X轴每单位在视景体中的长度
    GLdouble pixelPerUnitX_;		//X轴每单位占窗口的像素值

    GLfloat* data_;    //数据
    GLuint dataSize_;  //数据大小

public:
	BOOL loadData();

public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags,short zDelta,CPoint pt );
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnDispInit();
    afx_msg void OnLocateIndex();
    afx_msg void OnDispHelp();

};
