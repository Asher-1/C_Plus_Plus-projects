#pragma once

#include "glut\glut.h"
#include "glut\glInfo.h"
#include "glut\glext.h"

using namespace std;

// CGLDrawView 视图

class CGLDrawView : public CView
{

public:
	CGLDrawView();
	virtual ~CGLDrawView();

protected:
    BOOL InitGL(GLvoid);				//初始化openGL
    int DrawGLScene(GLvoid);			//绘图代码区
    void drawString(const char* str);   //显示文字
    void drawCoordinate();				//坐标线
    void drawRulerX();					//横坐标标尺、刻度、数字
    void drawRulerY();					//纵坐标标尺、刻度、数字
    void drawData();					//具体坐标值
    void drawMouseRect();				//窗口缩放的矩形框
    void fit2Rect(CRect &rect);			//窗口缩放
	void resetData();


    GLdouble viewHalfWidth_,viewHalfHight_;     //视景体半宽高
    GLsizei clientWidth_,clientHeight_;         //窗口宽高
    GLfloat vcWidthRatio_,vcHightRatio_;        //视窗比
    GLsizei marginWidth_,marginHight_;          //非绘图区域
    
    GLuint textLists_;	//文字显示列表
    LONG charWidth_;	//字符平均字宽
    LONG charHight_;	//字符高度

    GLfloat translateX_,translateY_;	//横纵坐标平移量
    GLfloat scaleX_,scaleY_;			//横纵坐标缩放量

    GLdouble dispHightPerUnitY_;	//Y轴每单位在视景体中的长度
    GLdouble pixelPerUnitY_;		//Y轴每单位占窗口的像素值
    GLdouble dispWidthPerUnitX_;	//X轴每单位在视景体中的长度
    GLdouble pixelPerUnitX_;		//X轴每单位占窗口的像素值

    GLfloat* data_;    //数据
    GLuint dataSize_;  //数据大小

public:
	BOOL loadData();
};
