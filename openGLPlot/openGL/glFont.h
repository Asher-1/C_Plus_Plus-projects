// GLfont.h: interface for the CMiscGL class.
// 中文输出类：别人写的，整理by kidwei
// 原理：将文字写入内存dc，成为一张内存位图，再将位图输出到场景中
// 一、中文字体的使用方法：
// 1、添加头文件glFont.h
// 2、增加成员变量：GLfont font（也可以动态分配）
// 3、默认字体在构造函数已经初始化了，如果需要改变，在OpenGL初始化做如下调用：
//      font.CreateFont("宋体", 12, FW_DONTCARE,false,false,false)
// 4、文字输出： font.ShowText(-0.90,-0.99,"输出的文字",0,1,0); 其中坐标范围是-1，1。后面三个参数是颜色
// 二、英文文字使用方法：
// 1.2 步骤与上面相同
// 3. 必须在场景初始化的时候调用：  font.BuildEnglishFont(hdc,12);
// 4. 文字输出：font.glPrintEnglish(0,0,0,1,1,"this is my English font:%d",5);
// 5. 前两个参数是位置，中间三个参数是颜色，最后可以格式化输出
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISCGL_H__3CF8DCE9_7A54_4519_AA65_5B375C048664__INCLUDED_)
#define AFX_MISCGL_H__3CF8DCE9_7A54_4519_AA65_5B375C048664__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
class GLfont
{
 HFONT m_hFont;
 unsigned int base;
public:
 GLfont();
 virtual ~GLfont();
 void BuildEnglishFont(HDC hDC,int size); 
 void glPrintEnglish(float x,float y,float r,float g,float b,const char *fmt, ...);
 void CreateFont(char *facename, int height, int weight, bool italic,bool underline,bool strikeout);
 void ShowText(float x, float y, LPCTSTR lpszText,float r,float g,float b);

};

#endif // !defined(AFX_MISCGL_H__3CF8DCE9_7A54_4519_AA65_5B375C048664__INCLUDED_)



